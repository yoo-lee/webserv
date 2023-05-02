#ifndef LEXER_H
#define LEXER_H
#include "Token.hpp"
#include <ctype.h>
#include <iostream>
#include <ostream>
#include <vector>
class Lexer
{
  private:
    std::string _text;
    Token::Type _state;
    std::vector<Token> _token_list;
    Token::Type get_state(char c) const;
    void set_state();
    char update_state();
    char consume();
    void tokenize();
    std::string extract_str(Token::Type quote_type);
    friend std::ostream &operator<<(std::ostream &os, const Lexer &lexer);

  public:
    Lexer(const std::string &text);
    ~Lexer();
    std::vector<Token> get_token_list() const;
};

#endif /* LEXER_H */
