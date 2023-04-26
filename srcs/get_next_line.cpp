#include "get_next_line.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include "utility.hpp"

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

int GetNextLine::read(char *buf, int size)
{
    return recv(this->fd, buf, size, MSG_DONTWAIT);
}

void GetNextLine::read_line()
{
    int size = this->read(this->_buf, BUF_MAX);
    if (size < 0){
        cout << "recv error" << endl;
        return ;
    }
    this->buf_size = size;
        //if (this->buf_size > 0)
            //break;
        //else if (cnt > 10)
            //return ;
        //usleep(10);
        //cnt++;
    //}
    string str = string(this->_buf);
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


int GetNextLine::get_extra_buf(char *buf)
{
    ssize_t i = 0;
    for(size_t j=0; j < this->sp->size();j++){
        i += ((ssize_t)(*(this->sp))[j].size());
    }
    while(i < this->buf_size)
    {
        if (_buf[i+3] == 10 && _buf[i+2] == 13 && _buf[i+1] == 10 && _buf[i] == 13){
            int size = this->buf_size - (i+4);
            i +=4;
            Utility::memcpy(buf, &(_buf[i]), size);
            this->buf_size = 0;
            return (size);
        }
        i++;
    }
    return (0);
}

string &GetNextLine::getline()
{
    if (this->sp == NULL || this->sp->size() == this->pos+1){
        this->read_line();
    }
    if (this->sp == NULL){
        return (this->last_str);
    }
    if (this->sp->size() == this->pos+1){
        return (this->last_str);
    }
    return (*(this->sp))[this->pos++];
}

int GetNextLine::get_body(char *buf, size_t size)
{
    return (read(buf, size));
}
