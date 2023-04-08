#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "request.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <string>
//#include <sys/types.h>
//#include <netinet/in.h>

class Socket
{
    public:
        Socket();
        //Socket(const Socket &socket);
        Socket(const Socket &socket);
        Socket& operator=(const Socket &socket);
        virtual ~Socket();
        virtual void communication();
        void close_fd();
        int getSockFD();
        Request *recv();
        bool send(std::string& data);
    protected:
        int sock_fd;
        int fd;
        std::string port;
        virtual void init();
        virtual int makeSocket();
        virtual void setAddrInfo(struct addrinfo &info);
        Request *req;
        //struct sockaddr_in hint;
};

#endif
