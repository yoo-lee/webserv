#ifndef HTTP_H
#define HTTP_H
#include "BlockStatement.hpp"
#include "Server.hpp"
#include "SyntaxError.hpp"
#include <string>
#include <vector>
class HTTP
{
  private:
    int get_client_max_body_size(BlockStatement &http_directive);

  public:
    int client_max_body_size;
    std::vector<Server *> server;
    HTTP();
    HTTP(Statement &directive);
    ~HTTP();
    Server const &operator[](size_t index);
    HTTP &operator=(HTTP const &other);
};

#endif /* HTTP_H */
