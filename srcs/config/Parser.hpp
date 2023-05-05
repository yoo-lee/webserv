#ifndef PARSER_H
#define PARSER_H
#include "AST.hpp"
#include "Http.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include <iostream>
#include <vector>

using std::string;
using std::vector;

class Parser
{
  private:
    AST _tree;

  public:
    Parser(string file_text);
    ~Parser();
    void print();
    vector<Statement const*> const& get_root();
};
#endif /* PARSER_H */
