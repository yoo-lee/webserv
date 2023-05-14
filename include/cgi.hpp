#ifndef CGI_HPP
#define CGI_HPP
#include "request.hpp"
#include <map>
#include <string>

enum E_CGI{
    PHP,
    RUBY,
    PYTHON2,
    PYTHON3,
    PERL,
};

class CGI{

    public:
        CGI(Request *req);
        int fork_parse();
        int get_result_fd();
    private:
        void parse_args(Request &req);
        void parse_args(std::string& uri);
        void make_cgi_process();
        void parse();
        void check_executable_path();
        void get_config_path();
        //std::string exe_command;
        std::string _cgi_filepath;
        //std::string _cgi_extension;
        std::string _query;
        std::string _path_info;
        std::map<std::string, std::string> args;
        void parse_cgi_file(std::string &uri, Request &req);
        int result_fd;
        Request* _req;

};

#endif
