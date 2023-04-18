#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "Token.hpp"
class Parser {
   private:
    /* data */
   public:
    Parser(std::vector<Token*> token_list);
    ~Parser();
};

Parser::Parser(std::vector<Token*> token_list) {}

Parser::~Parser() {}

#endif /* PARSER_H */
