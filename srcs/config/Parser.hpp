#ifndef PARSER_H
#define PARSER_H
#include "AST.hpp"
#include "Http.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include <iostream>
#include <vector>
class Parser
{
  private:
    AST _tree;

  public:
    Parser(std::string file_text);
    ~Parser();
    void print();
    std::vector<Statement *> const &get_root();
};
#endif /* PARSER_H */
