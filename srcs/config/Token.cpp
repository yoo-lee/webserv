#include "Token.hpp"
#include <iostream>
#include <ostream>
#include <string>
Token::Token(std::string str, Token::Type type) : _type(type), _str(str) {}

Token::Token(char c, Token::Type type)
    : _type(type), _str(std::string(&c, 1)) {}

std::string Token::getTypeName(Token::Type type) {
    const char* type_name_list[] = {"COMMA",  "SEMI",        "DQUOTE", "SQUOTE",
                                    "STRING", "LCURLY",      "RCURLY", "INT",
                                    "ID",     "WHITE_SPACE", "ANYCHAR"};
    if (type <= Token::ANYCHAR)
        return type_name_list[type];
    return "UNKNOWN";
}

Token::Type Token::getType() const {
    return _type;
}

std::string Token::getStr() const {
    return _str;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.getStr() << " : " << Token::getTypeName(token.getType());
    return os;
}

Token::~Token() {}
