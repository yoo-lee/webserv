#ifndef REQUEST_H
#define REQUEST_H

#include "Config.hpp"
#include "byte_vector.hpp"
#include "content_type.hpp"
#include "http_method.hpp"
#include "raw_request_reader.hpp"
#include "socket_data.hpp"
#include "splitted_string.hpp"
#include "transfer_encoding.hpp"
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

    void print_request() const;

    // -- request line --
    const string& get_version() const;
    HttpMethod get_method() const;
    const string& get_path() const;
    vector<string> get_path_list() const;

    // -- request header --
    const map<string, string>& get_headers() const;
    string get_domain() const;
    string get_ip_address() const;
    ssize_t get_content_length() const;
    string get_transfer_encoding() const;
    bool is_cgi() const;
    bool is_cgi(string path) const;
    std::string& get_host();
    ContentType const& get_content_type() const;
    std::string& get_port();

    // -- config --
    Config const* get_config() const; // これいらなくない？
    Server const* get_server_config() const;
    Location const* get_location_config() const;

    // -- body --
    // bodyはメモリに格納できないレベルで大きい場合があるので、（一旦ファイルに書き込む（予定））
    ByteVector read_body(); // will move to private
    ByteVector get_body_text();
    ByteVector get_body() const;
    vector<ByteVector> get_body_splitted() const;
    bool is_full_body_loaded() const;
    // vector<path> get_body_tmp_file_list(); // 未実装(実装予定 #39)

  private:
    const static int BUF_MAX = 1600;
    // -- init --
    void parse();
    void parse_server_config();
    void parse_location_config();
    void parse_request_line();
    void parse_header_field();
    void parse_content_length();
    void validate(); // いらなくない？
    // void save_tmp_file(ByteVector bytes); // 未実装(実装予定 #39)

    // -- util --
    bool is_contents_upload_file(string const& content_type);

    // -- 内部変数 --
    const int _fd;
    ByteVector _loaded_packet_body;
    RawRequestReader _buf;
    vector<path> _tmp_body_file_list;

    // -- request line --
    string _path;
    HttpMethod _method;
    string _version;

    // -- request header --
    ssize_t _content_length;
    TransferEncoding _transfer_encoding;
    map<string, string> _headers;
    ByteVector _body;
    string _err_line;
    string _domain;
    string _ip;
    string _port;
    string _host;
    ContentType _content_type;
};

#endif /* REQUEST_H */
