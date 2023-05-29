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
    static int hex_string_to_int(const string& hex_string);

    static bool is_file_exist(const string& path);
    static bool is_directory_exist(const string& path);
    static string get_cwd();
    static string read_file_text(const string& path);
    static ByteVector read_file_binary(const string& path);
    static bool delete_file(const string& path);
    static vector<string> get_entries_in_directory(const string & path);

    static string get_http_status_message(string status_code);
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

#endif /* UTILITY_H */
