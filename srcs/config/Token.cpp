#include "Token.hpp"
#include <iostream>
#include <ostream>
#include <string>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

Token::Token(std::string str, Token::Type type) : _type(type), _str(str)
{
}

Token::Token(char c, Token::Type type) : _type(type), _str(std::string(&c, 1))
{
}

std::string Token::getTypeName(Token::Type type)
{
    const char *type_name_list[] = {"COMMA",  "SEMI", "DQUOTE", "SQUOTE",      "STRING", "LCURLY",
                                    "RCURLY", "INT",  "ID",     "WHITE_SPACE", "NONE"};
    if (type <= Token::NONE && type >= 0)
        return type_name_list[type];
    return "UNKNOWN";
}

Token::Type Token::getType() const
{
    return _type;
}

std::string Token::getStr() const
{
    return _str;
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    os << token.getStr() << " : " << Token::getTypeName(token.getType());
    return os;
}

Token::~Token()
{
}

bool Token::operator==(const Token &rhs) const
{
    return _type == rhs.getType() && _str == rhs.getStr();
}
bool Token::operator!=(const Token &rhs) const
{
    return !(*this == rhs);
}

Token &Token::operator=(const Token &rhs)
{
    _type = rhs._type;
    _str = rhs._str;
    return *this;
}

#ifdef UNIT_TEST
TEST_CASE("Token Class Test")
{
    CHECK(Token::getTypeName(Token::COMMA) == "COMMA");
    CHECK(Token::getTypeName(Token::SEMI) == "SEMI");
    CHECK(Token::getTypeName(Token::DQUOTE) == "DQUOTE");
    CHECK(Token::getTypeName(Token::SQUOTE) == "SQUOTE");
    CHECK(Token::getTypeName(Token::STRING) == "STRING");
    CHECK(Token::getTypeName(Token::LCURLY) == "LCURLY");
    CHECK(Token::getTypeName(Token::RCURLY) == "RCURLY");
    CHECK(Token::getTypeName(Token::INT) == "INT");
    CHECK(Token::getTypeName(Token::ID) == "ID");
    CHECK(Token::getTypeName(Token::WHITE_SPACE) == "WHITE_SPACE");
    CHECK(Token::getTypeName(Token::Type(100)) == "UNKNOWN");
}
#endif