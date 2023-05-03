#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>
#include "split.hpp"
#include "get_next_line.hpp"

enum E_METHOD{
    GET,
    POST,
    PUT,
    HEAD,
    DELETE,
    OPTIONS,
    TRACE,
    CONNECT,
    NG,
    INVALID,
};

typedef enum E_METHOD METHOD;

class Request
{
    public:
        Request();
        Request(int fd);
        ~Request();
        const std::string &get_path();
        const std::string &get_uri(); // will remove
        const std::string &get_version();
        const std::string &get_body_size();
        METHOD get_method();
        const std::string get_method_string();
        const std::map<std::string, std::string> &get_headers();
        void print_request();
        int read_buf(char *buf);
        std::string get_domain();
        std::string get_ip_address();
        ssize_t get_content_length();
        ssize_t get_loaded_body_size();
        std::string get_transfer_encoding();
        void add_loaded_body_size(size_t size);
        bool analyze();
        bool is_loaded_body();
        bool is_cgi();
        static std::string identify_method(METHOD method);
        static METHOD identify_method(std::string method);
    private:
        void parse();
        std::string search_header(std::string);
        const static int BUF_MAX = 1024;
        const int fd;
        //int _body_size;
        ssize_t _content_length;
        ssize_t _loaded_body_size;
        std::string _transfer_encoding;
        //char* _body;
        char buf[BUF_MAX];
        GetNextLine _gnl;
        std::map<std::string, std::string> headers;
        METHOD method;
        std::string uri; //will remove
        std::string path;
        std::string version;
        std::string &get_next_line(int fd);
        std::string err_line;
        std::string domain;
        std::string ip;
        const static int _timeout = 1000;
        int _timeout_cnt;
        bool _loaded_body;
        bool _cgi;
};

#endif
