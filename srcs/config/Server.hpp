#ifndef SERVER_H
#define SERVER_H
#include "Location.hpp"
#include <string>
#include <vector>

class Server
{
  private:
    /* data */
  public:
    const int listen;
    const std::string server_name;
    const std::vector<Location> location;
    const std::vector<std::string> error_page;

    Server();
    Server(int listen, std::string server_name, std::vector<Location> location, std::vector<std::string> error_page);
    Server(Server *server);
    ~Server();
};

Server::Server() : listen(0)
{
}

Server::Server(int listen, std::string server_name, std::vector<Location> location, std::vector<std::string> error_page)
    : listen(listen), server_name(server_name), location(location), error_page(error_page)
{
}

Server::Server(Server *server)
    : listen(server->listen), server_name(server->server_name), location(server->location),
      error_page(server->error_page)
{
}

Server::~Server()
{
}

#endif /* SERVER_H */
