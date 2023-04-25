#include "get_next_line.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

GetNextLine::GetNextLine(int fd_) : fd(fd_), sp(NULL), pos(0)
{
    this->read_line(NULL, BUF_MAX);
}

GetNextLine::~GetNextLine()
{
    delete sp;
}

void GetNextLine::read_line(char *buf, size_t size)
{
    //int cnt = 0;
    //if (!buf)
        //buf = this->_buf;
    //while (1){
    this->buf_size = recv(this->fd, buf, size, MSG_DONTWAIT);
    if (this->buf_size < 0){
        cout << "recv error" << endl;
        return ;
    }
        //if (this->buf_size > 0)
            //break;
        //else if (cnt > 10)
            //return ;
        //usleep(10);
        //cnt++;
    //}
    string str = string(buf);
    if (sp == NULL){
        this->sp = new Split(str, "\r\n");
    }else{
        this->sp->concat(str, "\r\n");
    }
}

size_t GetNextLine::size()
{
    if (this->sp == NULL)
        return (0);
    return (this->sp->size());
}


int GetNextLine::get_extra_buf(char **buf)
{
    char *_buf = *buf;
    ssize_t i = 0;
    while(i < this->buf_size)
    {
        if (_buf[i+3] == 10 && _buf[i+2] == 13 && _buf[i+1] == 10 && _buf[i] == 13){
            int size = this->buf_size - (i+4);
            i +=4;
            *buf = &(_buf[i]);
            return (size);
        }
        i++;
    }
    *buf = NULL;
    return (0);
}

string &GetNextLine::getline(char *buff, size_t size)
{
    if (this->sp == NULL || this->sp->size() == pos+1) {
        this->read_line(buff, size);
    }
    if (this->sp == NULL){
        return (this->last_str);
    }
    if (this->sp->size() == pos+1) {
        return (this->last_str);
    }
    return (*(this->sp))[pos++];
}
