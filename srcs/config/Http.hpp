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
  public:
    std::string client_max_body_size;
    std::vector<Server *> server;
    HTTP();
    HTTP(BlockStatement http_directive);
    ~HTTP();
};

HTTP::HTTP()
{
}

HTTP::HTTP(BlockStatement http_directive)
{
    if (http_directive.get_directive() != "http")
        throw SyntaxError("invalid http directive");
    if (http_directive["client_max_body_size"]->get_params().size() > 1)
        throw SyntaxError("invalid client_max_body_size directive");
    client_max_body_size = http_directive["client_max_body_size"]->get_params()[0];
    std::vector<Statement *> server_directives = http_directive.get_child_statements("server");
    for (size_t i = 0; i < server_directives.size(); i++)
    {
        server.push_back(new Server(*server_directives[i]));
    }
}

HTTP::~HTTP()
{
}

#endif /* HTTP_H */
