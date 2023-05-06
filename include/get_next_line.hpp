#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H
#include "split.hpp"
#include <string>
#include <unistd.h>
#include <vector>

class GetNextLine
{
  public:
    GetNextLine(int fd);
    ~GetNextLine();
    std::string& getline();
    int get_body(char* buf, size_t size);
    std::string last_str;
    size_t size();
    int get_extra_buf(char* buf);

  private:
    const static int BUF_MAX = 1600;
    char* _buf_body_pos;
    ssize_t _buf_body_size;
    char _buf[BUF_MAX];
    ssize_t buf_size;
    // ssize_t buf_pos;
    const int _fd;
    Split* _sp;
    // std::vector<std::string> vec;
    size_t _pos;
    void read_line();
    int read(char* buf, int size);
    // void read_line(char *buf);
};

#endif /* GET_NEXT_LINE_H */
