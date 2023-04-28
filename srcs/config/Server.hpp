#ifndef SERVER_H
#define SERVER_H
#include "Location.hpp"
#include "SyntaxError.hpp"
#include <string>
#include <vector>
class Server
{
  private:
    /* data */
  public:
    int listen;
    bool is_default_server;
    std::string server_name;
    std::vector<Location> location;

    Server(BlockStatement* server_directive);
    Server(Server* server);
    ~Server();
};

Server::Server(BlockStatement* server_directive)
{
    if (server_directive["listen"]->get_params().size() > 2)
        throw SyntaxError("invalid listen directive");
    listen = std::stoi(server_directive["listen"]->get_params()[0]);
}

Server::~Server() {}

#endif /* SERVER_H */
