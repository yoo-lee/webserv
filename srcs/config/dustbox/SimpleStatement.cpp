#include "SimpleStatement.hpp"
#include <string>
#include <vector>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

SimpleStatement::SimpleStatement(std::string directive, std::vector<std::string> _params, std::string value)
    : Statement(directive, _params), _value(value)
{
}

SimpleStatement::SimpleStatement(std::string directive, std::string value)
    : Statement(directive, std::vector<std::string>(0)), _value(value)
{
}

SimpleStatement::SimpleStatement(const SimpleStatement &s) : Statement(s), _value(s._value)
{
}

SimpleStatement::~SimpleStatement()
{
}

std::string SimpleStatement::get_value() const
{
    return _value;
}

void SimpleStatement::print(std::ostream &os) const
{
    Statement::print(os);
    os << " value: '" << _value << "'";
}

std::ostream &operator<<(std::ostream &os, const SimpleStatement &statement)
{
    statement.print(os);
    return os;
}

bool SimpleStatement::operator==(const SimpleStatement &other) const
{
    return _directive == other._directive && _params == other._params && _value == other._value;
}
bool SimpleStatement::operator!=(const SimpleStatement &other) const
{
    return !(*this == other);
}

#ifdef UNIT_TEST
TEST_CASE("SimpleStatement constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");

    SimpleStatement s("directive", params, "value");
    CHECK(s.get_directive() == "directive");
    CHECK(s.get_params().size() == 2);
    CHECK(s.get_params()[0] == "param1");
    CHECK(s.get_params()[1] == "param2");
    CHECK(s.get_value() == "value");
}

TEST_CASE("SimpleStatement copy constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");
    SimpleStatement s("directive", params, "value");
    SimpleStatement s2(s);
    CHECK(s2.get_directive() == "directive");
    CHECK(s2.get_params().size() == 2);
    CHECK(s2.get_params()[0] == "param1");
    CHECK(s2.get_params()[1] == "param2");
    CHECK(&(s.get_params()[0]) != &(s2.get_params()[0]));
    CHECK(&(s.get_params()[1]) != &(s2.get_params()[1]));
    CHECK(s2.get_value() == "value");
}

#endif