#ifndef HTTP_H
#define HTTP_H
#include "Server.hpp"
#include <string>
class HTTP
{
  private:
  public:
    std::string include;
    std::string default_type;
    const Server server;
    HTTP();
    HTTP(std::string include, std::string default_type, Server server);
    ~HTTP();
};

HTTP::HTTP()
{
}

HTTP::HTTP(std::string include, std::string default_type, Server server)
    : include(include), default_type(default_type), server(server)
{
}

HTTP::~HTTP()
{
}

#endif /* HTTP_H */
