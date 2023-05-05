#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "get_next_line.hpp"
#include "socket_data.hpp"
#include "split.hpp"
#include <map>
#include <string>

class Request : public SocketData
{
  public:
    Request();
    Request(int fd);
    ~Request();
    const std::string& get_path();
    const std::string& get_uri(); // will remove
    const std::string& get_version();
    const std::string& get_body_size();
    METHOD get_method();
    const std::string get_method_string();
    const std::map<std::string, std::string>& get_headers();
    void print_request();
    int read_buf(char* buf);
    std::string get_domain();
    std::string get_ip_address();
    ssize_t get_content_length();
    ssize_t get_loaded_body_size();
    std::string get_transfer_encoding();
    void add_loaded_body_size(size_t size);
    bool analyze();
    bool have_data_in_body();
    bool is_cgi();
    bool is_cgi(string path) const;
    // static std::string identify_method(METHOD method);
    // static METHOD identify_method(std::string method);
    // bool increment_timeout(int time);
    // void clear_timeout();
  private:
    void parse();
    std::string search_header(std::string);
    const static int BUF_MAX = 1600;
    const int fd;
    // int _body_size;
    ssize_t _content_length;
    ssize_t _loaded_body_size;
    std::string _transfer_encoding;
    // char* _body;
    char buf[BUF_MAX];
    GetNextLine _gnl;
    std::map<std::string, std::string> headers;
    METHOD method;
    std::string uri; // will remove
    std::string path;
    std::string version;
    std::string& get_next_line(int fd);
    std::string err_line;
    std::string domain;
    std::string ip;
    // const static int _timeout = 200;
    // int _timeout_cnt;
    bool _data_in_body;
    bool _cgi;
};

#endif
