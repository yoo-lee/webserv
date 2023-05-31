#include "Location.hpp"
#include "BlockStatement.hpp"
#include "LimitExcept.hpp"
#include "SyntaxError.hpp"
#include "utility.hpp"
#include <iostream>
#ifdef UNIT_TEST
#include "doctest.h"
#endif

Location::Location() {}

LimitExcept* Location::parse_limit_except(vector<BlockStatement const*> statements) const
{
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "limit_except")
            return new LimitExcept(statements[i]);
    }
    return NULL;
}

string Location::parse_index(vector<SimpleStatement const*> statements) const
{
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "index")
            return statements[i]->get_params()[0];
    }
    return "index.html";
}

bool Location::parse_autoindex(vector<SimpleStatement const*> statements) const
{
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "autoindex") {
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

map<string, string> Location::parse_error_pages(vector<SimpleStatement const*> statements) const
{
    map<string, string> error_page;
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "error_page") {
            vector<string> params = statements[i]->get_params();
            if (params.size() < 2)
                throw SyntaxError("Location: Invalid error_page directive");
            string page = params[params.size() - 1];
            for (size_t j = 0; j < params.size() - 1; j++)
                error_page[params[j]] = page;
        }
    }
    return error_page;
}

string Location::parse_root(vector<SimpleStatement const*> statements) const
{
    string root;
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "root") {
            vector<string> params = statements[i]->get_params();
            if (params.size() != 1)
                throw SyntaxError("Location: Root must be single paramater");
            root = params[params.size() - 1];
        }
    }
    return root;
}

string Location::parse_cgi_pass(vector<SimpleStatement const*> statements) const
{
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "cgi_pass") {
            if (statements[i]->get_params().size() == 1)
                return statements[i]->get_params()[0];
            else if (statements[i]->get_params().size() == 0)
                return "";
            else
                throw SyntaxError("Location: Invalid cgi_pass param count(must be 0 or 1))");
        }
    }
    return "";
}

string Location::get_default_error_page(string status_code) const
{
    return "<!DOCTYPE html><html lang=\"ja\"><head>    <meta charset=\"UTF-8\">"
           "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
           "    <title>Document</title></head><body>" +
           status_code + " : " + Utility::get_http_status_message(status_code) + "</body></html>";
}

string Location::get_error_page(string status_code) const
{
    if (error_page.count(status_code) == 0)
        return get_default_error_page(status_code);
    return error_page.at(status_code);
}

Location::Location(Statement const* directive) : limit_except(0), cgi_pass("")
{
    if (directive == NULL)
        throw SyntaxError("Location: Taken directive is NULL");
    if (!dynamic_cast<BlockStatement const*>(directive))
        throw SyntaxError("Location: Taken directive is not block statement");
    if (directive->get_directive() != "location")
        throw SyntaxError("Location: Taken directive is not location directive");

    BlockStatement const* location_directive = dynamic_cast<BlockStatement const*>(directive);

    urls = location_directive->get_params();

    vector<SimpleStatement const*> location_simple_statements = location_directive->get_simple_statement_children();
    vector<BlockStatement const*> location_block_statements = location_directive->get_block_statement_children();

    limit_except = parse_limit_except(location_block_statements);
    index = parse_index(location_simple_statements);
    autoindex = parse_autoindex(location_simple_statements);
    error_page = parse_error_pages(location_simple_statements);
    root = parse_root(location_simple_statements);
    cgi_pass = parse_cgi_pass(location_simple_statements);

    for (size_t i = 0; i < location_simple_statements.size(); i++) {
        // 特定のディレクティブはpropertiesに入れない
        // メンバ変数が別途用意しているプロパティは元々入れない予定だったが、rootやcgi_passを
        // あとから追加したときに後方互換性のためにrootとcgi_passはpropertiesにいる
        if (location_simple_statements[i]->get_directive() != "limit_except" &&
            location_simple_statements[i]->get_directive() != "index" &&
            location_simple_statements[i]->get_directive() != "autoindex" &&
            location_simple_statements[i]->get_directive() != "error_page") {
            if (dynamic_cast<BlockStatement const*>(location_simple_statements[i]))
                throw SyntaxError("Location: Detected Non limit_except BlockStatement. Only limit_except is allowed "
                                  "for block statements in the Location directive.");
            properties[location_simple_statements[i]->get_directive()] = location_simple_statements[i]->get_params();
        }
    }
}

Location::Location(Location const& location) : limit_except(NULL)
{
    *this = location;
}

Location::~Location()
{
    delete limit_except;
}

vector<string> Location::operator[](string index) const
{
    return properties.at(index);
}

Location const& Location::operator=(Location const& other)
{
    if (this != &other) {
        urls = other.urls;
        properties = other.properties;
        index = other.index;
        autoindex = other.autoindex;
        root = other.root;
        cgi_pass = other.cgi_pass;
        error_page = other.error_page;
        delete limit_except;
        if (other.limit_except != NULL)
            limit_except = new LimitExcept(*(other.limit_except));
        else
            limit_except = NULL;
    }
    return *this;
}

#ifdef UNIT_TEST
#include "SimpleStatement.hpp"
TEST_CASE("Location: constructor")
{
    vector<string> params;
    params.push_back("/hello");
    params.push_back("/world");

    vector<Statement const*> statements;
    statements.push_back(new SimpleStatement("root", "/"));
    statements.push_back(new SimpleStatement("index", "index.html"));
    BlockStatement const* location_directive = new BlockStatement("location", params, statements);
    Location location(location_directive);
    CHECK(location.urls.size() == 2);
    CHECK(location.urls[0] == "/hello");
    CHECK(location.urls[1] == "/world");
    CHECK(location.properties["root"].size() == 1);
    CHECK(location.properties["root"][0] == "/");
    CHECK(location.root == "/");
    CHECK(location.cgi_pass == "");
    CHECK(location.index == "index.html");
    delete location_directive;
}

TEST_CASE("Location: copy constructor")
{
    vector<string> params;
    params.push_back("/hello");
    params.push_back("/world");

    vector<Statement const*> statements;
    statements.push_back(new SimpleStatement("root", "/"));
    statements.push_back(new SimpleStatement("index", "index.html"));
    BlockStatement const* location_directive = new BlockStatement("location", params, statements);
    Location* location = new Location(location_directive);
    Location location2(*location);
    CHECK(location2.urls[0] == "/hello");
    CHECK(location2.urls[1] == "/world");
    CHECK(location2.properties["root"].size() == 1);
    CHECK(location2.properties["root"][0] == "/");
    CHECK(location2.root == "/");
    CHECK(location2.cgi_pass == "");
    CHECK(location2.index == "index.html");

    delete location;
    delete location_directive;
}
#endif