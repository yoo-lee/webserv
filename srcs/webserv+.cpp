
#include "request.hpp"
#include "response.hpp"
#include "tcp_socket.hpp"
#include "webserv.hpp"

#include <algorithm>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;

Webserv::Webserv() : _epfd(0)
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
Webserv::Webserv(const Webserv& webserv) : _epfd(0)
{
    (void)webserv;
}
Webserv& Webserv::operator=(const Webserv& socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}

void Webserv::init_socket(std::vector<std::string> vec)
{
    try {
        for (size_t i = 0; i < vec.size(); i++) {
            Socket* sock = new Socket(vec[i]);
            this->_sockets.push_back(sock);
        }
    } catch (std::exception& e) {
        close_all();
        throw std::exception();
    }
}

void Webserv::close_all()
{
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        this->_sockets[i]->close_fd();
    }
}

bool Webserv::init_epoll()
{
    this->_epfd = epoll_create(this->_sockets.size());
    if (_epfd < 0) {
        cout << "Epoll Error:" << strerror(errno) << endl;
        return (false);
    }
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        Socket* socket = this->_sockets[i];

        ev.events = EPOLLIN;
        ev.data.fd = socket->getSockFD();
        if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, socket->getSockFD(), &ev) != 0) {
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            return (false);
        }
    }
    return (true);
}

Socket* Webserv::find_listen_socket(int socket_fd)
{
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        if (this->_sockets[i]->getSockFD() == socket_fd) {
            return (this->_sockets[i]);
        }
    }
    return (NULL);
}

void Webserv::connect_communication(int fd, struct epoll_event* event, Socket* socket)
{
    if (event->events & EPOLLIN) {
        Request* req = socket->recv();
        if (!req) {
            event->events = EPOLLOUT;
            if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, event) != 0) {
                cout << "error;connected_communication No.2" << endl;
            }
            return;
        }
        if (req->get_method() == NG) {
            cout << "error;connected_communication No.1" << endl;
            return;
        }

        // Test (will remove)
        req->print_request();
        // Body Test
        char buf[1024];
        int size = req->read_buf(buf);
        size_t file_size = 0;
        int cnt = 0;
        while (size > 0) {
            cnt++;
            req->add_loaded_body_size(size);
            file_size += size;
            // cout << "while No.1 body size:" << size << endl;
            // for(int i=0;i<size;i++){
            // cout << "body [" << i << "]:" << buf[i] << endl;
            //}
            // cout << buf << endl;
            size = req->read_buf(buf);
        }

        // todo. do something in server
        // CGI cgi(*req);
        bool read_all = true;
        if (read_all == false)
            return;

        // if(req->get_content_length() > req->get_loaded_body_size())
        // return ;
        event->events = EPOLLOUT;
        if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, event) != 0) {
            cout << "error;connected_communication No.2" << endl;
        }
    } else if (event->events & EPOLLOUT) {
        // std::string r_data = "HTTP/1.1 200 OK\r\ntext/plain;charset=UTF-8\r\nContent-Length:3\n\ntest5\r\n";
        std::string r_data = "HTTP/1.1 200 OK\n\
Date: Sun, 23 Apr 2023 13:14:41 GMT\n\
Server: Apache/2.4.52 (Ubuntu)\n\
Last-Modified: Sun, 23 Apr 2023 13:08:28 GMT\n\
ETag: \"5-5fa00961c5691\"\n\
Accept-Ranges: bytes\n\
Content-Type: text/html\n\
Content-Length: 4\n\
\n\
test\
";
        socket->send(r_data);

        // todo
        bool write_all = true;
        if (write_all == false)
            return;

        event->events = EPOLLIN;
        if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, event) != 0) {
            cout << "error;connected_communication No.3" << endl;
        }
        // close(fd);
    }
}

void Webserv::communicate()
{
    size_t size = this->_sockets.size();
    struct epoll_event sock_event[size];
    struct epoll_event server_event;

    memset(&(sock_event[0]), 0, sizeof(struct epoll_event) * size);
    memset(&server_event, 0, sizeof(struct epoll_event));

    std::vector<int> sock_fds;
    std::map<int, Socket*> map_socks;

    if (this->init_epoll() == false) {
        return;
    }
    while (1) {
        int nfds = epoll_wait(this->_epfd, sock_event, size, -1);
        if (nfds == 0) {
            continue;
        } else if (nfds < 0) {
            cout << "Epoll Wait Error:" << strerror(errno) << endl;
            return;
        }

        for (int i = 0; i < nfds; i++) {
            std::vector<int>::iterator tmp_fd = find(sock_fds.begin(), sock_fds.end(), sock_event[i].data.fd);
            if (tmp_fd != sock_fds.end()) {
                Socket* socket = map_socks.at(*tmp_fd);
                connect_communication(*tmp_fd, &(sock_event[i]), socket);
                continue;
            }
            Socket* socket = find_listen_socket(sock_event[i].data.fd);
            if (socket) {
                int fd = socket->accept_request();
                sock_fds.push_back(fd);
                memset(&server_event, 0, sizeof(server_event));
                server_event.events = EPOLLIN;
                server_event.data.fd = fd;
                map_socks.insert(std::make_pair(fd, socket));
                if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &server_event)) {
                    cout << "epoll_ctl error No.1" << endl;
                    continue;
                }
                // close(fd);
            }
        }
    }
}
