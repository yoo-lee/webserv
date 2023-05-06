#include "request.hpp"
#include "Config.hpp"
#include "get_next_line.hpp"
#include "split.hpp"
#include "utility.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;

Request::Request(int fd_)
    : fd(fd_),
      _content_length(0),
      _loaded_body_size(0),
      _gnl(this->fd),
      method(NG),
      err_line(""),
      _data_in_body(false),
      _cgi(false)
{
    this->parse();
}

Request::~Request()
{
    // delete _body;
    // close(this->fd);
}

void Request::print_request()
{
    cout << "Print Request!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "method: " << identify_method(this->method) << endl;
    cout << "uri: " << this->uri << endl;
    cout << "version: " << this->version << endl;

    cout << "headers size:" << this->headers.size() << endl;
    ;
    map<string, string>::iterator ite = this->headers.begin();
    map<string, string>::iterator end = this->headers.end();
    int i = 0;
    for (; ite != end; ite++) {
        cout << (*ite).first << ":" << (*ite).second << endl;
        i++;
    }
}

void Request::parse()
{
    string str = _gnl.getline();
    if (str == _gnl.last_str) {
        return;
    }
    Split sp(str, " ");
    if (sp.size() != 3) {
        cout << "size:" << sp.size() << endl;
        cout << "str:[" << str << "]" << endl;
        str = _gnl.getline();
        cout << "str:[" << str << "]" << endl;
        cout << "Error:not 3 factor" << endl;
        throw std::exception();
    }
    Split::iterator ite = sp.begin();
    this->method = identify_method(*ite);
    this->uri = *(++ite);
    const char* path = this->uri.c_str();
    size_t cnt = 0;
    while (path && *path) {
        if (*path != '/')
            break;
        cnt++;
        path++;
    }
    this->path = this->uri.substr(cnt);
    this->path = Utility::delete_space(this->path);
    this->version = *(++ite);
    this->version = Utility::delete_space(this->version);
    string header;
    string value;
    std::string::size_type pos;
    while ((str != _gnl.last_str)) {
        str = _gnl.getline();
        pos = str.find(":");
        if (pos == string::npos || str.size() <= 0) {
            break;
        }
        header = str.substr(0, pos);
        header = Utility::delete_space(header);
        header = Utility::delete_space(header);
        if (header.size() < 2) {
            break;
        }
        std::transform(header.begin(), header.end(), header.begin(), tolower);
        value = str.substr(pos + 1);
        value = Utility::delete_space(value);
        this->headers.insert(make_pair(header, value));
    }
    string size_str = this->search_header("content-length");
    ssize_t size = -1;
    if (size_str.size() > 10) {
        cout << "Error: exceed BODY SIZE MAX" << endl;
    }
    if (size_str != "") {
        std::stringstream ss;
        ss << size_str;
        ss >> size;
    }
    this->_content_length = size;
    this->_transfer_encoding = this->search_header("transfer-encoding");
}

METHOD Request::get_method()
{
    return (this->method);
}

const std::string Request::get_method_string()
{
    return (identify_method(this->method));
}

const string& Request::get_uri()
{
    return (this->uri);
}

const string& Request::get_version()
{
    return (this->version);
}

const map<string, string>& Request::get_headers()
{
    return (this->headers);
}

int Request::read_buf(char* buf)
{
    int size = this->_gnl.get_extra_buf(buf);
    if (size > 0)
        return (size);
    return (_gnl.get_body(&(buf[size]), BUF_MAX));
}

const string& Request::get_path()
{
    return (this->path);
}

string Request::get_ip_address()
{
    // todo
    return ("127.0.0.1");
}

string Request::get_domain()
{
    // todo
    return ("test.com");
}

ssize_t Request::get_content_length()
{
    return (this->_content_length);
}

string Request::get_transfer_encoding()
{
    return (this->_transfer_encoding);
}

ssize_t Request::get_loaded_body_size()
{
    return (this->_loaded_body_size);
}

void Request::add_loaded_body_size(size_t size)
{
    this->_loaded_body_size += size;
}

string Request::search_header(string header)
{
    (void)header;
    std::map<std::string, std::string>::const_iterator ite;
    ite = this->headers.find(header);
    if (ite == this->headers.end()) {
        return ("");
    }
    return (ite->second);
}

bool Request::analyze()
{
    _data_in_body = false;
    _cgi = false;
    return (true);
}

bool Request::have_data_in_body()
{
    _data_in_body = true;
    return (_data_in_body);
}

bool Request::is_cgi()
{
    return is_cgi(this->path);
}

bool Request::is_cgi(string path) const
{
    const Server* server = _config->http->get_server(this->headers["Host"]);
    for (size_t j = 0; j < server->location.size(); j++) {
        Location* current = const_cast<Location*>(server->location[j]);
        for (size_t k = 0; k < current->urls.size(); k++) {
            if (current->urls[k] == path && (*current)["cgi_path"][0] != "") {
                return (true);
            }
        }
    }
    return (false);
}
