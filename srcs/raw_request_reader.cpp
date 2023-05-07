#include "raw_request_reader.hpp"
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
      _pos(0)
{
    this->read_line();
}

RawRequestReader::~RawRequestReader()
{
    delete _sp;
}

int RawRequestReader::read(char* buf, int size)
{
    return recv(this->_fd, buf, size, MSG_DONTWAIT);
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
        this->_sp = new Split(str, "\r\n");
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

int RawRequestReader::get_extra_buf(char* buf)
{
    ssize_t tmp = 0;
    if (this->_buf_body_size > 0) {
        Utility::memcpy(buf, this->_buf_body_pos, this->_buf_body_size);
        tmp = this->_buf_body_size;
        this->_buf_body_size = 0;
    }
    return (tmp);
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

int RawRequestReader::get_body(char* buf, size_t size)
{
    return (read(buf, size));
}
