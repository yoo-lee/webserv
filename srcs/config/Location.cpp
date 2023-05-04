#include "Location.hpp"
#include "BlockStatement.hpp"
#include "LimitExcept.hpp"
#include "SyntaxError.hpp"
#include <iostream>
#ifdef UNIT_TEST
#include "doctest.h"
#endif

Location::Location() {}

LimitExcept *get_limit_except(std::vector<Statement *> statements)
{
    for (size_t i = 0; i < statements.size(); i++)
    {
        if (statements[i]->get_directive() == "limit_except")
            return new LimitExcept(statements[i]);
    }
    return NULL;
}

std::string get_index(std::vector<Statement *> statements)
{
    for (size_t i = 0; i < statements.size(); i++)
    {
        if (statements[i]->get_directive() == "index")
            return statements[i]->get_params()[0];
    }
    return "index.html";
}

bool get_autoindex(std::vector<Statement *> statements)
{
    for (size_t i = 0; i < statements.size(); i++)
    {
        if (statements[i]->get_directive() == "autoindex")
        {
            if (statements[i]->get_params()[0] == "on")
                return true;
            else if (statements[i]->get_params()[0] == "off")
                return false;
            else
                throw SyntaxError("Location: Invalid autoindex value");
        }
    }
    return false;
}

std::map<std::string, std::string> get_error_pages(std::vector<Statement *> statements)
{
    std::map<std::string, std::string> error_page;
    for (size_t i = 0; i < statements.size(); i++)
    {
        if (statements[i]->get_directive() == "error_page")
        {
            std::vector<std::string> params = statements[i]->get_params();
            if (params.size() != 2)
                throw SyntaxError("Location: Invalid error_page directive");
            std::string page = params[params.size() - 1];
            for (size_t j = 0; j < params.size() - 1; j++)
                error_page[params[j]] = page;
        }
    }
    return error_page;
}

Location::Location(Statement *directive) : limit_except(0)
{
    if (directive == NULL)
        throw SyntaxError("Location: Taken directive is NULL");
    if (!dynamic_cast<BlockStatement *>(directive))
        throw SyntaxError("Location: Taken directive is not block statement");
    if (directive->get_directive() != "location")
        throw SyntaxError("Location: Taken directive is not location directive");

    BlockStatement *location_directive = dynamic_cast<BlockStatement *>(directive);

    urls = location_directive->get_params();

    std::vector<Statement *> location_statements = location_directive->get_children();

    limit_except = get_limit_except(location_statements);
    index = get_index(location_statements);
    autoindex = get_autoindex(location_statements);
    error_page = get_error_pages(location_statements);

    for (size_t i = 0; i < location_statements.size(); i++)
    {
        if (location_statements[i]->get_directive() != "limit_except" &&
            location_statements[i]->get_directive() != "index" &&
            location_statements[i]->get_directive() != "autoindex" &&
            location_statements[i]->get_directive() != "error_page")
        {
            if (dynamic_cast<BlockStatement *>(location_statements[i]))
                throw SyntaxError("Location: Detected Non limit_except BlockStatement. Only limit_except is allowed "
                                  "for block statements in the Location directive.");
            properties[location_statements[i]->get_directive()] = location_statements[i]->get_params();
        }
    }
}

Location::Location(Location *location)
    : urls(location->urls),
      properties(location->properties),
      limit_except(NULL),
      index(location->index),
      autoindex(location->autoindex),
      error_page(location->error_page)
{
    if (this->limit_except != NULL)
        limit_except = new LimitExcept(location->limit_except);
}

Location::~Location()
{
    delete limit_except;
}

std::vector<std::string> Location::operator[](std::string index)
{
    return properties[index];
}

#ifdef UNIT_TEST
// #include "SimpleStatement.hpp"
// TEST_CASE("Location: constructor")
// {
//     std::vector<std::string> params;
//     params.push_back("/hello");
//     params.push_back("/world");
//     std::cout << "x" << std::endl;

//     std::vector<Statement *> statements;
//     statements.push_back(new SimpleStatement("root", "/"));
//     statements.push_back(new SimpleStatement("index", "index.html"));
//     BlockStatement *location_directive = new BlockStatement("location", params, statements);
//     Location location(location_directive);
//     CHECK(location.urls[0] == "/hello");
//     CHECK(location.urls[1] == "/world");
//     CHECK(location.properties["root"].size() == 1);
//     CHECK(location.properties["root"][0] == "/");
//     CHECK(location.index == "index.html");
//     delete location_directive;
// }

// TEST_CASE("Location: copy constructor")
// {
//     std::vector<std::string> params;
//     params.push_back("/hello");
//     params.push_back("/world");

//     std::vector<Statement *> statements;
//     statements.push_back(new SimpleStatement("root", "/"));
//     statements.push_back(new SimpleStatement("index", "index.html"));
//     BlockStatement *location_directive = new BlockStatement("location", params, statements);
//     Location *location = new Location(location_directive);
//     Location location2(location);
//     std::cout << "user2" << std::endl;
//     CHECK(location2.urls[0] == "/hello");
//     CHECK(location2.urls[1] == "/world");
//     CHECK(location2.properties["root"].size() == 1);
//     CHECK(location2.properties["root"][0] == "/");
//     CHECK(location2.index == "index.html");

//     delete location;
// }
#endif