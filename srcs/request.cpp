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
using std::tolower;
using std::vector;

#define BODY_TMP_DIRECTORY_PATH "/tmp/webserv_body_tmp/"

Request::Request(int fd_, Config const& config)
    : SocketData(config),
      _fd(fd_),
      _buf(this->_fd),
      _is_full_body_chunk_loaded(false),
      _method(HttpMethod::INVALID),
      _content_length(0),
      _err_line("")
{
    this->parse();
    // parse_server_config(); 未実装
    // parse_location_config(); 未実装

    const vector<Server const*> server_list = _config->http->server;
}

Request::Request(int fd_, Config const& config, string& port)
    : SocketData(config),
      _fd(fd_),
      _buf(this->_fd),
      _is_full_body_chunk_loaded(false),
      _method(HttpMethod::INVALID),
      _content_length(0),
      _err_line(""),
      _port(port)
{
    this->parse();
    // parse_server_config(); 未実装
    // parse_location_config(); 未実装

    const vector<Server const*> server_list = _config->http->server;
}

Request::~Request() {}

void Request::print_request() const
{
    cout << "|-- Print Request Header --|" << endl;
    cout << " fd: " << _fd << endl;
    cout << " method: " << _method.get_str() << endl;
    cout << " version: " << _version << endl;

    cout << " headers size: " << _headers.size() << endl;
    cout << " path: " << _path << endl;
    cout << " path dep: " << get_path_list().size() << endl;
    cout << " content-type: " << _content_type << endl;
    cout << " content-length: " << _content_length << endl;
    map<string, string>::const_iterator ite = _headers.begin();
    map<string, string>::const_iterator end = _headers.end();
    for (; ite != end; ite++) {
        cout << " _headers:" << (*ite).first << ": " << (*ite).second << endl;
    }
    cout << "|--------------------------|" << endl;
}

ssize_t get_content_length_from_chunk(ByteVector str)
{
    cout << "DEBUG str length: " << str.size() << endl;
    cout << "DEBUG str: " << str << endl;
    SplittedString ss(str.get_str(), "\r\n");
    cout << "DEBUG split: " << ss << endl;
    if (ss.size() < 2)
        throw std::runtime_error("check_body_contain_hex_length() size is not enough");
    return static_cast<ssize_t>(Utility::hex_string_to_int(ss[0]));
}

void Request::parse()
{
    parse_request_line();
    parse_header_field();

    parse_content_length();
    _transfer_encoding = TransferEncoding(_headers["transfer-encoding"]);
    _content_type = ContentType(_headers);
    ByteVector tmp_loaded_packet_body = read_body();
    cout << "DEBUG tmp_loaded_packet_body: " << tmp_loaded_packet_body << endl;
    if (_transfer_encoding == TransferEncoding::CHUNKED && _content_length == -1 && _method == HttpMethod::POST) {
        // transfer encodingは最初の行にlenが書いてあるので、それを読み込む
        try {
            _content_length = get_content_length_from_chunk(tmp_loaded_packet_body);
        } catch (std::exception& e) {
            throw std::runtime_error("This Post Request's Transfer-Encoding is chunked and Content-Length is not "
                                     "found. But chunked length is not found in body.");
        }
    }

    // tmpファイルに保存する場合はここにif文を作り分岐させる #39
    _loaded_packet_body = tmp_loaded_packet_body;
    validate();
}

void Request::parse_request_line()
{
    string request_line = _buf.getline();
    if (request_line == _buf.last_str) {
        return;
    }
    std::cout << "request line: " << request_line << std::endl;
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
    _method = HttpMethod(*request_line_words_it);
    _path = Utility::trim_white_space(*(++request_line_words_it));
    _version = Utility::trim_white_space(*(++request_line_words_it));
}

void Request::parse_header_field()
{
    string key;
    string value;
    std::string::size_type split_pos;
    string line = _buf.getline();
    while ((line != _buf.last_str)) {
        line = _buf.getline();
        split_pos = line.find(":");
        if (split_pos == string::npos || line.size() <= 0) {
            break;
        }
        key = line.substr(0, split_pos);
        key = Utility::trim_white_space(key);
        key = Utility::trim_white_space(key);
        if (key.size() < 2) {
            break;
        }
        std::transform(key.begin(), key.end(), key.begin(), static_cast<int (*)(int)>(std::tolower));
        value = line.substr(split_pos + 1);
        value = Utility::trim_white_space(value);
        this->_headers.insert(make_pair(key, value));
    }
}

void Request::parse_content_length()
{
    string size_str = _headers["content-length"];
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

// void Request::save_tmp_file(ByteVector bytes) // 未実装(#39)
// {
//     std::cout << "save_tmp_file: " << bytes << std::endl;
// }

HttpMethod Request::get_method() const
{
    return this->_method;
}

const string& Request::get_version() const
{
    return (this->_version);
}

const map<string, string>& Request::get_headers() const
{
    return (this->_headers);
}

ByteVector Request::get_body_text()
{
    throw std::runtime_error("This method will be removed So you may use get_body()");
}

// vector<path> Request::get_body_tmp_file_list() 未実装(#39)
// {
//     return _tmp_body_file_list;
// }

bool is_end_chunk(ByteVector bytes)
{
    string non_spitted = bytes.get_str();
    vector<string> splitted;
    while (non_spitted.find("\r\n") != string::npos) {
        splitted.push_back(non_spitted.substr(0, non_spitted.find("\r\n")));
        non_spitted = non_spitted.substr(non_spitted.find("\r\n") + 2);
    }
    // cout << "non_splitted: " << non_spitted << endl;
    // cout << "splitted.size():" << splitted.size() << endl;
    // for (size_t i = 0; i < splitted.size(); i++) {
    //     cout << "splitted[" << i << "]:" << splitted[i] << endl;
    // }
    return false;
}

// reading body from socket(fd)
ByteVector Request::read_body()
{
    if (is_full_body_loaded())
        return ByteVector();
    ByteVector bytes;
    if (_buf.get_extra_buf().size() > 0)
        bytes = this->_buf.get_extra_buf();
    else
        bytes = this->_buf.get_body(BUF_MAX);

    _body.insert(_body.end(), bytes.begin(), bytes.end());
    return bytes;
}

string const& Request::get_path() const
{
    return _path;
}

// hoge/fuga/piyo -> hoge, fuga, piyo
vector<string> Request::get_path_list() const
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

string Request::get_ip_address() const
{
    // todo
    return ("127.0.0.1");
}

string Request::get_domain() const
{
    // todo
    return ("test.com");
}

ssize_t Request::get_content_length() const
{
    return (this->_content_length);
}

string Request::get_transfer_encoding() const
{
    return (this->_transfer_encoding.get_str());
}

ByteVector Request::get_body() const
{
    if (!is_full_body_loaded())
        throw std::runtime_error("Request::get_body() body is not loaded");
    return _body;
}

vector<ByteVector> Request::get_body_splitted() const
{
    if (!is_full_body_loaded())
        throw std::runtime_error("Request::get_body_splitted() body is not loaded");
    if (!_content_type.is_multipart())
        throw std::runtime_error("Request::get_body_splitted() can only use  multipart/*. This content-type is " +
                                 _content_type.get_media_type());

    string boundary = _content_type.get_boundary();
    vector<ByteVector> body_list;
    string body = _body.get_str();

    if (body.find(boundary) == string::npos) {
        throw std::runtime_error("Request::get_body_splitted() boundary is not found");
    }

    SplittedString ss(body, boundary + "\r\n");
    vector<string>::iterator its = ss.begin();
    vector<string>::iterator ite = ss.end();
    while (its != ite) {
        body_list.push_back(ByteVector((*its).c_str(), (*its).length()));
        its++;
    }
    return body_list;
}

// そもそもvalidateが必要なのか？
void Request::validate() {}

bool Request::is_cgi() const
{
    return is_cgi(_path);
}

Server const* Request::get_server_config() const
{
    try {
        return _config->http->get_server(_headers.at("Host"));
    } catch (const std::exception& e) {
        return _config->http->server.at(0);
    }
}

bool is_prefix(const std::string& strA, const std::string& strB)
{
    if (strB.size() > strA.size()) {
        return false;
    }
    return strA.find(strB) == 0;
}

Location const* Request::get_location_config() const // 未実装 (sanoさん待ち)
{
    // vector<Location* const> maybe_current_locations;
    // Server const* server = get_server_config();
    // for (size_t j = 0; j < server->location.size(); j++) {
    //     Location* current = const_cast<Location*>(server->location[j]);
    //     for (size_t k = 0; k < current->urls.size(); k++)
    //         if (is_prefix(current->urls[k], _path) && _port == current->properties["listen"])
    //             maybe_current_locations.push_back(current);
    // }
    // if (maybe_current_locations.size() == 0)
    //     return 0;
    // if (maybe_current_locations.size() == 1)
    return 0;
}

bool Request::is_cgi(string path) const
{
    Server const* server = get_server_config();
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

Config const* Request::get_config() const
{
    return _config;
}

bool Request::is_full_body_loaded() const
{
    if (_method == HttpMethod::GET)
        return true; // ここは例外を発生させたほうが誤用が少なくなってよいかも？
    if (_transfer_encoding == TransferEncoding::CHUNKED)
        return _is_full_body_chunk_loaded;
    return _body.size() >= static_cast<unsigned long>(_content_length);
}

std::string& Request::get_port()
{
    return _port;
}

std::string& Request::get_host()
{
    return _host;
}

ContentType Request::get_content_type() const
{
    return _content_type;
}