#include "Statement.hpp"
#include <string>
#include <vector>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

Statement::Statement(std::string directive, std::vector<std::string> params) : _directive(directive), _params(params)
{
}

Statement::Statement(std::string directive, std::string param) : _directive(directive)
{
    _params.push_back(param);
}

Statement::Statement(const Statement &s) : _directive(s._directive), _params(s._params)
{
}

Statement::~Statement()
{
}

void Statement::print(std::ostream &os) const
{
    os << "directive: '" << _directive << "' , params: [ ";
    for (size_t i = 0; i < _params.size(); i++)
    {
        os << "'" << _params[i] << "' ";
    }
    os << "]";
}

std::ostream &operator<<(std::ostream &os, const Statement &statement)
{
    statement.print(os);
    return os;
}

std::string Statement::get_directive() const
{
    return _directive;
}

std::vector<std::string> Statement::get_params() const
{
    return _params;
}

#ifdef UNIT_TEST

TEST_CASE("Statement constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");
    Statement s("directive", params);
    CHECK(s.get_directive() == "directive");
    CHECK(s.get_params().size() == 2);
    CHECK(s.get_params()[0] == "param1");
    CHECK(s.get_params()[1] == "param2");
    CHECK(&(s.get_params()[0]) != &(params[0]));
    CHECK(&(s.get_params()[1]) != &(params[1]));
}

TEST_CASE("Statement copy constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");
    Statement s("directive", params);
    Statement s2(s);
    CHECK(s2.get_directive() == "directive");
    CHECK(s2.get_params().size() == 2);
    CHECK(s2.get_params()[0] == "param1");
    CHECK(s2.get_params()[1] == "param2");
    CHECK(&(s.get_params()[0]) != &(s2.get_params()[0]));
    CHECK(&(s.get_params()[1]) != &(s2.get_params()[1]));
}

#endif