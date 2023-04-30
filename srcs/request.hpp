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
    INVALID,
};

typedef enum E_METHOD METHOD;

class Request
{
    public:
        Request();
        Request(int fd);
        ~Request();
        const METHOD get_method();
		const std::string get_method_string();
        const std::string &get_uri();
        const std::string &get_version();
        const std::map<std::string, std::string> &get_headers();
        void print_request();
        int read_buf(char *buf);
        static std::string identify_method(METHOD method);
        static METHOD identify_method(std::string method);
    private:
        void parse();
        const static int BUF_MAX = 1024;
        const int fd;
        int buf_size = 0;
        char extra_buf[BUF_MAX];
        std::map<std::string, std::string> headers;
        METHOD method;
        std::string uri;
        std::string version;
        std::string &get_next_line(int fd);
        std::string err_line = "";
        //Split *sp;
};

#endif
