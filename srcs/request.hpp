#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>

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
        const METHOD get_method();
        const std::string &get_uri();
        const std::string &get_version();
        const std::map<std::string, std::string> &get_headers();
    private:
        const static int BUF_MAX = 1024;
        void parse();
        int fd;
        std::map<std::string, std::string> headers;
        std::string identify_method(METHOD method);
        METHOD identify_method(std::string method);
        METHOD method;
        std::string uri;
        std::string version;
        void print_request();
};

#endif
