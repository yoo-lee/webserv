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
    this->readLine();
}

GetNextLine::~GetNextLine()
{
    delete sp;
}

void GetNextLine::readLine()
{
    char buf[BUF_MAX];

    int cnt = 0;
    while (1){
        ssize_t rval = recv(this->fd, buf, BUF_MAX, MSG_DONTWAIT);
        if (rval > 0)
            break;
        else if (cnt > 10)
            return ;
        usleep(10);
        cnt++;
    }
    string str = string(buf);
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

string &GetNextLine::getline()
{
    if (this->sp == NULL || this->sp->size() == pos+1) {
        this->readLine();
    }
    if (this->sp == NULL)
        return (this->last_str);
    if (this->sp->size() == pos+1) {
        return (this->last_str);
    }
    return (*(this->sp))[pos++];
}
