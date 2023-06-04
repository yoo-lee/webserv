#include "iread.hpp"
#include <unistd.h>

IRead::IRead()
{
}
IRead::~IRead()
{
}

ReadNormal::ReadNormal()
{
}

ReadNormal::~ReadNormal()
{
}

int ReadNormal::iread(int fd, char* buf, size_t size)
{
    return (read(fd, buf, size));
}

ReadSocket::ReadSocket()
{
}
ReadSocket::~ReadSocket()
{
}

int ReadSocket::iread(int fd, char* buf, size_t size)
{
    return (read(fd, buf, size));
}
