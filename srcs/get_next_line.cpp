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
    this->read_line();
}

GetNextLine::~GetNextLine()
{
    delete sp;
}

void GetNextLine::read_line()
{
    int cnt = 0;
    while (1){
        this->buf_size = recv(this->fd, this->buf, BUF_MAX, MSG_DONTWAIT);
        if (this->buf_size > 0)
            break;
        else if (cnt > 10)
            return ;
        usleep(10);
        cnt++;
    }
    string str = string(this->buf);
    if (sp == NULL)
        this->sp = new Split(str, "\n");
    else
        this->sp->concat(str, "\n");
}

size_t GetNextLine::size()
{
    if (this->sp == NULL)
        return (0);
    return (this->sp->size());
}


int GetNextLine::get_extra_buf(char *cp_buf)
{
    ssize_t i = 0;
    while(i < this->buf_size)
    {
        if (buf[i+3] == 10 && buf[i+2] == 13 && buf[i+1] == 10 && buf[i] == 13){
            int size = this->buf_size - (i+4);
            i +=4;
            ssize_t j = 0;
            while(i < this->buf_size){
                cp_buf[j++] = buf[i++];
            }
            return (size);
        }
        i++;
    }
    cp_buf = NULL;
    return (0);
}

string &GetNextLine::getline()
{
    if (this->sp == NULL || this->sp->size() == pos+1) {
        this->read_line();
    }
    if (this->sp == NULL)
        return (this->last_str);
    if (this->sp->size() == pos+1) {
        return (this->last_str);
    }
    return (*(this->sp))[pos++];
}
