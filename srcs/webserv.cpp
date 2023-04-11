#include "webserv.hpp"
#include "tcp_socket.hpp"
#include "request.hpp"
#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;

Webserv::Webserv() : epfd(0)
{
    std::vector<std::string> vec;
    vec.push_back("80");
    init_socket(vec);
}

Webserv::Webserv(const std::vector<std::string> ports)
{
    init_socket(ports);
}

Webserv::~Webserv()
{
    close_all();
}
Webserv::Webserv(const Webserv &webserv) : epfd(0)
{
}
Webserv& Webserv::operator=(const Webserv &socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}

void Webserv::init_socket(std::vector<std::string> vec)
{
    try{
        for (size_t i=0; i < vec.size(); i++)
        {
            Socket *sock = new Socket(vec[i]);
            this->sockets.push_back(sock);
        }
    }catch(std::exception &e) {
        close_all();
        throw std::exception();
    }
}

void Webserv::close_all()
{
    for(size_t i=0; i < this->sockets.size();i++){
        this->sockets[i]->close_fd();
    }
}

bool Webserv::init_epoll()
{
    this->epfd = epoll_create(this->sockets.size());
    if(epfd < 0){
        cout << "Epoll Error:" << strerror(errno) << endl;
        return (false);
    }
    for(size_t i=0; i < this->sockets.size();i++)
    {
        struct epoll_event ev{0};
        Socket *socket = this->sockets[i];

        ev.events = EPOLLIN;
        ev.data.fd = socket->getSockFD();
        //ev.data.ptr = malloc(sizeof(Socket));
        if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, socket->getSockFD(), &ev) != 0){
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            return (false);
        }
    }
    return (true);
}

Socket* Webserv::find_listen_socket(int socket_fd)
{
    for (size_t i=0;i< this->sockets.size(); i++)
    {
        if (this->sockets[i]->getSockFD() == socket_fd){
            return (this->sockets[i]);
        }
    }
    return (NULL);
}

void Webserv::connected_communication(int fd, struct epoll_event *event, Socket *socket)
{
    if (event->events & EPOLLIN){
        Request *req = socket->recv();
        if (req->get_method() == NG){
            cout << "error;connected_communication No.1" << endl;
            return ;
        }
        req->print_request();
        char buf[1024]{0};
        int size = req->read_buf(buf);
        cout << "body:" << endl;
        while(size > 0){
            cout << string(buf) << endl;
            size = req->read_buf(buf);
        }
        event->events = EPOLLOUT | EPOLLONESHOT;
        if(epoll_ctl(this->epfd, EPOLL_CTL_MOD, fd, event) != 0){
            cout << "error;connected_communication No.2" << endl;
        }
    }else if (event->events & EPOLLOUT){
        std::string r_data = "HTTP/1.1 200 OK\n\ntest5";
        socket->send(r_data);
        event->events = EPOLLIN | EPOLLONESHOT;
        if(epoll_ctl(this->epfd, EPOLL_CTL_DEL, fd, event) != 0){
            cout << "error;connected_communication No.3" << endl;
        }
        close(fd);
    }
}

void Webserv::communication()
{
    size_t size = this->sockets.size();
    struct epoll_event sock_event[size];
    struct epoll_event server_event = {0};
    std::vector<int> sock_fds;
    std::map<int, Socket*> map_socks;

    if(init_epoll() == false){
        return ;
    }
    while(1)
    {
        int nfds = epoll_wait(this->epfd, sock_event, size, -1);
        if (nfds == 0) {
            continue;
        }
        else if (nfds < 0)
        {
            cout << "Epoll Wait Error:" << strerror(errno) << endl;
            return ;
        }
        for (int i = 0; i < nfds; i++)
        {
            std::vector<int>::iterator tmp_fd = find(sock_fds.begin(), sock_fds.end(), sock_event[i].data.fd);
            if (tmp_fd != sock_fds.end())
            {
                Socket *socket = map_socks.at(*tmp_fd);
                connected_communication(*tmp_fd, &(sock_event[i]), socket);
                continue;

            }
            Socket *socket = find_listen_socket(sock_event[i].data.fd);
            if (socket)
            {
                int fd = socket->accept_request();
                sock_fds.push_back(fd);
                memset(&server_event, 0, sizeof(server_event));
                server_event.events = EPOLLIN | EPOLLONESHOT;
                server_event.data.fd = fd;
                map_socks.insert(std::make_pair(fd, socket));
                if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &server_event))
                {
                    cout << "epoll_ctl error No.1" << endl;
                    continue;
                }
                //close(fd);
            }
        }
    }
}
