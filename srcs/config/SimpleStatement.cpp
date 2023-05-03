#ifdef UNIT_TEST
#include "doctest.h"
#endif
#include "SimpleStatement.hpp"

SimpleStatement::SimpleStatement(std::string directive, std::vector<std::string> params) : Statement(directive, params)
{
    if (params.size() == 0)
        throw SyntaxError("SimpleStatement params size must be > 0");
}
SimpleStatement::SimpleStatement(std::string directive, std::string param) : Statement(directive, param) {}
SimpleStatement::SimpleStatement(std::string directive) : Statement(directive) {}
SimpleStatement::SimpleStatement(const SimpleStatement &s) : Statement(s) {}
SimpleStatement::SimpleStatement(Statement *s) : Statement(*s) {}
SimpleStatement::~SimpleStatement() {}

Statement *SimpleStatement::clone() const
{
    return new SimpleStatement(*this);
}

#ifdef UNIT_TEST
TEST_CASE("SimpleStatement constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");
    SimpleStatement s("directive", params);
    CHECK(s.get_directive() == "directive");
    CHECK(s.get_params().size() == 2);
    CHECK(s.get_params()[0] == "param1");
    CHECK(s.get_params()[1] == "param2");
    CHECK(&(s.get_params()[0]) != &(params[0]));
    CHECK(&(s.get_params()[1]) != &(params[1]));
}

TEST_CASE("SimpleStatement copy constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");
    SimpleStatement s("directive", params);
    SimpleStatement s2(s);
    CHECK(s2.get_directive() == "directive");
    CHECK(s2.get_params().size() == 2);
    CHECK(s2.get_params()[0] == "param1");
    CHECK(s2.get_params()[1] == "param2");
    CHECK(&(s.get_params()[0]) != &(s2.get_params()[0]));
    CHECK(&(s.get_params()[1]) != &(s2.get_params()[1]));
}
#endif /* UNIT_TEST */