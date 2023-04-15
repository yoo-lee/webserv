#include "Lexer.hpp"
#include <ctype.h>
#include <iostream>
#include <vector>
#include "Token.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif

Lexer::Lexer(std::string text)
    : _text(text), _token_list(std::vector<Token*>()), _state(Token::COMMA) {
    while (_text.length() != 0 && _state != Token::NONE) {
        std::string buf = "";
        setState();
        if (_state == Token::DQUOTE) {
            consume();
            while (_text.length() != 0 && _text[0] != '"')
                buf += consume();
            _token_list.push_back(new Token(buf, Token::STRING));
            consume();
        } else if (_state == Token::SQUOTE) {
            consume();
            while (_text.length() != 0 && _text[0] != '\'')
                buf += consume();
            _token_list.push_back(new Token(buf, Token::STRING));
            consume();
        } else if (_state == Token::COMMA || _state == Token::SEMI ||
                   _state == Token::DQUOTE || _state == Token::LCURLY ||
                   _state == Token::RCURLY) {
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

Lexer::~Lexer() {
    for (std::vector<Token*>::iterator it = _token_list.begin();
         it != _token_list.end(); ++it) {
        delete *it;
    }
    _token_list.clear();
}

Token::Type Lexer::getState(char c) const {
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
    } else if (c == '\'') {
        return Token::SQUOTE;
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
    if (_text.length() == 0)
        return Token::NONE;
    int i = c;
    std::cerr << "Invalid character: '" << c << "' (" << i << ")" << std::endl;
    throw std::runtime_error("error");
}

void Lexer::setState() {
    // std::cout << "'" << Token::getTypeName(_state) << "' =>";
    _state = getState(_text[0]);
    // std::cout << "'" << Token::getTypeName(_state) << "' (" << _text[0] <<
    // ")"
    //   << std::endl;
}

char Lexer::updateState() {
    char c = consume();
    setState();
    return c;
}

char Lexer::consume() {
    char buf = _text[0];
    _text = _text.substr(1);
    return buf;
}

std::vector<Token*> Lexer::getTokenList() const {
    return _token_list;
}

#ifdef UNIT_TEST
TEST_CASE("Lexer Basic Char Test") {
    CHECK(Lexer("").getTokenList().size() == 0);
    CHECK(Lexer("''").getTokenList().size() == 1);
    CHECK(*Lexer("ANYCHAR").getTokenList()[0] ==
          Token("ANYCHAR", Token::ANYCHAR));
    CHECK(*Lexer("ANYCH4R").getTokenList()[0] ==
          Token("ANYCH4R", Token::ANYCHAR));
    CHECK(*Lexer("4NYCH4R").getTokenList()[0] == Token("4", Token::INT));
    CHECK(*Lexer(";").getTokenList()[0] == Token(";", Token::SEMI));
    CHECK(*Lexer("{").getTokenList()[0] == Token("{", Token::LCURLY));
    CHECK(*Lexer("}").getTokenList()[0] == Token("}", Token::RCURLY));
    CHECK(*Lexer(",").getTokenList()[0] == Token(",", Token::COMMA));
    CHECK(*Lexer("100").getTokenList()[0] == Token("100", Token::INT));
    CHECK(*Lexer("\"string test\"").getTokenList()[0] ==
          Token("string test", Token::STRING));
}

TEST_CASE("Lexer Simple Statement Test") {
    Lexer l("key param1 param2;");
    std::vector<Token*> tl = l.getTokenList();
    REQUIRE(tl.size() == 4);
    CHECK(*tl[0] == Token("key", Token::ANYCHAR));
    CHECK(*tl[1] == Token("param1", Token::ANYCHAR));
    CHECK(*tl[2] == Token("param2", Token::ANYCHAR));
    CHECK(*tl[3] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer Block Statement Test") {
    Lexer l("key param1 param2 {\n}");
    std::vector<Token*> tl = l.getTokenList();
    REQUIRE(tl.size() == 5);
    CHECK(*tl[0] == Token("key", Token::ANYCHAR));
    CHECK(*tl[1] == Token("param1", Token::ANYCHAR));
    CHECK(*tl[2] == Token("param2", Token::ANYCHAR));
    CHECK(*tl[3] == Token("{", Token::LCURLY));
    CHECK(*tl[4] == Token("}", Token::RCURLY));
}

TEST_CASE("Lexer Complex Block Statement Test") {
    Lexer l("key1 param1 param2 {\nkey2 param3 param4 {\nkey3 value;\n}\n}");
    std::vector<Token*> tl = l.getTokenList();
    REQUIRE(tl.size() == 13);
    CHECK(*tl[0] == Token("key1", Token::ANYCHAR));
    CHECK(*tl[1] == Token("param1", Token::ANYCHAR));
    CHECK(*tl[2] == Token("param2", Token::ANYCHAR));
    CHECK(*tl[3] == Token("{", Token::LCURLY));
    CHECK(*tl[4] == Token("key2", Token::ANYCHAR));
    CHECK(*tl[5] == Token("param3", Token::ANYCHAR));
    CHECK(*tl[6] == Token("param4", Token::ANYCHAR));
    CHECK(*tl[7] == Token("{", Token::LCURLY));
    CHECK(*tl[8] == Token("key3", Token::ANYCHAR));
    CHECK(*tl[9] == Token("value", Token::ANYCHAR));
    CHECK(*tl[10] == Token(";", Token::SEMI));
    CHECK(*tl[11] == Token("}", Token::RCURLY));
    CHECK(*tl[12] == Token("}", Token::RCURLY));
}

TEST_CASE("Lexer practical Test1") {
    Lexer l(
        "log_format main '$remote_addr - $remote_user [$time_local] "
        "\"$request\" '\n    '$status $body_bytes_sent \"$http_referer\" "
        "'\n    '\"$http_user_agent\" \"$http_x_forwarded_for\"';");
    std::vector<Token*> tl = l.getTokenList();
    REQUIRE(tl.size() == 6);
    CHECK(*tl[0] == Token("log_format", Token::ANYCHAR));
    CHECK(*tl[1] == Token("main", Token::ANYCHAR));
    CHECK(*tl[2] ==
          Token("$remote_addr - $remote_user [$time_local] \"$request\" ",
                Token::STRING));
    CHECK(*tl[3] ==
          Token("$status $body_bytes_sent \"$http_referer\" ", Token::STRING));
    CHECK(*tl[4] == Token("\"$http_user_agent\" \"$http_x_forwarded_for\"",
                          Token::STRING));
    CHECK(*tl[5] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer practical Test2") {
    Lexer l("gzip_disable \"MSIE [1-6]\\.\";");
    std::vector<Token*> tl = l.getTokenList();
    REQUIRE(tl.size() == 3);
    CHECK(*tl[0] == Token("gzip_disable", Token::ANYCHAR));
    CHECK(*tl[1] == Token("MSIE [1-6]\\.", Token::STRING));
    CHECK(*tl[2] == Token(";", Token::SEMI));
}
#endif