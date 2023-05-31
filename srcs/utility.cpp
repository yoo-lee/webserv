
#include "utility.hpp"
#include "splitted_string.hpp"
#include <algorithm>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <istream>
#include <sstream>
#include <sys/stat.h>

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

bool orderby_string_len(string a, string b)
{
    return (a.size() >= b.size());
}

void Utility::sort_orderby_len(std::vector<std::string>& str)
{
    (sort(str.begin(), str.end(), orderby_string_len));
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

// 存在しないステータスコードを指定すると、空文字列を返す
string Utility::get_http_status_message(string status_code)
{
    map<string, string> status_codes;
    // 1xx
    status_codes.insert(make_pair("100", "Continue"));
    status_codes.insert(make_pair("101", "Switching Protocols"));
    status_codes.insert(make_pair("102", "Processing"));
    // 2xx
    status_codes.insert(make_pair("200", "OK"));
    status_codes.insert(make_pair("201", "Created"));
    status_codes.insert(make_pair("202", "Accepted"));
    status_codes.insert(make_pair("203", "Non-Authoritative Information"));
    status_codes.insert(make_pair("204", "No Content"));
    status_codes.insert(make_pair("205", "Reset Content"));
    status_codes.insert(make_pair("206", "Partial Content"));
    status_codes.insert(make_pair("207", "Multi-Status"));
    status_codes.insert(make_pair("208", "Already Reported"));
    status_codes.insert(make_pair("226", "IM Used"));
    // 3xx
    status_codes.insert(make_pair("300", "Multiple Choices"));
    status_codes.insert(make_pair("301", "Moved Permanently"));
    status_codes.insert(make_pair("302", "Found"));
    status_codes.insert(make_pair("303", "See Other"));
    status_codes.insert(make_pair("304", "Not Modified"));
    status_codes.insert(make_pair("305", "Use Proxy"));
    status_codes.insert(make_pair("307", "Temporary Redirect"));
    status_codes.insert(make_pair("308", "Permanent Redirect"));
    // 4xx
    status_codes.insert(make_pair("400", "Bad Request"));
    status_codes.insert(make_pair("401", "Unauthorized"));
    status_codes.insert(make_pair("402", "Payment Required"));
    status_codes.insert(make_pair("403", "Forbidden"));
    status_codes.insert(make_pair("404", "Not Found"));
    status_codes.insert(make_pair("405", "Method Not Allowed"));
    status_codes.insert(make_pair("406", "Not Acceptable"));
    status_codes.insert(make_pair("407", "Proxy Authentication Required"));
    status_codes.insert(make_pair("408", "Request Timeout"));
    status_codes.insert(make_pair("409", "Conflict"));
    status_codes.insert(make_pair("410", "Gone"));
    status_codes.insert(make_pair("411", "Length Required"));
    status_codes.insert(make_pair("412", "Precondition Failed"));
    status_codes.insert(make_pair("413", "Payload Too Large"));
    status_codes.insert(make_pair("414", "URI Too Long"));
    status_codes.insert(make_pair("415", "Unsupported Media Type"));
    status_codes.insert(make_pair("416", "Range Not Satisfiable"));
    status_codes.insert(make_pair("417", "Expectation Failed"));
    status_codes.insert(make_pair("418", "I'm a teapot"));
    status_codes.insert(make_pair("421", "Misdirected Request"));
    status_codes.insert(make_pair("422", "Unprocessable Entity"));
    status_codes.insert(make_pair("423", "Locked"));
    status_codes.insert(make_pair("424", "Failed Dependency"));
    status_codes.insert(make_pair("425", "Too Early"));
    status_codes.insert(make_pair("426", "Upgrade Required"));
    status_codes.insert(make_pair("428", "Precondition Required"));
    status_codes.insert(make_pair("429", "Too Many Requests"));
    status_codes.insert(make_pair("431", "Request Header Fields Too Large"));
    status_codes.insert(make_pair("451", "Unavailable For Legal Reasons"));
    // 5xx
    status_codes.insert(make_pair("500", "Internal Server Error"));
    status_codes.insert(make_pair("501", "Not Implemented"));
    status_codes.insert(make_pair("502", "Bad Gateway"));
    status_codes.insert(make_pair("503", "Service Unavailable"));
    status_codes.insert(make_pair("504", "Gateway Timeout"));
    status_codes.insert(make_pair("505", "HTTP Version Not Supported"));
    status_codes.insert(make_pair("506", "Variant Also Negotiates"));
    status_codes.insert(make_pair("507", "Insufficient Storage"));
    status_codes.insert(make_pair("508", "Loop Detected"));
    status_codes.insert(make_pair("510", "Not Extended"));
    status_codes.insert(make_pair("511", "Network Authentication Required"));
    return status_codes[status_code];
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
