
#include "webserv.hpp"
#include "tcp_socket.hpp"
#include "request.hpp"
#include "response.hpp"

#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <errno.h>

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
    (void)webserv;
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
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        Socket *socket = this->sockets[i];

        ev.events = EPOLLIN;
        ev.data.fd = socket->getSockFD();
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
        Request *req = socket->recv(fd);
        if (!req){
            event->events = EPOLLOUT;
            if(epoll_ctl(this->epfd, EPOLL_CTL_MOD, fd, event) != 0){
                cout << "error;connected_communication No.2" << endl;
            }
            return;
        }
        if (req->get_method() == NG){
            cout << "error;connected_communication No.1" << endl;
            return ;
        }

        // Body Test
        //cout << "Body(only string):" << endl;
        char buf[1024];
        int size = req->read_buf(buf);
        size_t file_size= 0;
        int cnt = 0;
        while(size > 0){
            cnt++;
            req->add_loaded_body_size(size);
            file_size += size;
            //cout << "while No.1 body size:" << size << endl;
            //for(int i=0;i<size;i++){
                //cout << "body [" << i << "]:" << buf[i] << endl;
            //}
            //cout << buf << endl;
            size = req->read_buf(buf);
        }


        // Test (will remove)
        req->print_request();
        if (req->analyze() == false)
        {
            //make error Response;
            return;
        }

        //bool read_all = true;
        if (req->have_data_in_body() == false)
        {
            return ;
        }

        Response *res;
        if (req->is_cgi())
        {
            res = new Response(*req);
            //cig processing
        }
        else
        {
            res = new Response(*req);
            //server processing except cgi
        }
        socket->set_response(fd, res);

        //socket


        //cout << "req->get_content_length()=" << req->get_content_length() << endl;
        //cout << "req->get_loaded_body_size()=" << req->get_loaded_body_size() << endl;
        if(req->get_content_length() > req->get_loaded_body_size())
            return ;
        socket->erase_request(fd);
        event->events = EPOLLOUT;
        if(epoll_ctl(this->epfd, EPOLL_CTL_MOD, fd, event) != 0){
            cout << "error;connected_communication No.2" << endl;
        }
    }else if (event->events & EPOLLOUT){
        bool write_all = socket->send(fd);
        //todo
        if (write_all == false)
            return ;

        //sock_fds.erase(fd);
        this->_fd_sockets.erase(fd);
        if(epoll_ctl(this->epfd, EPOLL_CTL_DEL, fd, event) != 0){
            cout << "error;connected_communication No.3" << endl;
        }
        socket->erase_response(fd);
        //close(fd);
    }
}

void Webserv::communication()
{
    Socket *sock;
    size_t size = this->sockets.size();
    struct epoll_event sock_event[size];
    struct epoll_event server_event;

    memset(&(sock_event[0]), 0, sizeof(struct epoll_event) * size);
    memset(&server_event, 0, sizeof(struct epoll_event));

    //std::vector<int> sock_fds;
    //std::map<int, Socket*> this->_fd_sockets;

    if(this->init_epoll() == false){
        return ;
    }
    while(1)
    {
        std::vector<int> tmp(10);
        tmp[0] = 1;
        tmp[1] = 2;
        cout << tmp[0] << endl;
        int time_sec = -1;
        if (this->_fd_sockets.size() > 0){
            time_sec = 5;
        }
        time_sec = 5;
        int nfds = epoll_wait(this->epfd, sock_event, size, time_sec * 10);
        if (nfds == 0) {
            for(size_t i=0; i < this->sockets.size(); i++){
                sock = this->sockets[i];
                std::vector<int> delete_fd = sock->timeout(time_sec);
                cout << "timeout end No.1 delete fd size=" << delete_fd.size() << endl;
                for(size_t i=0; i < delete_fd.size(); i++){
                    int tmp_fd = delete_fd[i];
                    cout << "timeout end No.2 fd=" << tmp_fd << endl;
                    //Socket *sock = this->_fd_sockets[tmp_fd];
                    cout << "timeout end No.3" << endl;
                    sock->delete_fd_map(tmp_fd);
                    cout << "timeout end No.4" << endl;
                    this->_fd_sockets.erase(tmp_fd);
                    cout << "timeout end No.5" << endl;
                //cout << "timeout end No.3" << endl;
                }
                cout << "timeout end No.6" << endl;
            }
            continue;
        }
        else if (nfds < 0)
        {
            cout << "Epoll Wait Error:" << strerror(errno) << endl;
            return ;
        }
        for (int i = 0; i < nfds; i++)
        {
            std::map<int, Socket*>::iterator tmp_fd = this->_fd_sockets.find(sock_event[i].data.fd);
            if (tmp_fd != this->_fd_sockets.end())
            {
                Socket *socket = tmp_fd->second;
                connected_communication(tmp_fd->first, &(sock_event[i]), socket);
                continue;
            }
            Socket *socket = find_listen_socket(sock_event[i].data.fd);
            if (socket)
            {
                int fd = socket->accept_request();
                //sock_fds.push_back(fd);
                memset(&server_event, 0, sizeof(server_event));
                server_event.events = EPOLLIN;
                server_event.data.fd = fd;
                this->_fd_sockets.insert(std::make_pair(fd, socket));
                if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &server_event))
                {
                    continue;
                }
                //close(fd);
            }
        }
    }
}

