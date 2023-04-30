#ifndef Utility_HPP
#define Utility_HPP
#include <unistd.h>
#include <string>

class Utility
{
    public:
        static char *memcpy(char *dst, const char *src, size_t n);
        static size_t strlen(const char *str);
        static std::string delete_space(std::string& str);
        static char *strnstr(const char *haystack, const char *needle, size_t len);
};

#endif
