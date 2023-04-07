#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <string>

class Split
{
    private:
        std::vector<std::string> vec;
    public:
        //Split();
        Split(int fd, std::string delimiter, std::string end);
        Split(std::string &str, std::string delimiter);
        //~Split();
        typedef typename std::vector<std::string>::iterator iterator;
        iterator begin();
        iterator end();
        size_t size();
        std::string& operator[](size_t i);
        static std::vector<std::string>* split(std::string &str, std::string delimiter);
};

#endif
