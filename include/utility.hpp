#ifndef UTILITY_H
#define UTILITY_H
#include "Config.hpp"
#include "Server.hpp"
#include "byte_vector.hpp"
#include "utility.hpp"
#include <sstream>
#include <string>
#include <unistd.h>

using std::map;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

class Utility
{
  public:
    static char* memcpy(char* dst, const char* src, size_t n);
    static size_t strlen(const char* str);
    static string delete_space(string& str);
    static char* strnstr(const char* haystack, const char* needle, size_t len);
    static string to_lower(string str);
    static string trim_white_space(string str);

    template <typename T>
    static string to_string(T value);
    static string to_string2(int value);

    static string delete_duplicated_slash(string str);
    static void sort_orderby_len(std::vector<std::string>& str);
    static int hex_string_to_int(const string& hex_string);

    static string get_http_status_message(string status_code);
    template <typename T>
    static void print_vector(vector<T> v);
};

template <typename T>
string Utility::to_string(T value)
{
    stringstream sss;
    string str;

    sss << value;
    str = sss.str();

    return (str);
}

template <typename T>
void Utility::print_vector(vector<T> v)
{
    for (size_t i = 0; i < v.size(); i++)
        if (v.size() - 1 != i)
            cout << v[i] << ",";
        else
            cout << v[i] << std::endl;
}

#endif /* UTILITY_H */
