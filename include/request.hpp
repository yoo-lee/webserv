#ifndef REQUEST_H
#define REQUEST_H

#include "Config.hpp"
#include "get_next_line.hpp"
#include "socket_data.hpp"
#include "split.hpp"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;
class Request : public SocketData
{
  public:
    Request();
    Request(int fd, Config const& config);
    ~Request();
    const string& get_path();
    vector<string> get_path_list();
    const string& get_version();
    const string& get_body_size();
    METHOD get_method();
    const string get_method_string();
    const map<string, string>& get_headers();
    void print_request();
    int read_body(char* buf); // will move to private
    int read_buf(char* buf);
    char* get_body(int* size);
    string get_domain();
    string get_ip_address();
    ssize_t get_content_length();
    ssize_t get_loaded_body_size();
    string get_transfer_encoding();
    void add_loaded_body_size(size_t size);
    bool analyze();
    bool have_data_in_body();
    bool is_cgi();
    bool is_cgi(string path) const;
    // static string identify_method(METHOD method);
    // static METHOD identify_method(string method);
    // bool increment_timeout(int time);
    // void clear_timeout();

    // bodyはメモリに格納できないレベルで大きい場合があるので、（一旦ファイルに書き込む（予定））
  private:
    const static int BUF_MAX = 1600;
    void parse();
    string search_header(string);
    string& get_next_line(int fd);

    const int _fd;
    ssize_t _content_length;
    ssize_t _loaded_body_size;
    string _transfer_encoding;
    char _loaded_packet_body[BUF_MAX];
    RawRequestReader _buf;
    map<string, string> _headers;
    METHOD _method;
    string _path;
    string _version;
    string _err_line;
    string _domain;
    string _ip;
    bool _data_in_body;
    bool _cgi;
    int _body_size;
};

#endif /* REQUEST_H */
