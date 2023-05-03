#include "Server.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

std::string Server::get_server_name(BlockStatement const &server)
{
    try
    {
        Statement *server_name_directive = server["server_name"];
        return server_name_directive->get_param(0);
    }
    catch (const std::exception &e)
    {
        return "";
    }
}

int Server::get_listen_port(BlockStatement const &server)
{
    try
    {
        Statement *listen_directive = server["listen"];
        return myStoi(listen_directive->get_param(0));
    }
    catch (const std::exception &e)
    {
        throw SyntaxError("listen directive not found or invalid");
    }
}

Server::Server(Statement *server)
{
    if (server->get_directive() != "server")
        throw SyntaxError("Server: Taken directive is not server directive");
    if (!dynamic_cast<BlockStatement *>(server))
        throw SyntaxError("Server: Taken directive is not block statement");
    BlockStatement server_directive = *(dynamic_cast<BlockStatement *>(server));

    listen = get_listen_port(server_directive);
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
        delete &location[i];
    location.clear();
}

#ifdef UNIT_TEST
#include "AST.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Statement.hpp"
TEST_CASE("Server: constructor")
{
    Parser parser("server { listen 80; server_name localhost; }");
    std::vector<Statement *> server_directive = parser.get_root();
    Server server(server_directive[0]);
    CHECK(server.listen == 80);
    CHECK(server.server_name == "localhost");
}

/* ERROR CASE */

TEST_CASE("Server: taken not server directive")
{
    Parser parser("location / { root /; }");
    std::vector<Statement *> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not block statement")
{
    Parser parser("server listen 80;");
    std::vector<Statement *> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}

TEST_CASE("Server: taken not listen directive")
{
    Parser parser("server { server_name localhost; }");
    std::vector<Statement *> server_directive = parser.get_root();
    CHECK_THROWS_AS((Server(server_directive[0])), SyntaxError);
}
#endif