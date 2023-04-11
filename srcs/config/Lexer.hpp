#ifndef LEXER_H
#define LEXER_H
#include <ctype.h>
#include <iostream>
#include <vector>
#include "Token.hpp"
class Lexer {
   private:
    std::string _text;
    Token::Type _state;
    std::vector<Token*> _token_list;
    Token::Type getState(char c) const throw(std::runtime_error);
    void setState();
    char updateState();
    char consume();

   public:
    Lexer(std::string text);
    ~Lexer();
    std::vector<Token*> getTokenList() const;
};

#endif /* LEXER_H */
