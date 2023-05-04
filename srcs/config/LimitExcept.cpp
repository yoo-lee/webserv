#include "LimitExcept.hpp"
#include "BlockStatement.hpp"
#include "SimpleStatement.hpp"
#include "SyntaxError.hpp"
#include <iostream>
#ifdef UNIT_TEST
#include "doctest.h"
#endif
LimitExcept::LimitExcept() {}
LimitExcept::LimitExcept(Statement *const directive)
{
    if (directive == NULL)
        throw SyntaxError("LimitExcept: Taken directive is NULL.");
    if (!dynamic_cast<BlockStatement *>(directive))
        throw SyntaxError("LimitExcept: Taken directive is not a block statement.");
    if (directive->get_directive() != "limit_except")
        throw SyntaxError("LimitExcept: Taken directive is not a limit_except. Taken directive is " +
                          directive->get_directive() + ".");

    BlockStatement &limit_except_directive = *(dynamic_cast<BlockStatement *>(directive));
    for (size_t i = 0; i < limit_except_directive.get_params().size(); i++)
        methods.push_back(std::string(limit_except_directive.get_params()[i]));
    std::vector<Statement *> statements = limit_except_directive.get_children();
    for (size_t i = 0; i < statements.size(); i++)
    {
        SimpleStatement *simple_statement = dynamic_cast<SimpleStatement *>(statements[i]);
        if (simple_statement == NULL)
            throw SyntaxError("LimitExcept: Taken directive is not a simple statement.");
        if (simple_statement->get_directive() == "deny")
            deny_list.push_back(std::string(simple_statement->get_param(0)));
        else if (simple_statement->get_directive() == "allow")
            allow_list.push_back(std::string(simple_statement->get_param(0)));
        else
            throw SyntaxError("LimitExcept: Taken directive is not a deny or allow.");
    }
}
LimitExcept::LimitExcept(LimitExcept *const l)
{
    methods = l->methods;
    deny_list = l->deny_list;
    allow_list = l->allow_list;
}

#include <iostream>
LimitExcept::~LimitExcept()
{
    // すべてのプロパティを表示
    std::cout << "methods: ";
    std::cout << methods.size() << " | ";
    for (size_t i = 0; i < methods.size(); i++)
        std::cout << methods[i] << " ";
    std::cout << std::endl;
    std::cout << "deny_list: ";
    std::cout << deny_list.size() << " | ";
    for (size_t i = 0; i < deny_list.size(); i++)
        std::cout << deny_list[i] << " ";
    std::cout << std::endl;
    std::cout << "allow_list: ";
    std::cout << allow_list.size() << " | ";
    for (size_t i = 0; i < allow_list.size(); i++)
        std::cout << allow_list[i] << " ";
    std::cout << std::endl;
}

#ifdef UNIT_TEST
TEST_CASE("LimitExcept: default")
{
    LimitExcept limit_except;
    CHECK(limit_except.methods.size() == 0);
    CHECK(limit_except.deny_list.size() == 0);
    CHECK(limit_except.allow_list.size() == 0);
}
#endif