#ifndef GET_NEXT_LINE_HPP
#define GET_NEXT_LINE_HPP
#include <string>
#include <vector>
#include "split.hpp"

class GetNextLine{
    public:
        GetNextLine(int fd);
        ~GetNextLine();
        std::string &getline();
        std::string last_str = "";
        size_t size();
    private:
        const static int BUF_MAX = 1024;
        const int fd;
        Split *sp;
        //std::vector<std::string> vec;
        size_t pos;
        void readLine();
};

#endif
