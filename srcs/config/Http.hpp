#ifndef HTTP_H
#define HTTP_H
#include "BlockStatement.hpp"
#include "Server.hpp"
#include "SyntaxError.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class HTTP
{
  private:
    int get_client_max_body_size(BlockStatement const& http_directive);

  public:
    int client_max_body_size;
    vector<Server const*> server;
    HTTP();
    HTTP(Statement const* directive);
    HTTP(HTTP const& h);
    ~HTTP();
    Server const* get_server(string server_name) const;
    Server const& operator[](size_t index);
    HTTP& operator=(HTTP const& other);
};

#endif /* HTTP_H */
