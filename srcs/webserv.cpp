
#include "webserv.hpp"
#include "tcp_socket.hpp"
#include "request.hpp"
#include "response.hpp"

#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <string>

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;

Webserv::Webserv() : epfd(0)
{
    init_socket();
}

Webserv::Webserv(const std::vector<std::string> ports)
{
    init_socket();
}


Webserv::~Webserv()
{
    close_all();
}
Webserv::Webserv(const Webserv &socket) : epfd(0)
{
    init_socket();
}
Webserv& Webserv::operator=(const Webserv &socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}

void Webserv::init_socket()
{
    try{
        Socket *sock = new Socket();
	    this->sockets.push_back(sock);
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
        if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, socket->getSockFD(), &ev) != 0){
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            return (false);
        }
    }
    return (true);
}

Socket* Webserv::find_socket(int socket_fd)
{
    for (size_t i=0;i< this->sockets.size(); i++)
    {
        if (this->sockets[i]->getSockFD() == socket_fd){
            return (this->sockets[i]);
        }
    }
    return (this->sockets[0]);
}

void Webserv::communication()
{
    //int epfd;
    size_t size = this->sockets.size();
    struct epoll_event event[size];

    if(init_epoll() == false){
        return ;
    }
    while(1)
    {
        int nfds = epoll_wait(this->epfd, event, size, -1);
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
            Socket *socket = find_socket(event[i].data.fd);
            Request *req = socket->recv();
            req->print_request();

            char buf[1024]{0};
            int size = req->read_buf(buf);
            while(size > 0)
            {
                cout << "body test: size=" << size << endl << "body:" << string(buf) << endl;
                size = req->read_buf(buf);
            }

            Response response(*req); // Create a Response object using the received Request
            response.buildResponse(); // Build the HTTP response

            // send response
            int res = send(event[i].data.fd, response.getRes().c_str(), response.getLen(), 0);
            if (res == -1)
			{
                cout << "Send Error:" << strerror(errno) << endl;
            }
        }
    }
}

