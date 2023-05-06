#ifndef Utility_HPP
#define Utility_HPP
#include <sstream>
#include <string>
#include <unistd.h>

// template<typename T>
class Utility
{
  public:
    static char* memcpy(char* dst, const char* src, size_t n);
    static size_t strlen(const char* str);
    static std::string delete_space(std::string& str);
    static char* strnstr(const char* haystack, const char* needle, size_t len);

    template <typename T>
    static std::string to_string(T value);
    static std::string to_string2(int value);
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
