#ifndef SERVER_H
#define SERVER_H
#include "BlockStatement.hpp"
#include "Location.hpp"
#include "SyntaxError.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Server
{
  private:
    void set_server_name(BlockStatement const &server);
    void set_listen_port(BlockStatement const &server);

  public:
    std::string listen;
    bool is_default_server;
    string server_name;
    vector<Location *> location;
    Location const &operator[](size_t index) const;

    Server(Statement const *server);
    Server(Server const *server);
    ~Server();
};

#endif /* SERVER_H */
