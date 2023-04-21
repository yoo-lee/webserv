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
        bool change_epoll_config_to_write(int fd, int event);
    private:
        int epfd;
        void init_socket(std::vector<std::string>);
        bool init_epoll();
        void close_all();
        void connected_communication(int fd, struct epoll_event *event, Socket *socket);
        std::vector<Socket*> sockets;
        Socket* find_listen_socket(int socket_fd);
        //struct epoll_event ev;
};
#endif
