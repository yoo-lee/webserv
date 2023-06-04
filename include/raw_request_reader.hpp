#ifndef RAW_REQUEST_READER_H
#define RAW_REQUEST_READER_H
#include "byte_vector.hpp"
#include "iread.hpp"
#include "splitted_string.hpp"
#include <string>
#include <unistd.h>
#include <vector>

class RawRequestReader
{
  public:
    RawRequestReader(int fd);
    RawRequestReader(int fd, IRead* iread);
    ~RawRequestReader();
    std::string& getline();
    ByteVector get_body(size_t size);
    std::string last_str;
    size_t size();
    ByteVector get_extra_buf();

  private:
    const static int BUF_MAX = 1600;
    char* _buf_body_pos;
    ssize_t _buf_body_size;
    char _buf[BUF_MAX];
    ssize_t _buf_size;
    const int _fd;
    SplittedString* _sp;
    size_t _pos;
    void read_line();
    int read(char* buf, int size);
    IRead* _iread;
};

#endif /* RAW_REQUEST_READER_H */
