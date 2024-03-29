#ifndef WEBSERV_H
#define WEBSERV_H
#include "Config.hpp"
#include "tcp_socket.hpp"
#include <climits>
#include <set>
#include <vector>

using std::map;
using std::string;
using std::vector;

class Webserv
{
  public:
    Webserv();
    Webserv(const std::vector<std::string> ports);
    Webserv(Config& config);
    Webserv(const Webserv& sockets);
    Webserv& operator=(const Webserv& sockets);
    ~Webserv();
    void process_communication();
    bool change_epoll_config_to_write(int fd, int event);
    const Config& get_config();

  private:
    int _epfd;
    void init_socket(std::set<std::string>);
    bool init_epoll();
    void close_all();
    void process_connected_communication(int fd, struct epoll_event* event, Socket* socket);
    vector<Socket*> _sockets;
    Socket* find_listen_socket(int socket_fd);
    const static unsigned int BODY_MAX = INT_MAX;
    map<int, Socket*> _fd_sockets;
    void timeout(int time_sec);
    const Config& _config;

    string get_auto_index_page(string const& path) const;
};
#endif /* WEBSERV_H */
