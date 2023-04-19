#ifndef CONFIG_H
#define CONFIG_H
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "Lexer.hpp"

class Location {
   private:
   public:
    std::vector<std::string> urls;
    std::map<std::string, std::string> properties;
    Location();
    Location(std::vector<std::string> url,
             std::map<std::string, std::string> properties);
    Location(Location* location);
    ~Location();
};

Location::Location() {}

Location::Location(std::vector<std::string> urls,
                   std::map<std::string, std::string> properties)
    : urls(urls), properties(properties) {}

Location::Location(Location* location)
    : urls(location->urls), properties(location->properties) {}

Location::~Location() {}

class Server {
   private:
    /* data */
   public:
    const int listen;
    const std::string server_name;
    const std::vector<Location> location;
    const std::vector<std::string> error_page;

    Server();
    Server(int listen,
           std::string server_name,
           std::vector<Location> location,
           std::vector<std::string> error_page);
    Server(Server* server);
    ~Server();
};

Server::Server() : listen(0) {}

Server::Server(int listen,
               std::string server_name,
               std::vector<Location> location,
               std::vector<std::string> error_page)
    : listen(listen),
      server_name(server_name),
      location(location),
      error_page(error_page) {}

Server::Server(Server* server)
    : listen(server->listen),
      server_name(server->server_name),
      location(server->location),
      error_page(server->error_page) {}

Server::~Server() {}

class HTTP {
   private:
   public:
    std::string include;
    std::string default_type;
    const Server server;
    HTTP();
    HTTP(std::string include, std::string default_type, Server server);
    ~HTTP();
};

HTTP::HTTP() {}

HTTP::HTTP(std::string include, std::string default_type, Server server)
    : include(include), default_type(default_type), server(server) {}

HTTP::~HTTP() {}
class Config {
   private:
    std::string _file_text;

   public:
    HTTP http;
    Lexer lexer;
    Config(std::string file_path);
    ~Config(){};
};

Config::Config(std::string file_text)
    : _file_text(file_text), lexer((_file_text)) {}

#ifdef TEST
#include <fstream>
int main() {
    // test.nginx.confから読み取ってconfigに渡す
    std::ifstream file("sample.nginx.conf");
    std::string str;
    char c;
    while (file.get(c)) {
        str += c;
    }
    file.close();
    Config config("}");
    std::vector<Token*> token_list = config.lexer.getTokenList();
    for (std::size_t i = 0; i < token_list.size(); i++) {
        std::cout << *token_list[i] << std::endl;
    }
    return 0;
}

#endif

#endif /* CONFIG_H */
