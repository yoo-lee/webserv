#ifndef GET_NEXT_LINE_HPP
#define GET_NEXT_LINE_HPP
#include <string>
#include <unistd.h>
#include <vector>
#include "split.hpp"

class GetNextLine{
    public:
        GetNextLine(int fd);
        ~GetNextLine();
        std::string &getline();
        std::string last_str;
        size_t size();
        int get_extra_buf(char *buf);
    private:
        const static int BUF_MAX = 1024;
        char buf[BUF_MAX];
        ssize_t buf_size;
        const int fd;
        Split *sp;
        //std::vector<std::string> vec;
        size_t pos;
        void read_line();
};

#endif
