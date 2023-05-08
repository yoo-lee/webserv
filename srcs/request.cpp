#include "request.hpp"
#include "Config.hpp"
#include "raw_request_reader.hpp"
#include "splitted_string.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cctype>
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

#define BODY_TMP_DIRECTORY_PATH "/tmp/webserv_body_tmp/"

Request::Request(int fd_, Config const& config)
    : SocketData(config),
      _fd(fd_),
      _loaded_body_size(0),
      _buf(this->_fd),
      _content_length(0),
      _method(NG),
      _err_line(""),
      _is_data_in_body(false),
      _is_cgi(false)
{
    this->parse();
    // TODO: configの絞り込み
    // server_list = config.server
    // for server in server_list
    // {
    //     if server->server_name == get_domain(url) && server->port == this->_port
    //         {
    //             this->server = server;
    //             break;
    //         }
    // }
}

Request::~Request() {}

void Request::print_request()
{
    cout << "|----- Print Request -----|" << endl;
    cout << " method: " << method_to_str(this->_method) << endl;
    cout << " version: " << this->_version << endl;

    cout << " headers size:" << this->_headers.size() << endl;
    cout << " path" << this->_path << endl;
    map<string, string>::iterator ite = this->_headers.begin();
    map<string, string>::iterator end = this->_headers.end();
    for (; ite != end; ite++) {
        cout << " " << (*ite).first << ": " << (*ite).second << endl;
    }
    cout << " path dep: " << get_path_list().size() << endl;
    cout << "|-------------------------|" << endl;
}

void Request::parse()
{
    parse_request_line();
    parse_header_field();

    parse_content_length();
    _transfer_encoding = this->search_header("transfer-encoding");
    parse_content_type();

    ByteVector tmp_loaded_packet_body = this->read_body();
    if (_content_type.is_form_data())
        save_tmp_file(tmp_loaded_packet_body);
    else
        _loaded_packet_body = tmp_loaded_packet_body;

    this->add_loaded_body_size(tmp_loaded_packet_body.size());
}

void Request::parse_request_line()
{
    string request_line = _buf.getline();
    if (request_line == _buf.last_str) {
        return;
    }
    std::cout << "str: " << request_line << std::endl;
    SplittedString request_line_words(request_line, " ");
    if (request_line_words.size() != 3) {
        cout << "size:" << request_line_words.size() << endl;
        cout << "str:[" << request_line << "]" << endl;
        request_line = _buf.getline();
        cout << "str:[" << request_line << "]" << endl;
        cout << "Error:not 3 factor" << endl;
        throw std::exception();
    }
    std::cout << request_line_words << std::endl;
    SplittedString::iterator request_line_words_it = request_line_words.begin();
    _method = str_to_method(*request_line_words_it);
    _path = Utility::delete_space(*(++request_line_words_it));
    _version = Utility::delete_space(*(++request_line_words_it));
}

void Request::parse_header_field()
{
    string key;
    string value;
    std::string::size_type split_pos;
    string line;
    while ((line != _buf.last_str)) {
        line = _buf.getline();
        split_pos = line.find(":");
        if (split_pos == string::npos || line.size() <= 0) {
            break;
        }
        key = line.substr(0, split_pos);
        key = Utility::delete_space(key);
        key = Utility::delete_space(key);
        if (key.size() < 2) {
            break;
        }
        std::transform(key.begin(), key.end(), key.begin(), static_cast<int (*)(int)>(std::tolower));
        value = line.substr(split_pos + 1);
        value = Utility::delete_space(value);
        this->_headers.insert(make_pair(key, value));
    }
}

void Request::parse_content_length()
{
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
}

void Request::parse_content_type()
{
    _content_type = ContentType(_headers);
}

void Request::save_tmp_file(ByteVector bytes)
{
    std::cout << "save_tmp_file: " << bytes << std::endl;
}

METHOD Request::get_method()
{
    return (this->_method);
}

const std::string Request::get_method_string()
{
    return (method_to_str(this->_method));
}

const string& Request::get_version()
{
    return (this->_version);
}

const map<string, string>& Request::get_headers()
{
    return (this->_headers);
}

ByteVector Request::get_body_text()
{
    if (!this->_content_type.is_form_data())
        return _loaded_packet_body;
    throw std::runtime_error("Request::get_body_text() does not use  multipart/form-data");
}

vector<path> Request::get_body_tmp_file_list()
{
    return _tmp_body_file_list;
}

ByteVector Request::read_body()
{
    ByteVector bytes = this->_buf.get_extra_buf();
    if (bytes.size() > 0)
        return bytes;
    return (_buf.get_body(BUF_MAX));
}

string const& Request::get_path()
{
    return _path;
}

// hoge/fuga/piyo -> hoge, fuga, piyo
vector<string> Request::get_path_list()
{
    vector<string> path_list;
    string buf;
    size_t i = 0;
    while (_path[i] != 0) {
        if ((_path[i] == '/' || i == _path.length() - 1) && buf != "") {
            path_list.push_back(buf);
            buf = "";
        } else
            buf += _path[i];
        i++;
    }
    return (path_list);
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
    ite = _headers.find(header);
    if (ite == _headers.end()) {
        return ("");
    }
    return (ite->second);
}

bool Request::analyze()
{
    _is_data_in_body = false;
    _is_cgi = false;
    return (true);
}

bool Request::have_data_in_body()
{
    _is_data_in_body = true;
    return (_is_data_in_body);
}

bool Request::is_cgi()
{
    return is_cgi(_path);
}

bool Request::is_cgi(string path) const
{
    const Server* server;
    try {
        server = _config->http->get_server(_headers.at("Host"));
    } catch (const std::exception& e) {
        server = _config->http->server.at(0);
    }
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
