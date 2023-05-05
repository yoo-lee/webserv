#ifndef LEXER_H
#define LEXER_H
#include "Token.hpp"
#include <ctype.h>
#include <iostream>
#include <ostream>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class Lexer
{
  private:
    string _text;
    Token::Type _state;
    vector<Token> _token_list;
    Token::Type get_state(char c) const;
    void set_state();
    char update_state();
    char consume();
    void tokenize();
    string extract_str(Token::Type quote_type);
    friend ostream &operator<<(ostream &os, const Lexer &lexer);

  public:
    Lexer(string const &text);
    ~Lexer();
    vector<Token> get_token_list() const;
};

#endif /* LEXER_H */
