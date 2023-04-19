#ifndef PARSER_H
#define PARSER_H
#include "AST.hpp"
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
};

Parser::Parser(std::string file_text)
{
    // Lexer lexer(file_text);
    (void)file_text;
    Lexer lexer("simple statement;");
    std::vector<Token *> tokens = lexer.getTokenList();
    _tree = AST(tokens);
    std::cout << _tree << std::endl;
}

Parser::~Parser()
{
}

#ifdef TEST
int main()
{
    Parser parser("simple statement;");
    return 0;
}
#endif

#endif /* PARSER_H */
