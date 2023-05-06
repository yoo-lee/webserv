
#include "webserv.hpp"

#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "request.hpp"
#include "response.hpp"
#include "tcp_socket.hpp"
#include "utility.hpp"

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;

/*
Webserv::Webserv() : epfd(0)
{
    std::vector<std::string> vec;
    vec.push_back("80");
    init_socket(vec);
    _config = Config();
}

Webserv::Webserv(const std::vector<std::string> ports)epfd(0)
{
    init_socket(ports);
}
*/

Webserv::Webserv(Config& config) : _epfd(0), _config(config)
{
    std::vector<std::string> ports;
    size_t server_cnt = config.http->server.size();
    for (size_t i = 0; i < server_cnt; i++) {
        ports.push_back(config.http->server[i]->listen);
    }
    init_socket(ports);
}

Webserv::~Webserv()
{
    close_all();
}
/*
Webserv::Webserv(const Webserv &webserv) : epfd(0)
{
    (void)webserv;
}
*/
Webserv& Webserv::operator=(const Webserv& socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}

const Config& Webserv::get_config()
{
    return (this->_config);
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
        Request* req = socket->recv(fd);
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

        // Body Test
        string buf;
        buf = req->get_body();
        cout << "Body: " << buf << endl;
        cout << "Body length: " << buf.length() << endl;

        // Test (will remove)
        req->print_request();
        if (req->analyze() == false) {
            // make error Response;
            return;
        }

        // bool read_all = true;
        if (req->have_data_in_body() == false) {
            return;
        }

        Response* res;
        if (req->is_cgi()) {
            res = new Response(*req);
            // cig processing
        } else {
            res = new Response(*req);
            // server processing except cgi
        }
        socket->set_response(fd, res);

        // socket

        if (req->get_content_length() > req->get_loaded_body_size()) {
            cout << "connected_communication not change OUT" << endl;
            return;
        }
        socket->erase_request(fd);
        event->events = EPOLLOUT;
        if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, event) != 0) {
            cout << strerror(errno) << endl;
        }
    } else if (event->events & EPOLLOUT) {
        bool write_all = socket->send(fd);
        // todo
        if (write_all == false)
            return;

        // sock_fds.erase(fd);
        this->_fd_sockets.erase(fd);
        if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, event) != 0) {
            cout << "connected_communication not change IN" << endl;
        }
        socket->erase_response(fd);
        // close(fd);
    }
}

void Webserv::timeout(int time_sec)
{
    Socket* sock;
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        sock = this->_sockets[i];
        std::vector<int> delete_fd = sock->timeout(time_sec);
        for (size_t i = 0; i < delete_fd.size(); i++) {
            int tmp_fd = delete_fd[i];
            sock->delete_fd_map(tmp_fd);
            this->_fd_sockets.erase(tmp_fd);
        }
    }
}

void Webserv::communicate()
{
    size_t size = this->_sockets.size();
    struct epoll_event sock_event[size];
    struct epoll_event server_event;

    memset(&(sock_event[0]), 0, sizeof(struct epoll_event) * size);
    memset(&server_event, 0, sizeof(struct epoll_event));

    if (this->init_epoll() == false) {
        return;
    }
    while (1) {
        int time_msec = -1;
        if (this->_fd_sockets.size() > 0) {
            time_msec = 5;
        }
        int nfds = epoll_wait(this->_epfd, sock_event, size, time_msec * 1000);
        if (nfds == 0) {
            this->timeout(time_msec);
            continue;
        } else if (nfds < 0) {
            cout << "Epoll Wait Error:" << strerror(errno) << endl;
            return;
        }
        for (int i = 0; i < nfds; i++) {
            std::map<int, Socket*>::iterator tmp_fd = this->_fd_sockets.find(sock_event[i].data.fd);
            if (tmp_fd != this->_fd_sockets.end()) {
                Socket* socket = tmp_fd->second;
                connect_communication(tmp_fd->first, &(sock_event[i]), socket);
                continue;
            }
            Socket* socket = find_listen_socket(sock_event[i].data.fd);
            if (socket) {
                int fd = socket->accept_request();
                memset(&server_event, 0, sizeof(server_event));
                server_event.events = EPOLLIN;
                server_event.data.fd = fd;
                this->_fd_sockets.insert(std::make_pair(fd, socket));
                if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &server_event)) {
                    continue;
                }
            }
        }
    }
}
