#include "Server.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::exception;

string Server::get_server_name(BlockStatement const &server)
{
    try
    {
        Statement const *server_name_directive = server["server_name"];
        return server_name_directive->get_param(0);
    }
    catch (const exception &e)
    {
        return "";
    }
}

int Server::get_listen_port(BlockStatement const &server)
{
    try
    {
        Statement const *listen_directive = server["listen"];
        return my_stoi(listen_directive->get_param(0));
    }
    catch (const exception &e)
    {
        throw SyntaxError("listen directive not found or invalid");
    }
}

Server::Server(Statement const *server) : is_default_server(false)
{
    if (server == NULL)
        throw SyntaxError("Server: Taken directive is NULL");
    if (!dynamic_cast<BlockStatement const *>(server))
        throw SyntaxError("Server: Taken directive is not block statement");
    if (server->get_directive() != "server")
        throw SyntaxError("Server: Taken directive is not server directive");
    BlockStatement server_directive = *(dynamic_cast<BlockStatement const *>(server));

    listen = get_listen_port(server_directive);
    if (server_directive["listen"]->get_params().size() == 2 &&
        server_directive["listen"]->get_directive() == "default_server")
        is_default_server = true;
    server_name = get_server_name(server_directive);
    for (size_t i = 0; i < server_directive.get_children("location").size(); i++)
        location.push_back(new Location(server_directive.get_children("location")[i]));
}

Location const &Server::operator[](size_t index) const
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
    vector<Statement const *> server_directive = parser.get_root();
    Server server(server_directive[0]);
    CHECK(server.listen == 80);
    CHECK(server.server_name == "localhost");
}

TEST_CASE("Server: constructor")
{
    Parser parser("server { listen 80; server_name localhost; location / { root /; } }");
    vector<Statement const *> server_directive = parser.get_root();
    Server server(server_directive[0]);
    CHECK(server.listen == 80);
    CHECK(server.server_name == "localhost");
    CHECK(server.location.size() == 1);
    CHECK(server.location[0]->urls.size() == 1);
    CHECK(server.location[0]->urls[0] == "/");
}

/* ERROR CASE */

TEST_CASE("Server: taken not server directive")
{
    Parser parser("location / { root /; }");
    vector<Statement const *> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not block statement")
{
    Parser parser("server listen 80;");
    vector<Statement const *> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not listen directive")
{
    Parser parser("server { server_name localhost; }");
    vector<Statement const *> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}
#endif