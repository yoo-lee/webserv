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
        while (_text.length() != 0) {
            std::string buf;
            setState();
            if (_text[0] == '"') {
                consume();
                while (_text.length() != 0 && _text[0] != '"')
                    buf += consume();
                _token_list.push_back(new Token(buf, Token::STRING));
                consume();
            } else if (_text[0] == '\'') {
                consume();
                while (_text.length() != 0 && _text[0] != '\'')
                    buf += consume();
                _token_list.push_back(new Token(buf, Token::STRING));
                consume();
            } else if (_state == Token::SEMI || _state == Token::DQUOTE ||
                       _state == Token::LCURLY || _state == Token::RCURLY) {
                _token_list.push_back(new Token(consume(), _state));
            } else if (_state == Token::ANYCHAR) {
                while (_text.length() != 0 && _state == Token::ANYCHAR)
                    buf += updateState();
                _token_list.push_back(new Token(buf, Token::ANYCHAR));
            } else if (_state == Token::INT) {
                while (_text.length() != 0 && _state == Token::INT)
                    buf += updateState();
                _token_list.push_back(new Token(buf, Token::INT));
            } else if (_state == Token::ID) {
                while (_text.length() != 0 && _state == Token::ID)
                    buf += updateState();
                _token_list.push_back(new Token(buf, Token::ID));
            } else
                consume();
        }
    }

    ~Lexer() { _token_list.clear(); }
    Token::Type getState(char c) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
            return Token::WHITE_SPACE;
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
            if (_state == Token::ID)
                return Token::ID;
            if (_state == Token::ANYCHAR)
                return Token::ANYCHAR;
            return Token::INT;
        } else if (isprint(c)) {
            if (_state == Token::ID)
                return Token::ID;
            return Token::ANYCHAR;
        }
        throw std::runtime_error("Invalid character: '" + std::string(&c, 1) +
                                 "'");
    }
    void setState() {
        // std::cout << "'" << Token::getTypeName(_state) << "' =>";
        _state = getState(_text[0]);
        // std::cout << "'" << Token::getTypeName(_state) << "' (" << _text[0]
        //   << ")" << std::endl;
    }

    char updateState() {
        char c = consume();
        setState();
        return c;
    }

    char consume() {
        char buf = _text[0];
        _text = _text.substr(1);
        return buf;
    }
    std::vector<Token*> getTokenList() const { return _token_list; }
};

#endif /* LEXER_H */
