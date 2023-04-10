#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <vector>
#include "tcp_socket.hpp"

class Webserv
{
    public:
        Webserv();
        Webserv(const std::vector<std::string> ports);
        //Webserv(size_t size);
        //Webserv(const Webserv &socket);
        Webserv(const Webserv &sockets);
        Webserv& operator=(const Webserv &sockets);
        ~Webserv();
        void communication();
    private:
        int epfd;
        void init_socket(std::vector<std::string>);
        bool init_epoll();
        void close_all();
        std::vector<Socket*> sockets;
        Socket* find_listen_socket(int socket_fd);
        //struct epoll_event ev;

};
#endif
