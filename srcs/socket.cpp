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

//bind
void Socket::init()
{
    this->_sock_fd= makeSocket();
    memset(&(this->_clientinfo), 0, sizeof(s_clientinfo));
    this->_ev.data.ptr = &_clientinfo;
    if (this->_sock_fd < 0)
    {
         cout << strerror(errno) << endl;
         throw std::runtime_error("Failed to create sock_fdet\n");
    }

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    setAddrInfo(hint);
    struct addrinfo *res = NULL;
    int err = getaddrinfo(NULL, this->_port.c_str(), &hint, &res);
    if (err != 0)
    {
        this->close_fd();
        cout << "Error getaddrinfo() :" << gai_strerror(err) << endl;
        throw std::runtime_error("Failed to init()\n");
    }
    int yes = 1;
    setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
    if (bind(this->_sock_fd, res->ai_addr, res->ai_addrlen) != 0)
    {
        this->close_fd();
        freeaddrinfo(res);
        cout << "Error bind1:" << strerror(errno) << endl;
        throw std::runtime_error("Failed to bind()\n");
    }
    freeaddrinfo(res);

    //// sysctl net.ipv4.tcp_max_syn_backlog
    //// NG in guacamole
    listen(this->_sock_fd, _SOCKET_NUM);
}
//bind
// バインド（bind）とは、ソケットにIPアドレスとポート番号を関連付けることです。
Socket::Socket() : _sock_fd(0), _port("11112")
{
    init();
}

Socket::Socket(std::string port_) : _sock_fd(0) ,_port(port_)
{
    init();
}

Socket::~Socket()
{
    //delete this->req;
}
Socket::Socket(const Socket &sock_fdet) : _sock_fd(sock_fdet._sock_fd), _port(sock_fdet._port)
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
    return (this->_sock_fd);
}

void Socket::close_fd()
{
    close(this->_sock_fd);
}


int Socket::accept_request()
{
    struct sockaddr_in client;
    memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(client);

    int fd = accept(this->_sock_fd,(struct sockaddr *)&client, &len);
    if (fd < 0)
    {
        cout << "Error accept():" << strerror(errno) << endl;
        //return ();
    }

    FDManager *fd_m = new FDManager(fd);
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    int cur_flags = fcntl(fd, F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, cur_flags);
    return (fd);
}

Request *Socket::recv(int fd)
{
    Request *req = this->_fd_map[fd]->get_req();
    if (req != NULL)
        return req;
    try{
        req = new Request(fd);
        this->_fd_map[fd]->insert(req);
    }catch(std::exception &e){
        req = NULL;
        this->_fd_map[fd]->insert(req);
        cout << e.what() << endl;
    }
    return (req);
}

bool Socket::send(int fd)
{

    Response *res = get_response(fd);
    char last = '\0';
    write(fd, res->getRes().c_str(), res->getRes().size());
    write(fd, &last, 1);
    return (true);
}

void Socket::set_response(int fd, Response *res)
{
    this->_fd_map[fd]->insert(res);
}

Response *Socket::get_response(int fd)
{
    return (this->_fd_map[fd]->get_res());
}

std::vector<int> Socket::timeout(int time)
{
    std::map<int, FDManager*>::iterator iter = _fd_map.begin();
    std::map<int, FDManager*>::iterator end = _fd_map.end();
    std::vector<int> delete_fd;

    FDManager *fd_m;
    int fd;
    bool timeout;
    for(; iter != end; iter++)
    {
        fd = iter->first;
        fd_m = iter->second;
        if (fd_m->get_req()){
            timeout = increment_timeout(*fd_m->get_req(), time);
        }else if (fd_m->get_res()){
            timeout = increment_timeout(*fd_m->get_res(), time);
        }else{
            timeout = increment_timeout(*fd_m, time);
        }
        if (timeout){
            delete_fd.push_back(fd);
        }
    }
    return (delete_fd);
    //increment_timeout(_fd_req_map, time);
    //increment_timeout(_fd_res_map, time);
}

void Socket::erase_request(int fd)
{
    this->_fd_map[fd]->delete_req();
}

void Socket::erase_response(int fd)
{
    //Response *tmp = _fd_map[fd].get_res();
    //delete tmp;
    this->_fd_map[fd]->delete_res();
}

void Socket::delete_fd_map(int fd)
{
    FDManager* fd_m = this->_fd_map[fd];
    fd_m->close_fd();
    delete fd_m;
    this->_fd_map.erase(fd);
}
