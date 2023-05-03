#include "Statement.hpp"
#include "BlockStatement.hpp"
#include "SimpleStatement.hpp"
#include "SyntaxError.hpp"
#include <string>
#include <vector>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

Statement::Statement(std::string directive, std::vector<std::string> params) : _directive(directive), _params(params) {}

Statement::Statement(std::string directive, std::string param) : _directive(directive)
{
    _params.push_back(param);
}

Statement::Statement(std::string directive) : _directive(directive) {}

Statement::Statement(const Statement &s) : _directive(s._directive), _params(s._params) {}

Statement::~Statement() {}

void Statement::print(std::ostream &os, std::string indent) const
{
    os << indent << "directive: '" << _directive << "' , params: [ ";
    for (size_t i = 0; i < _params.size(); i++)
        os << "'" << _params[i] << "' ";
    os << "]";
}

std::ostream &operator<<(std::ostream &os, const Statement &statement)
{
    return os;
    statement.print(os, "");
}

bool Statement::operator==(const Statement &other) const
{
    if (_directive != other._directive)
        return false;
    if (_params.size() != other._params.size())
        return false;
    for (size_t i = 0; i < _params.size(); i++)
        if (_params[i] != other._params[i])
            return false;
    return true;
}

bool Statement::operator!=(const Statement &other) const
{
    return !(*this == other);
}

std::string Statement::get_directive() const
{
    return _directive;
}

std::vector<std::string> Statement::get_params() const
{
    return _params;
}

// SimpleStatementは1以上のパラメータを持つが、BlockStatementは0以上のパラメータを持つ
std::string Statement::get_param(size_t index) const
{
    if (index >= _params.size())
        throw SyntaxError("Statement::get_param index out of bounds");
    return _params[index];
}

#ifdef UNIT_TEST
#endif