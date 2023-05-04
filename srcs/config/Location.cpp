#include "Location.hpp"
#include "BlockStatement.hpp"
#include "LimitExcept.hpp"
#include "SyntaxError.hpp"
#include <iostream>
#ifdef UNIT_TEST
#include "doctest.h"
#endif

Location::Location() {}

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
    for (size_t i = 0; i < location_statements.size(); i++)
    {
        if (location_statements[i]->get_directive() == "limit_except")
        {
            limit_except = new LimitExcept(location_statements[i]);
        }
        else
        {
            if (dynamic_cast<BlockStatement *>(location_statements[i]))
                throw SyntaxError("Location: Detected Non limit_except BlockStatement. Only limit_except is allowed "
                                  "for block statements in the Location directive.");
            properties[location_statements[i]->get_directive()] = location_statements[i]->get_params();
            // std::cout << limit_except->methods.size() << std::endl;
        }
    }
}

Location::Location(Location *location) : urls(location->urls), properties(location->properties)
{
    if (this->limit_except != NULL)
        limit_except = new LimitExcept(location->limit_except);
}

Location::~Location()
{
    delete limit_except;
}

std::string Location::operator[](size_t index) const
{
    if (index < urls.size())
        return urls[index];
    throw SyntaxError("Location: index out of range");
}

#ifdef UNIT_TEST
#include "SimpleStatement.hpp"
TEST_CASE("Location: constructor")
{
    std::vector<std::string> params;
    params.push_back("/hello");
    params.push_back("/world");

    std::vector<Statement *> statements;
    statements.push_back(new SimpleStatement("root", "/"));
    statements.push_back(new SimpleStatement("index", "index.html"));
    BlockStatement *location_directive = new BlockStatement("location", params, statements);
    Location location(location_directive);
    CHECK(location.urls[0] == "/hello");
    CHECK(location.urls[1] == "/world");
    CHECK(location.properties["root"][0] == "/");
    CHECK(location.properties["index"][0] == "index.html");

    delete location_directive;
}

TEST_CASE("Location: copy constructor")
{
    std::vector<std::string> params;
    params.push_back("/hello");
    params.push_back("/world");

    std::vector<Statement *> statements;
    statements.push_back(new SimpleStatement("root", "/"));
    statements.push_back(new SimpleStatement("index", "index.html"));
    BlockStatement *location_directive = new BlockStatement("location", params, statements);
    Location *location = new Location(location_directive);
    Location location2(location);
    CHECK(location2.urls[0] == "/hello");
    CHECK(location2.urls[1] == "/world");
    CHECK(location2.properties["root"][0] == "/");
    CHECK(location2.properties["index"][0] == "index.html");

    delete location;
}
#endif