#ifndef SERVER_H
#define SERVER_H
#include "BlockStatement.hpp"
#include "Location.hpp"
#include "SyntaxError.hpp"
#include "utils.hpp"
#include <string>
#include <vector>
class Server
{
  private:
    static std::string get_server_name(BlockStatement const &server);
    static int get_listen_port(BlockStatement const &server);

  public:
    int listen;
    bool is_default_server;
    std::string server_name;
    std::vector<Location *> location;
    Location const &operator[](size_t index) const;

    Server(Statement *server);
    Server(Server *server);
    ~Server();
};

#endif /* SERVER_H */
