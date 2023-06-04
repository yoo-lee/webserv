#include "raw_request_reader.hpp"
#include "byte_vector.hpp"
#include "utility.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

RawRequestReader::RawRequestReader(int fd_)
    : _buf_body_pos(NULL),
      _buf_body_size(0),
      _buf_size(0),
      _fd(fd_),
      _sp(NULL),
      _pos(0),
      _iread(new ReadSocket())
{
    this->read_line();
}

RawRequestReader::RawRequestReader(int fd_, IRead *iread)
    : _buf_body_pos(NULL),
      _buf_body_size(0),
      _buf_size(0),
      _fd(fd_),
      _sp(NULL),
      _pos(0),
      _iread(iread)
{
    this->read_line();
}


RawRequestReader::~RawRequestReader()
{
    delete _sp;
    delete _iread;
}

int RawRequestReader::read(char* buf, int size)
{
    return _iread->iread(this->_fd, buf, size);
}

#include <stdio.h>
void RawRequestReader::read_line()
{
    int size = this->read(this->_buf, BUF_MAX);
    if (size < 0) {
        cout << "read_line() test No.0 escape loop() recv error" << endl;
        return;
    }
    this->_buf_size = size;

    this->_buf_body_pos = Utility::strnstr(this->_buf, "\r\n\r\n", size);
    if (this->_buf_body_pos) {
        *this->_buf_body_pos = '\0';
        this->_buf_body_pos += 4;
        this->_buf_body_size = size - (this->_buf_body_pos - this->_buf);
    }
    string str = string(this->_buf);
    if (_sp == NULL) {
        this->_sp = new SplittedString(str, "\r\n");
    } else {
        this->_sp->concat(str, "\r\n");
    }
}

size_t RawRequestReader::size()
{
    if (this->_sp == NULL)
        return (0);
    return (this->_sp->size());
}

// headerを読み込んだ際に余分に読み込んだbodyを返す
ByteVector RawRequestReader::get_extra_buf()
{
    ByteVector by(this->_buf_body_pos, this->_buf_body_size);
    this->_buf_body_size = 0;
    return (by);
}

string& RawRequestReader::getline()
{
    if (this->_sp == NULL || this->_sp->size() == this->_pos) {
        if (this->_buf_body_pos == NULL) {
            this->read_line();
        }
    }
    if (this->_sp == NULL) {
        return (this->last_str);
    }
    if (this->_sp->size() == this->_pos) {
        return (this->last_str);
    }
    return (*(this->_sp))[this->_pos++];
}

ByteVector RawRequestReader::get_body(size_t size)
{
    char buf[BUF_MAX];
    read(buf, size);
    return (ByteVector(buf, size));
}
