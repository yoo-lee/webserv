#include "webserv.hpp"
#include "tcp_socket.hpp"
#include "request.hpp"
#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>

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
    cout << "find_listen_socket No.1 fd=" << socket_fd << endl;
    for (size_t i=0;i< this->sockets.size(); i++)
    {
        if (this->sockets[i]->getSockFD() == socket_fd){
            cout << "find_listen_socket No.2 fd=" << socket_fd << endl;
            return (this->sockets[i]);
        }
    }
    cout << "find_listen_socket No.3 fd=" << socket_fd << endl;
    return (NULL);
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
        cout << "epoll_wait No.1 nfds=" << endl;
        int nfds = epoll_wait(this->epfd, sock_event, size, -1);
        cout << "epoll_wait No.2 nfds=" << nfds << endl;
        if (nfds == 0) {
        cout << "epoll_wait No.3" << endl;
            continue;
        }
        else if (nfds < 0)
        {
            cout << "Epoll Wait Error:" << strerror(errno) << endl;
            return ;
        }
        cout << "epoll_wait No.4 nfds=" << endl;
        for (int i = 0; i < nfds; i++)
        {
            cout << "epoll_wait No.5, i=" << i << ", fd=" << sock_event[i].data.fd << endl;
            Socket *socket = find_listen_socket(sock_event[i].data.fd);
            if (socket)
            {
                int fd = socket->accept_request();
                cout << "epoll_wait No.6 recv fd=" << fd << endl;
                sock_fds.push_back(fd);
                memset(&server_event, 0, sizeof(server_event));
                server_event.events = EPOLLIN | EPOLLONESHOT;
                server_event.data.fd = fd;
                //server_event.data.ptr = malloc(sizeof(Socket));
                cout << "sizeof(ptr)" << sizeof(server_event.data.ptr) << endl;
                cout << "sizeof(socket)" << sizeof(socket) << endl;
                map_socks.insert(std::make_pair(fd, socket));
                if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &server_event))
                {
                    cout << "epoll_ctl error No.1" << endl;
                    continue;
                }
            }
            else
            {
                std::vector<int>::iterator tmp_fd = find(sock_fds.begin(), sock_fds.end(), sock_event[i].data.fd);
                cout << "sock_event[i].data.fd=" << sock_event[i].data.fd << endl;
                if (tmp_fd == sock_fds.end())
                {
                    cout << "end error" << endl;
                    continue;
                }
                socket = map_socks.at(*tmp_fd);
            cout << "epoll_wait No.7 EPOLLIN=" << EPOLLIN << ", events=" << sock_event[i].events << endl;
                if (sock_event[i].events & EPOLLIN){
                    cout << "epoll_wait No.8 tmp_fd=" << *tmp_fd << endl;
                    Request *req = socket->recv();

                    //Request *req = socket->recv();
            cout << "epoll_wait No.9" << endl;
                    if (req->get_method() == NG){
            cout << "epoll_wait No.10" << endl;
                        break;
                    }
                    //Request req(fd);
            cout << "epoll_wait No.11" << endl;
                    req->print_request();
            cout << "epoll_wait No.12" << endl;
                    char buf[1024]{0};
                    int size = req->read_buf(buf);
            cout << "epoll_wait No.13" << endl;
                    while(size > 0){
                        cout << "body test: size=" << size << endl << "body:" << string(buf) << endl;
                        size = req->read_buf(buf);
                    }
            cout << "epoll_wait No.13" << endl;

                    sock_event[i].events = EPOLLOUT;
            cout << "epoll_wait No.14" << endl;
                    if(epoll_ctl(this->epfd, EPOLL_CTL_MOD, *tmp_fd, &(sock_event[i])) != 0){
                        cout << "epoll_ctl No.2 error" << endl;
                    }
            cout << "epoll_wait No.15" << endl;

                }else if (sock_event[i].events & EPOLLOUT){
            cout << "epoll_wait No.16" << endl;
                    if(epoll_ctl(this->epfd, EPOLL_CTL_DEL, *tmp_fd, &(sock_event[i])) != 0){
                        cout << "epoll_ctl No.3 error" << endl;
                    }
            cout << "epoll_wait No.17" << endl;
                    std::string r_data = "HTTP/1.1 200 OK\n\ntest5";
                    //std::string r_data = "HTTP/1.1 204 No Content";
                    //cout <<"r_data=" << r_data << endl;
                    socket->send(r_data);
                    cout << "epoll_wait No.9" << endl;

                }
                //if (sock_event[i].fd == )

            }

            //cout << "epoll_wait No.6" << endl;

            //char buf[1024]{0};
            //int size = req->read_buf(buf);
            //while(size > 0){
                //cout << "body test: size=" << size << endl << "body:" << string(buf) << endl;
                ////size = req->read_buf(buf);
            //}
            //cout << "epoll_wait No.8" << endl;

            //todo  do something with data

            //test
            //string test = string(data);
            //cout << "test:" << endl << test << endl;

            //todo send something
            //socket->read_all();
            //delete data;
        }
    }
}
