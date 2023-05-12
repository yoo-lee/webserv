
#include "utility.hpp"
#include <iostream>
#include <sstream>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::cout;
using std::endl;
using std::string;
using std::make_pair;

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

std::vector<Server const *> Utility::get_cfg_server(const Config &cfg, string &port)
{
    std::vector<Server const*> servers;
    for(size_t i=0; i<cfg.http->server.size(); i++){
        if (cfg.http->server[i]->listen == port){
            servers.push_back(cfg.http->server[i]);
        }
    }
    return (servers);
}

std::vector<std::string> Utility::get_cfg_locations(const Config &cfg, string &port)
{
    std::map<std::string, std::vector<std::string> >::iterator cash_ite =  Utility::_cfg_locations.find(port);
    if (cash_ite != Utility::_cfg_locations.end()){
        return (cash_ite->second);
    }

    std::vector<Server const*> servers = Utility::get_cfg_server(cfg, port);
    (void)servers;
    std::vector<std::string> locations;

    for(size_t i=0; i< servers.size();i++){
        for(size_t j=0;j<servers[i]->location.size();j++){
            for(size_t k=0;k<servers[i]->location[j]->urls.size();k++){
                locations.push_back(servers[i]->location[j]->urls[k]);
            }
        }
    }
    Utility::_cfg_locations.insert(std::make_pair(port, locations));
    return (locations);
}

std::vector<std::map<std::string, std::vector<std::string > > > Utility::get_cfg_locations_contents(const Config &cfg, string &port, string &location)
{
    std::vector<map<string, std::vector<string> > > properties;
    std::vector<Server const*> servers = Utility::get_cfg_server(cfg, port);
    //std::map<std::string, std::map<std::string, std::map<std::string, std::string> > > port_location_contens;
    //std::map<std::string, <std::string> port_location;

    //std::map<std::string, <std::string> port_location;
    //std::vector<map<string, vector<string> > > properties;
    for(size_t i=0; i< servers.size();i++){
        for(size_t j=0;j<servers[i]->location.size();j++){
            for(size_t k=0;k<servers[i]->location[j]->urls.size();k++){
                if (servers[i]->location[j]->urls[k] == location){
                    properties.push_back(servers[i]->location[i]->properties);
                    Utility::_cfg_locations_content.insert(make_pair( make_pair(port, location) , servers[i]->location[i]->properties));
                    //break;
                }
            }
            //map<string, vector<string> >::iterator pro_ite = servers[i]->location[i]->properties.begin();
            //map<string, vector<string> >::iterator pro_end = servers[i]->location[i]->properties.end();
            //for(; pro_ite != pro_end ; pro_ite++){
                ////cout << "properties first:" << (*pro_ite).first << endl;
                //vector<string> tmp_vec = (*pro_ite).second;
                //for (size_t i=0; i<tmp_vec.size(); i++){
                    //extensions.push_back(tmp_vec[i]);
                    //cout << "properties second:" << tmp_vec[i] << endl;
                //}
            //}
        }
    }
    //std::vector<string> test_map;
    //std::vector<map<string, vector<string> > > properties2;
    //string test1 = "test1";
    //std::vector<string> test3;
    //test3.push_back(test1);
    //properties2.insert(make_pair(test1, test3));
    //Utility::_cfg_locations_contents.insert(make_pair(port, location), properties);
    Utility::_cfg_locations_contents.insert(make_pair(make_pair(port, location), properties));
    //
    //

    //std::map<std::string, std::map<std::string, std::vector<std::string> > > tmp1;
    //tmp1.insert(make_pair( test1, properties2));
    //std::map<pair<std::string, std::string> , std::map<std::string, std::vector<std::string> > > tmp1;
    //Utility::_cfg_locations_contents.insert(make_pair( make_pair(port, location) , properties2));
    return (properties);
}



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
