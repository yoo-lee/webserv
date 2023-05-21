
#include "utility.hpp"
#include "splitted_string.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::cout;
using std::endl;
using std::make_pair;
using std::string;

#include <stdio.h>

size_t Utility::strlen(const char* str)
{
    size_t cnt = 0;
    while (str[cnt]) {
        cnt++;
    }
    return (cnt);
}

char* Utility::memcpy(char* dst, const char* src, size_t n)
{
    size_t i;
    unsigned char* dst_p;
    unsigned char* src_p;

    if (n == 0)
        return (dst);
    else if (dst == NULL && src == NULL)
        return (NULL);
    i = n;
    dst_p = (unsigned char*)dst;
    src_p = (unsigned char*)src;
    while (i--) {
        // cout << "i=" << i << endl;
        *(dst_p++) = *(src_p++);
    }
    return (dst);
}

static char* process(const char* haystack, const char* needle, size_t len)
{
    size_t i;
    size_t k;
    size_t len_n;

    i = -1;
    k = 0;
    len_n = Utility::strlen(needle);
    if (len_n == 0)
        return ((char*)haystack);
    if (len_n > Utility::strlen(haystack) || len_n > len)
        return (NULL);
    while (++i < len) {
        if (haystack[i] == needle[k])
            k++;
        else {
            i -= k;
            k = 0;
        }
        if (len_n == k)
            return (&(((char*)haystack)[i - len_n + 1]));
    }
    return (NULL);
}

char* Utility::strnstr(const char* haystack, const char* needle, size_t len)
{
    if (len == 0) {
        if (needle == NULL || needle[0] == '\0')
            return ((char*)haystack);
        return (NULL);
    }
    return (process(haystack, needle, len));
}

string Utility::to_lower(string str)
{
    string result = "";
    for (size_t i = 0; i < str.length(); i++) {
        result += tolower(str[i]);
    }
    return (result);
}

string Utility::trim_white_space(string str)
{
    std::string::size_type left = str.find_first_not_of("\t\n\v\f\r ");
    if (left != std::string::npos) {
        std::string::size_type right = str.find_last_not_of("\t\n\v\f\r ");
        return str.substr(left, right - left + 1);
    } else
        return "";
}

Server const* Utility::get_cfg_server(const Config& cfg, string& port, string& host)
{
    std::map<std::pair<std::string, std::string>, Server const*>::iterator cash_ite =
        Utility::_cfg_servers.find(make_pair(port, host));
    if (cash_ite != Utility::_cfg_servers.end()) {
        return (cash_ite->second);
    }
    std::vector<Server const*> servers;
    for (size_t i = 0; i < cfg.http->server.size(); i++) {
        if (cfg.http->server[i]->listen == port && cfg.http->server[i]->server_name == host) {
            Utility::_cfg_servers.insert(make_pair(make_pair(port, host), cfg.http->server[i]));
            return (cfg.http->server[i]);
        }
    }

    for (size_t i = 0; i < cfg.http->server.size(); i++) {
        if (cfg.http->server[i]->listen == port && cfg.http->server[i]->is_default_server) {
            Utility::_cfg_servers.insert(make_pair(make_pair(port, host), cfg.http->server[i]));
            return (cfg.http->server[i]);
        }
    }
    for (size_t i = 0; i < cfg.http->server.size(); i++) {
        if (cfg.http->server[i]->listen == port) {
            Utility::_cfg_servers.insert(make_pair(make_pair(port, host), cfg.http->server[i]));
            return (cfg.http->server[i]);
        }
    }
    return (NULL);
}

std::vector<std::string> Utility::get_cfg_locations(const Config& cfg, string& port, string& host)
{
    std::map<pair<std::string, std::string>, std::vector<std::string> >::iterator cash_ite =
        Utility::_cfg_locations.find(make_pair(port, host));
    if (cash_ite != Utility::_cfg_locations.end()) {
        return (cash_ite->second);
    }
    Server const* servers = Utility::get_cfg_server(cfg, port, host);
    std::vector<std::string> locations;

    for (size_t j = 0; j < servers->location.size(); j++) {
        for (size_t k = 0; k < servers->location[j]->urls.size(); k++) {
            locations.push_back(servers->location[j]->urls[k]);
        }
    }
    Utility::_cfg_locations.insert(std::make_pair(std::make_pair(port, host), locations));
    return (locations);
}

std::map<std::string, std::vector<std::string> > Utility::get_cfg_locations_contents(const Config& cfg, string& port,
                                                                                     string& host, string& location)
{
    map<pair<pair<string, string>, string>, map<string, vector<string> > >::iterator cash_ite =
        Utility::_cfg_locations_content.find(make_pair(make_pair(port, host), location));
    if (cash_ite != Utility::_cfg_locations_content.end()) {
        return (cash_ite->second);
    }

    std::vector<map<string, std::vector<string> > > properties;
    Server const* servers = Utility::get_cfg_server(cfg, port, host);
    for (size_t j = 0; j < servers->location.size(); j++) {
        for (size_t k = 0; k < servers->location[j]->urls.size(); k++) {
            if (servers->location[j]->urls[k] == location) {
                Utility::_cfg_locations_content.insert(
                    make_pair(make_pair(make_pair(port, host), location), servers->location[j]->properties));

                return (servers->location[j]->properties);
            }
        }
    }
    std::map<std::string, std::vector<std::string> > rval;
    return (rval);
}

std::string Utility::delete_duplicated_slash(std::string str)
{
    string delimiter = "/";
    SplittedString sp(str, delimiter);
    std::string only_one_slash;
    if (str[0] == '/') {
        only_one_slash = delimiter;
    }
    for (size_t i = 0; i < sp.size(); i++) {
        only_one_slash += sp[i];
        only_one_slash += "/";
    }
    if (str[str.size() - 1] != '/') {
        only_one_slash.erase(only_one_slash.end() - 1, only_one_slash.end());
    }
    return (only_one_slash);
}

int Utility::hex_string_to_int(const std::string& hex_string)
{
    // 16進数の文字列をintに変換
    char* endPtr;
    long int result = strtol(hex_string.c_str(), &endPtr, 16);

    // 変換が成功したかどうかを確認
    if (*endPtr != '\0')
        throw std::invalid_argument("hex_string_to_int: invalid argument");

    return static_cast<int>(result);
}

std::map<std::pair<std::string, std::string>, Server const*> Utility::_cfg_servers;
std::map<std::pair<std::string, std::string>, std::vector<std::string> > Utility::_cfg_locations;
std::map<pair<std::pair<std::string, std::string>, std::string>, std::map<std::string, std::vector<std::string> > >
    Utility::_cfg_locations_content;

#ifdef UNIT_TEST
TEST_CASE("trim_white_space")
{
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  ") == "");
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  a") == "a");
    CHECK(Utility::trim_white_space("a  \t\n\v\f\r  ") == "a");
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  a  \t\n\v\f\r  ") == "a");
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  a  \t\n\v\f\r  b  \t\n\v\f\r  ") == "a  \t\n\v\f\r  b");
}
#endif
