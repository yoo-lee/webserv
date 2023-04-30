//#include "sock_fdet.hpp"
#include <string.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include "tcp_socket.hpp"
#include "string.h"
#include <unistd.h>
#include <fcntl.h>
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
    //this->clientinfo;
    memset(&(this->clientinfo), 0, sizeof(s_clientinfo));
    this->ev.data.ptr = &clientinfo;
    if (this->sock_fd < 0)
    {
         cout << strerror(errno) << endl;
         //throw std::exception();
         throw std::runtime_error("Failed to create sock_fdet\n");
    }

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
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

Socket::Socket(std::string port_) : sock_fd(0) ,port(port_)
{
    init();
}

Socket::~Socket()
{
    delete this->req;
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


int Socket::accept_request()
{
    struct sockaddr_in client;
    memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(client);

    this->fd = accept(this->sock_fd,(struct sockaddr *)&client, &len);
    if (this->fd < 0)
    {
        cout << "Error accept():" << strerror(errno) << endl;
        //return ();
    }
    int cur_flags = fcntl(fd, F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, cur_flags);
    return (this->fd);
}

/*
Request *Socket::recv(int fd)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    cout << "socket recv No.1 fd=" << this->sock_fd << endl;
    this->fd= accept(fd,(struct sockaddr *)&client, &len);
    cout << "socket recv No.2 fd=" << this->sock_fd << endl;
    if (this->fd < 0)
    {
        cout << "Error accept():" << strerror(errno) << endl;
        return (NULL);
    }
    if (this->req != NULL)
        delete this->req;
    this->req = new Request(this->fd);
    return (this->req);
}
*/

Request *Socket::recv()
{
    if (this->req != NULL)
        delete this->req;
    try{
        this->req = new Request(this->fd);
    }catch(std::exception &e){
        this->req = NULL;
        cout << e.what() << endl; 
        //std::string r_data = "HTTP/1.1 400 NG\n";
        //this->send(r_data);
        //return (NULL);
    }
    return (this->req);
}

/*
bool Socket::send_err(std::string& data)
{

    char last = '\0';
    write(this->fd, data.c_str(), data.size());
    write(this->fd, &last, 1);
    return (true);
}
*/



bool Socket::send(std::string& data)
{

    char last = '\0';
    write(this->fd, data.c_str(), data.size());
    write(this->fd, &last, 1);
    return (true);
}
