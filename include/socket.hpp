#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "request.hpp"
//#include "response.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//
//

typedef struct clientinfo{
    int fd;
    char buf[1024];
    int n;
    int state;
}s_clientinfo;

class Socket
{
    public:
        Socket();
        Socket(std::string port);
        //Socket(const Socket &socket);
        Socket(const Socket &socket);
        Socket& operator=(const Socket &socket);
        virtual ~Socket();
        virtual void communication();
        void close_fd();
        int getSockFD();
        //Request *recv();
        Request *recv(int fd);
        bool send(int fd, std::string& data);
        int accept_request();
    protected:
        int sock_fd;
        //int fd;
        const static int _SOCKET_NUM = 10;
        //list<int> _fd_list;
        std::map <int, Request*> _req_map;
        std::string port;
        virtual void init();
        virtual int makeSocket();
        virtual void setAddrInfo(struct addrinfo &info);
        Request *req;
        //std::map <int, Response*> res_map;
        //Response *res;
        struct epoll_event ev;
        s_clientinfo clientinfo;
        bool send_err(std::string err);
        //struct sockaddr_in hint;
};

#endif
