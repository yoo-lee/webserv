#include "Location.hpp"
#include "BlockStatement.hpp"
#include "SyntaxError.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif

Location::Location() {}

Location::Location(BlockStatement const &location_directive)
{
    if (location_directive.get_directive() != "location")
        throw SyntaxError("taken directive is not location directive");
    urls = location_directive.get_params();
    std::vector<Statement *> location_statements = location_directive.get_children();
    for (size_t i = 0; i < location_statements.size(); i++)
        properties[location_statements[i]->get_directive()] = location_statements[i]->get_params()[0];
}

Location::Location(Location *location) : urls(location->urls), properties(location->properties) {}

Location::~Location() {}

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
    BlockStatement location_directive("location", params, statements);
    Location location(location_directive);
    CHECK(location.urls[0] == "/hello");
    CHECK(location.urls[1] == "/world");
    CHECK(location.properties["root"] == "/");
    CHECK(location.properties["index"] == "index.html");
}

TEST_CASE("Location: copy constructor")
{
    std::vector<std::string> params;
    params.push_back("/hello");
    params.push_back("/world");

    std::vector<Statement *> statements;
    statements.push_back(new SimpleStatement("root", "/"));
    statements.push_back(new SimpleStatement("index", "index.html"));
    BlockStatement location_directive("location", params, statements);
    Location location(location_directive);
    Location location2(&location);
    CHECK(location2.urls[0] == "/hello");
    CHECK(location2.urls[1] == "/world");
    CHECK(location2.properties["root"] == "/");
    CHECK(location2.properties["index"] == "index.html");
}
#endif