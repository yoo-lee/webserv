#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>
#include "split.hpp"

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
};

typedef enum E_METHOD METHOD;

class Request
{
    public:
        Request(int fd);
        ~Request();
        METHOD get_method();
        const std::string &get_path();
        const std::string &get_uri(); // will remove
        const std::string &get_version();
        const std::map<std::string, std::string> &get_headers();
        void print_request();
        int read_buf(char *buf);
        std::string get_domain();
        std::string get_ip_address();
    private:
        void parse();
        const static int BUF_MAX = 1024;
        const int fd;
        int buf_size;
        char extra_buf[BUF_MAX];
        std::map<std::string, std::string> headers;
        std::string identify_method(METHOD method);
        METHOD identify_method(std::string method);
        METHOD method;
        std::string uri; //will remove
        std::string path;
        std::string version;
        std::string &get_next_line(int fd);
        std::string err_line;
        std::string domain;
        std::string ip;
        //Split *sp;
};

#endif
