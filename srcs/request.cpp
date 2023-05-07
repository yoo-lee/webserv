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

Request::Request(int fd_, Config const& config)
    : SocketData(config),
      _fd(fd_),
      _content_length(0),
      _loaded_body_size(0),
      _buf(this->_fd),
      _method(NG),
      _err_line(""),
      _data_in_body(false),
      _cgi(false),
      _body_size(0)
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
    string str = _buf.getline();
    if (str == _buf.last_str) {
        return;
    }
    std::cout << "str: " << str << std::endl;
    SplittedString sp(str, " ");
    if (sp.size() != 3) {
        cout << "size:" << sp.size() << endl;
        cout << "str:[" << str << "]" << endl;
        str = _buf.getline();
        cout << "str:[" << str << "]" << endl;
        cout << "Error:not 3 factor" << endl;
        throw std::exception();
    }
    std::cout << sp << std::endl;
    SplittedString::iterator ite = sp.begin();
    this->_method = str_to_method(*ite);
    this->_path = Utility::delete_space(*(++ite));
    this->_version = Utility::delete_space(*(++ite));
    string header;
    string value;
    std::string::size_type pos;
    while ((str != _buf.last_str)) {
        str = _buf.getline();
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
        std::transform(header.begin(), header.end(), header.begin(), static_cast<int (*)(int)>(std::tolower));
        value = str.substr(pos + 1);
        value = Utility::delete_space(value);
        this->_headers.insert(make_pair(header, value));
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

    // ここでボディをファイルに保存する場合は保存してそうじゃない場合はbufに保存する
    this->_body_size = this->read_body().get_length();
    this->add_loaded_body_size(this->_body_size);
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

ByteVector Request::get_body(size_t size)
{
    return (ByteVector(_loaded_packet_body, size));
}

string get_body_tmp_file_path()
{
    return "file_path";
}

ByteVector Request::read_buf()
{
    return (ByteVector(_loaded_packet_body, _body_size));
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
