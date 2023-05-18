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
    Request(int fd, Config const& config, std::string& port);
    ~Request();
    const string& get_path() const;
    vector<string> get_path_list() const;
    const string& get_version() const;
    ByteVector get_body() const;
    vector<ByteVector> get_body_splitted() const;
    ssize_t get_loaded_body_size() const;
    const string& get_body_size() const;
    METHOD get_method() const;
    const string get_method_string() const;
    const map<string, string>& get_headers() const;
    void print_request() const;
    ByteVector read_body(); // will move to private
    ByteVector get_body_text();
    string get_domain() const;
    string get_ip_address() const;
    ssize_t get_content_length() const;
    string get_transfer_encoding() const;
    void add_loaded_body_size(size_t size);
    void validate();
    bool is_cgi() const;
    bool is_cgi(string path) const;
    Config const* get_config() const;
    vector<path> get_body_tmp_file_list();
    Server const* get_server_config() const;
    Location const* get_location_config() const;
    void parse_server_config();
    void parse_location_config();
    bool is_full_body_loaded() const;
    std::string& get_port();
    std::string& get_host();
    ContentType get_content_type() const;
    // static string identify_method(METHOD method);
    // static METHOD identify_method(string method);
    // bool increment_timeout(int time);
    // void clear_timeout();

    // bodyはメモリに格納できないレベルで大きい場合があるので、（一旦ファイルに書き込む（予定））
  private:
    const static int BUF_MAX = 1600;
    void parse();
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
    ByteVector _body;
    string _version;
    string _err_line;
    string _domain;
    string _ip;
    string _port;
    string _host;
    ContentType _content_type;
};

#endif /* REQUEST_H */
