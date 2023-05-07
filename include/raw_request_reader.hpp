#ifndef RAW_REQUEST_READER_H
#define RAW_REQUEST_READER_H
#include "splitted_string.hpp"
#include <string>
#include <unistd.h>
#include <vector>

class RawRequestReader
{
  public:
    RawRequestReader(int fd);
    ~RawRequestReader();
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
    ssize_t _buf_size;
    // ssize_t buf_pos;
    const int _fd;
    SplittedString* _sp;
    // std::vector<std::string> vec;
    size_t _pos;
    void read_line();
    int read(char* buf, int size);
    // void read_line(char *buf);
};

#endif /* RAW_REQUEST_READER_H */
