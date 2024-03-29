#include "Http.hpp"
#include "utils.hpp"
#include <exception>

using std::exception;

HTTP::HTTP() {}

int HTTP::get_client_max_body_size(BlockStatement const& http_directive)
{
    try {
        return my_stoi(http_directive["client_max_body_size"]->get_param(0));
    } catch (exception& e) {
        throw SyntaxError("HTTP: client_max_body_size not found");
    }
}

HTTP::HTTP(Statement const* directive)
{
    if (dynamic_cast<BlockStatement const*>(directive) == NULL)
        throw SyntaxError("HTTP: Taken directive is not a block statement.");
    BlockStatement const& http_directive = *(dynamic_cast<BlockStatement const*>(directive));
    if (http_directive.get_directive() != "http")
        throw SyntaxError("HTTP: Taken directive is not a http. Taken directive is " + http_directive.get_directive() +
                          ".");
    client_max_body_size = get_client_max_body_size(http_directive);
    vector<Statement const*> server_directives = http_directive.get_children("server");
    bool has_default_server = false;
    for (size_t i = 0; i < server_directives.size(); i++) {
        server.push_back(new Server(server_directives[i]));
        if (server[i]->is_default_server && has_default_server)
            throw SyntaxError("HTTP: There are more than one default server.");
        if (server[i]->is_default_server)
            has_default_server = true;
    }
}

HTTP::HTTP(HTTP const& h) : client_max_body_size(h.client_max_body_size)
{
    for (size_t i = 0; i < h.server.size(); i++)
        server.push_back(new Server(*h.server[i]));
}
HTTP::~HTTP()
{
    for (size_t i = 0; i < server.size(); i++)
        delete server[i];
}

Server const* HTTP::get_server(string server_name) const
{
    for (size_t i = 0; i < server.size(); i++)
        if (server[i]->server_name == server_name)
            return server[i];
    return NULL;
}

Server const& HTTP::operator[](size_t index)
{
    if (index < server.size())
        return *server[index];
    throw SyntaxError("directive not found");
}

HTTP& HTTP::operator=(HTTP const& other)
{
    if (this != &other) {
        client_max_body_size = other.client_max_body_size;
        for (size_t i = 0; i < other.server.size(); i++)
            server.push_back(new Server(*other.server[i]));
    }
    return *this;
}
