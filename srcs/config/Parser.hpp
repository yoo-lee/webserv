#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "AST.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
class Parser {
   private:
    /* data */
   public:
    Parser(std::string file_text);
    ~Parser();
};

Parser::Parser(std::string file_text) {
    // Lexer lexer(file_text);
    (void)file_text;
    Lexer lexer("simple statement;");
    std::vector<Token*> tokens = lexer.getTokenList();
    AST at(tokens);
}

Parser::~Parser() {}

#endif /* PARSER_H */
