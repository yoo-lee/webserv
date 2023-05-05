#ifndef SIMPLESTATEMENT_H
#define SIMPLESTATEMENT_H
#include "Statement.hpp"
#include "SyntaxError.hpp"
#include <string>
#include <vector>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::string;
using std::vector;

class SimpleStatement : public Statement
{
  public:
    SimpleStatement(string directive, vector<string> params);
    SimpleStatement(string directive, string param);
    SimpleStatement(string directive);
    SimpleStatement(const SimpleStatement &s);
    SimpleStatement(Statement const *s);
    ~SimpleStatement();
    virtual Statement *clone() const;
};

#endif /* SIMPLESTATEMENT_H */
