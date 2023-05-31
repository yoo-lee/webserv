#include "LimitExcept.hpp"
#include "BlockStatement.hpp"
#include "SimpleStatement.hpp"
#include "SyntaxError.hpp"
#include <iostream>
#ifdef UNIT_TEST
#include "doctest.h"
#endif
LimitExcept::LimitExcept() {}
LimitExcept::LimitExcept(Statement const* directive) : deny_all(false), allow_all(false)
{
    if (directive == NULL)
        throw SyntaxError("LimitExcept: Taken directive is NULL.");
    if (!dynamic_cast<BlockStatement const*>(directive))
        throw SyntaxError("LimitExcept: Taken directive is not a block statement.");
    if (directive->get_directive() != "limit_except")
        throw SyntaxError("LimitExcept: Taken directive is not a limit_except. Taken directive is " +
                          directive->get_directive() + ".");
    BlockStatement const& limit_except_directive = *(dynamic_cast<BlockStatement const*>(directive));
    for (size_t i = 0; i < limit_except_directive.get_params().size(); i++)
        methods.push_back(std::string(limit_except_directive.get_params()[i]));
    std::vector<SimpleStatement const*> statements = limit_except_directive.get_simple_statement_children();
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->get_directive() == "deny") {
            if (statements[i]->get_param(0) == "all")
                deny_all = true;
            else
                deny_list.push_back(statements[i]->get_param(0));
        } else if (statements[i]->get_directive() == "allow") {
            if (statements[i]->get_param(0) == "all")
                allow_all = true;
            else
                allow_list.push_back(statements[i]->get_param(0));
        } else
            throw SyntaxError("LimitExcept: Taken directive is not a deny or allow.");
    }
}

LimitExcept::LimitExcept(LimitExcept const& l)
{
    methods = l.methods;
    deny_list = l.deny_list;
    allow_list = l.allow_list;
    deny_all = l.deny_all;
    allow_all = l.allow_all;
}

#include <iostream>
LimitExcept::~LimitExcept() {}

#ifdef UNIT_TEST
TEST_CASE("LimitExcept: default")
{
    LimitExcept limit_except;
    CHECK(limit_except.methods.size() == 0);
    CHECK(limit_except.deny_list.size() == 0);
    CHECK(limit_except.allow_list.size() == 0);
}

#include <Parser.hpp>
TEST_CASE("LimitExcept: from statement (and copy constructor)")
{
    vector<string> params;
    params.push_back("GET");
    params.push_back("POST");
    vector<Statement const*> statements;
    statements.push_back(new SimpleStatement("deny", "all"));
    BlockStatement const* limit_except_directive = new BlockStatement("limit_except", params, statements);

    LimitExcept limit_except_parent(limit_except_directive);
    LimitExcept limit_except(limit_except_parent);
    REQUIRE(limit_except.methods.size() == 2);
    CHECK(limit_except.methods[0] == "GET");
    CHECK(limit_except.methods[1] == "POST");
    REQUIRE(limit_except.deny_list.size() == 0);
    REQUIRE(limit_except.allow_list.size() == 0);
    CHECK(limit_except.deny_all == true);
    CHECK(limit_except.allow_all == false);
}

#endif