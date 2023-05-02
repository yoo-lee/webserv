#ifndef SIMPLESTATEMENT_H
#define SIMPLESTATEMENT_H
#include "Statement.hpp"
#include "SyntaxError.hpp"
#include <string>
#include <vector>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

class SimpleStatement : public Statement
{
  public:
    SimpleStatement(std::string directive, std::vector<std::string> params);
    SimpleStatement(std::string directive, std::string param);
    SimpleStatement(std::string directive);
    SimpleStatement(const SimpleStatement &s);
    SimpleStatement(Statement *s);
    ~SimpleStatement();
};

#endif /* SIMPLESTATEMENT_H */