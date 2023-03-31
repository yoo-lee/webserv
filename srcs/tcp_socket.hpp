#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include "socket.hpp"
//#include <sys/types.h>
//#include <netinet/in.h>

class TCPSocket : public Socket
{
    public:
        TCPSocket();
        //TCPSocket(const TCPSocket &socket);
        TCPSocket(const TCPSocket &socket);
        TCPSocket& operator=(const TCPSocket &socket);
        ~TCPSocket();
        void communication();
        //void close_fd();
        //int getSockFD();
    private:
        //int sock_fd;
        //std::string port;
        void init();
        //struct sockaddr_in hint;
};

#endif
