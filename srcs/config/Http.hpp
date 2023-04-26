#ifndef HTTP_H
#define HTTP_H
#include "BlockStatement.hpp"
#include "Server.hpp"
#include <string>
#include <vector>
class HTTP
{
  private:
  public:
    std::string include;
    std::string default_type;
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
        throw std::runtime_error("invalid http directive");
}

HTTP::~HTTP()
{
}

#endif /* HTTP_H */
