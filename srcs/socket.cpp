//#include "sock_fdet.hpp"
#include <string.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include "tcp_socket.hpp"
//#include <netdb.h>
//#include <sys/types.h>

using std::cout;
using std::endl;

int Socket::makeSocket()
{
    return (socket(AF_INET, SOCK_STREAM, 0));
}

void Socket::setAddrInfo(struct addrinfo &info)
{
    info.ai_family = AF_INET;
    info.ai_flags = AI_PASSIVE;
    info.ai_socktype = SOCK_STREAM;
}

void Socket::init()
{
    this->sock_fd= makeSocket();
    if (this->sock_fd < 0)
    {
         cout << strerror(errno) << endl;
         //throw std::exception();
         throw std::runtime_error("Failed to create sock_fdet\n");
    }

    struct addrinfo hint{0};
    setAddrInfo(hint);
    struct addrinfo *res = NULL;
    int err = getaddrinfo(NULL, this->port.c_str(), &hint, &res);
    if (err != 0)
    {
        this->close_fd();
        cout << "Error getaddrinfo() :" << gai_strerror(err) << endl;
        throw std::runtime_error("Failed to init()\n");
    }
    int yes = 1;
    setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
    if (bind(this->sock_fd, res->ai_addr, res->ai_addrlen) != 0)
    {
        this->close_fd();
        freeaddrinfo(res);
        cout << "Error bind1:" << strerror(errno) << endl;
        throw std::runtime_error("Failed to bind()\n");
    }
    freeaddrinfo(res);

    //// sysctl net.ipv4.tcp_max_syn_backlog
    //// NG in guacamole
    listen(this->sock_fd, 16);
}

Socket::Socket() : sock_fd(0), port("11111")
{
    init();
}
Socket::~Socket()
{
}
Socket::Socket(const Socket &sock_fdet) : sock_fd(sock_fdet.sock_fd), port(sock_fdet.port)
{
    init();
}
Socket& Socket::operator=(const Socket &sock_fdet)
{
    if (&sock_fdet == this)
        return (*this);
    return (*this);
}

void Socket::communication()
{
}

int Socket::getSockFD()
{
    return (this->sock_fd);
}

void Socket::close_fd()
{
    close(this->sock_fd);
}

char *Socket::recv()
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    const int BUF_MAX = 4096;
    char *buf;
    ssize_t read_size;

    int read_fd = accept(this->sock_fd,(struct sockaddr *)&client, &len);
    if (read_fd < 0)
    {
        cout << "Error accept():" << strerror(errno) << endl;
        return (NULL);
    }
    buf = new char[BUF_MAX];
    read_size = read(read_fd, buf, BUF_MAX);
    while(read_size == BUF_MAX)
    {
        //todo
        break;
    }

    return (buf);
}
