#ifndef REQUEST_H
#define REQUEST_H

#include "Config.hpp"
#include "byte_vector.hpp"
#include "content_type.hpp"
#include "raw_request_reader.hpp"
#include "socket_data.hpp"
#include "splitted_string.hpp"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

typedef string path;
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
    ByteVector read_body(); // will move to private
    ByteVector get_body_text();
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
    vector<path> get_body_tmp_file_list();
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
    void parse_request_line();
    void parse_header_field();
    void parse_content_length();
    void parse_content_type();
    bool is_contents_upload_file(string const& content_type);
    void save_tmp_file(ByteVector bytes);

    const int _fd;
    ssize_t _loaded_body_size;
    ByteVector _loaded_packet_body;
    RawRequestReader _buf;
    vector<path> _tmp_body_file_list;

    ssize_t _content_length;
    string _transfer_encoding;

    map<string, string> _headers;
    METHOD _method;
    string _path;
    string _version;
    string _err_line;
    string _domain;
    string _ip;
    ContentType _content_type;

    bool _is_data_in_body;
    bool _is_cgi;
};

#endif /* REQUEST_H */
