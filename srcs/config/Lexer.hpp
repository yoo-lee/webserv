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

   public:
    Lexer(std::string text) : _text(text) {
        std::string buf;
        while (_text.length() != 0) {
            setState();
            if (_state == Token::SEMI || _state == Token::DQUOTE ||
                _state == Token::LCURLY || _state == Token::RCURLY) {
                _token_list.push_back(
                    new Token(std::string(consume(), 1), _state));
            } else if (_state == Token::ANYCHAR) {
                while (_text.length() != 0 && _state == Token::ANYCHAR) {
                    buf += consume();
                    setState();
                }
                _token_list.push_back(new Token(buf, Token::ANYCHAR));
                buf = "";
                consume();
            } else if (_state == Token::INT) {
                while (_text.length() != 0 && _state == Token::INT) {
                    buf += consume();
                    setState();
                }
                _token_list.push_back(new Token(buf, _state));
                buf = "";
                consume();
            } else if (_state == Token::ID) {
                while (_text.length() != 0 && _state == Token::ID) {
                    buf += consume();
                    setState();
                }
                _token_list.push_back(new Token(buf, _state));
                buf = "";
                consume();
            } else
                consume();
        }
    }

    ~Lexer() { _token_list.clear(); }
    Token::Type getState(char c) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
            return Token::WS;
        } else if (c == '{') {
            return Token::LCURLY;
        } else if (c == '}') {
            return Token::RCURLY;
        } else if (c == ';') {
            return Token::SEMI;
        } else if (c == ',') {
            return Token::COMMA;
        } else if (c == '"') {
            return Token::DQUOTE;
        } else if (c >= '0' && c <= '9') {
            if (_state == Token::INT)
                return Token::INT;
            else if (_state == Token::ID)
                return Token::ID;
        } else if (isprint(c)) {
            if (_state == Token::ID)
                return Token::ID;
        }
    }
    void setState() { _state = getState(_text[0]); }
    char consume() {
        char buf = _text[0];
        _text = _text.substr(1);
        return buf;
    }
    std::vector<Token*> getTokenList() const { return _token_list; }
};

#endif /* LEXER_H */
