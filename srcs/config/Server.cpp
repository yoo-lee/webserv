#include "Server.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::exception;

void Server::set_server_name(BlockStatement const& server)
{
    try {
        Statement const* server_name_directive = server["server_name"];
        server_name = server_name_directive->get_param(0);
    } catch (const exception& e) {
        server_name = "";
    }
}

void Server::validate_listen() const
{
    if (listen.length() > 6)
        throw SyntaxError("listen port is invalid");
    int port = my_stoi(listen);

    if ((listen != "0" && port == 0) || port < 0 || port > 65535)
        throw SyntaxError("listen port is invalid");
}

void Server::set_listen_port(BlockStatement const& server)
{
    try {
        Statement const* listen_directive = server["listen"];
        if (listen_directive->get_params().size() != 2 && listen_directive->get_params().size() != 1)
            throw SyntaxError("listen directive has invalid number of parameters");
        try {
            if (listen_directive->get_param(1) == "default_server")
                is_default_server = true;
        } catch (const exception& e) {
        }
        listen = listen_directive->get_param(0);
        validate_listen();
    } catch (const exception& e) {
        throw SyntaxError("listen directive not found or invalid");
    }
}

Server::Server(Statement const* server) : is_default_server(false)
{
    if (server == NULL)
        throw SyntaxError("Server: Taken directive is NULL");
    if (!dynamic_cast<BlockStatement const*>(server))
        throw SyntaxError("Server: Taken directive is not block statement");
    if (server->get_directive() != "server")
        throw SyntaxError("Server: Taken directive is not server directive");
    BlockStatement server_directive = *(dynamic_cast<BlockStatement const*>(server));

    set_listen_port(server_directive);
    set_server_name(server_directive);
    for (size_t i = 0; i < server_directive.get_children("location").size(); i++)
        location.push_back(new Location(server_directive.get_children("location")[i]));
}

#include <iostream>
Server::Server(Server const& server)
    : listen(server.listen),
      is_default_server(server.is_default_server),
      server_name(server.server_name)
{
    for (size_t i = 0; i < server.location.size(); i++) {
        if (server.location[i] != NULL)
            this->location.push_back(new Location(*server.location[i]));
    }
}

Location const& Server::operator[](size_t index) const
{
    if (index < location.size())
        return *location[index];
    throw SyntaxError("directive not found");
}

Server::~Server()
{
    for (size_t i = 0; i < location.size(); i++)
        delete location[i];
}

#ifdef UNIT_TEST
#include "AST.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Statement.hpp"
TEST_CASE("Server: constructor")
{
    Parser parser("server { listen 80; server_name localhost; }");
    vector<Statement const*> server_directive = parser.get_root();
    Server server(server_directive[0]);
    CHECK(server.listen == "80");
    CHECK(server.server_name == "localhost");
}

TEST_CASE("Server: constructor")
{
    Parser parser("server { listen 80; server_name localhost; location / { root /; } }");
    vector<Statement const*> server_directive = parser.get_root();
    Server server(server_directive[0]);
    CHECK(server.listen == "80");
    CHECK(server.server_name == "localhost");
    CHECK(server.location.size() == 1);
    CHECK(server.location[0]->urls.size() == 1);
    CHECK(server.location[0]->urls[0] == "/");
}

TEST_CASE("Server: copy constructor")
{
    Parser parser("server { listen 80; server_name localhost; location / { root /; } }");
    vector<Statement const*> server_directive = parser.get_root();
    Server server_parent(server_directive[0]);
    Server server(server_parent);
    CHECK(server.listen == "80");
    CHECK(server.server_name == "localhost");
    CHECK(server.location.size() == 1);
    CHECK(server.location[0]->urls.size() == 1);
    CHECK(server.location[0]->urls[0] == "/");
}

/* ERROR CASE */

TEST_CASE("Server: listen port is not number")
{
    Parser parser("server { listen eight; server_name localhost; }");
    vector<Statement const*> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: listen port is too large")
{
    Parser parser("server { listen 10000000000; server_name localhost; }");
    vector<Statement const*> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: listen port is minus")
{
    Parser parser("server { listen -1; server_name localhost; }");
    vector<Statement const*> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not server directive")
{
    Parser parser("location / { root /; }");
    vector<Statement const*> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not block statement")
{
    Parser parser("server listen 80;");
    vector<Statement const*> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not listen directive")
{
    Parser parser("server { server_name localhost; }");
    vector<Statement const*> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}
#endif