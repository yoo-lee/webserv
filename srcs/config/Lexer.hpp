#ifndef LEXER_H
#define LEXER_H
#include "Token.hpp"
#include <ctype.h>
#include <iostream>
#include <vector>
class Lexer
{
  private:
    std::string _text;
    Token::Type _state;
    std::vector<Token *> _token_list;
    Token::Type getState(char c) const;
    void setState();
    char updateState();
    char consume();
    void tokenize();
    std::string extractString(Token::Type quote_type);

  public:
    Lexer(std::string text);
    ~Lexer();
    std::vector<Token *> getTokenList() const;
};

#endif /* LEXER_H */
