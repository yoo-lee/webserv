#ifndef Utility_HPP
#define Utility_HPP
#include <sstream>
#include <string>
#include <unistd.h>
#include "Server.hpp"
#include "Config.hpp"
#include "utility.hpp"

// template<typename T>
class Utility
{
  public:
    static char* memcpy(char* dst, const char* src, size_t n);
    static size_t strlen(const char* str);
    static std::string delete_space(std::string& str);
    static char* strnstr(const char* haystack, const char* needle, size_t len);
    static std::string to_lower(std::string str);
    static std::string trim_white_space(std::string str);

    template <typename T>
    static std::string to_string(T value);
    static std::string to_string2(int value);
    static Server const* get_cfg_server(const Config &cfg, string &port, string &host);
    static std::vector<std::string> get_cfg_locations(const Config &cfg, string &port, string &host) ;
    //static std::vector<std::string> get_cfg_locations_contents(const Config &cfg, string &port, string &location);
    static std::map<std::string, std::vector<std::string> > get_cfg_locations_contents(const Config &cfg, string &port, string &host, string &location);


    //static std::vector<std::string> get_cfg_extension(const Config &cfg, string &port);

    //static std::map<std::string, std::vector<std::string> > _cfg_root_dir;
    static std::map< std::pair<std::string, std::string>, Server const *> _cfg_servers;
    static std::map< std::pair<std::string, std::string>, std::vector<std::string> > _cfg_locations;
    //static std::map<std::string, std::vector<std::string> > _cfg_locations_contents;
    //static std::map<pair<std::string, std::string> , std::vector<std::map<std::string, std::vector<std::string> > > > _cfg_locations_contents;
    static std::map<pair<std::pair<std::string, std::string>, std::string> , std::map<std::string, std::vector<std::string> > > _cfg_locations_content;
    static std::string delete_duplicated_slash(std::string str);
};

template <typename T>
std::string Utility::to_string(T value)
{
    std::stringstream sss;
    std::string str;

    sss << value;
    str = sss.str();

    return (str);
}

#endif
