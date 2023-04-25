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
        const std::string &get_body_size();
        const std::map<std::string, std::string> &get_headers();
        void print_request();
        int read_buf(char **buf);
        std::string get_domain();
        std::string get_ip_address();
    private:
        void parse();
        std::string search_header(std::string);
        const static int BUF_MAX = 1024;
        const int fd;
        int _body_size;
        char* _body;
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
        void load_header(GetNextLine& gnl, char *buf);
        void load_body(GetNextLine& gnl, char *buf);
        //Split *sp;
};

#endif
