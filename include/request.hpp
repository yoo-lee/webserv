#ifndef REQUEST_H
#define REQUEST_H

#include "get_next_line.hpp"
#include "socket_data.hpp"
#include "split.hpp"
#include <map>
#include <string>

using std::map;
using std::string;

class Request : public SocketData
{
  public:
    Request();
    Request(int fd);
    ~Request();
    const string& get_path();
    const string& get_version();
    const string& get_body_size();
    METHOD get_method();
    const string get_method();
    const map<string, string>& get_headers();
    void print_request();
    int read_buf(char* buf);
    string get_domain();
    string get_ip_address();
    ssize_t get_content_length();
    ssize_t get_loaded_body_size();
    string get_transfer_encoding();
    void add_loaded_body_size(size_t size);
    bool analyze();
    bool have_data_in_body();
    bool is_cgi();
    // static string identify_method(METHOD method);
    // static METHOD identify_method(string method);
    // bool increment_timeout(int time);
    // void clear_timeout();
  private:
    const static int BUF_MAX = 1600;
    void parse();
    string search_header(string);
    string& get_next_line(int fd);

    const int _fd;
    ssize_t _content_length;
    ssize_t _loaded_body_size;
    string _transfer_encoding;
    char _buf[BUF_MAX];
    GetNextLine _gnl;
    map<string, string> _headers;
    METHOD _method;
    string _path;
    string _version;
    string _err_line;
    string _domain;
    string _ip;
    bool _data_in_body;
    bool _cgi;
};

#endif /* REQUEST_H */
