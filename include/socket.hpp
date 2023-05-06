#ifndef SOCKET_H
#define SOCKET_H

#include "fd_manager.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
//
//

typedef struct clientinfo {
    int fd;
    char buf[1024];
    int n;
    int state;
} s_clientinfo;

class Socket
{
  public:
    Socket();
    Socket(std::string port);
    Socket(const Socket& socket);
    Socket& operator=(const Socket& socket);
    virtual ~Socket();
    virtual void communicate();
    void close_fd();
    int getSockFD();
    Request* recv(int fd);
    bool send(int fd);
    int accept_request();
    void set_response(int fd, Response* res);
    Response* get_response(int fd);
    void erase_request(int fd);
    void erase_response(int fd);
    std::vector<int> timeout(int time);
    void delete_fd_map(int fd);

  protected:
    int _sock_fd;
    const static int _SOCKET_NUM = 10;
    std::map<int, FDManager*> _fd_map;
    std::string _port;
    virtual void init();
    virtual int makeSocket();
    virtual void setAddrInfo(struct addrinfo& info);
    struct epoll_event _ev;
    s_clientinfo _clientinfo;
    bool send_err(std::string err);

    template <typename T>
    bool increment_timeout(T& obj, int time);
};

#include <iostream>
#include <stdio.h>
#include <string>

using std::cout;
using std::endl;
template <typename T>
bool Socket::increment_timeout(T& obj, int time)
{
    SocketData* socket_data = &obj;

    bool exceed;
    exceed = socket_data->increment_timeout(time);
    if (exceed) {
        return (true);
    }
    return (false);
}

#endif /* SOCKET_H */
