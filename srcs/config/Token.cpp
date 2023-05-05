#include "Token.hpp"
#include <iostream>
#include <ostream>
#include <string>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::ostream;

Token::Token() {}

Token::Token(string str, Token::Type type) : _type(type), _str(str) {}

Token::Token(char c, Token::Type type) : _type(type), _str(string(&c, 1)) {}

Token::Token(const Token& token)
{
    _type = token.get_type();
    _str = token.get_str();
}

string Token::get_typename(Token::Type type)
{
    const char* type_name_list[] = {"COMMA",  "SEMI", "DQUOTE", "SQUOTE",      "STRING",  "LCURLY",
                                    "RCURLY", "INT",  "ID",     "WHITE_SPACE", "COMMENT", "NONE"};
    if (type <= Token::NONE && type >= 0)
        return type_name_list[type];
    return "UNKNOWN";
}

Token::Type Token::get_type() const
{
    return _type;
}

string Token::get_str() const
{
    return _str;
}

ostream& operator<<(ostream& os, const Token& token)
{
    os << token.get_str() << " : " << Token::get_typename(token.get_type());
    return os;
}

Token::~Token() {}

bool Token::operator==(const Token& rhs) const
{
    return _type == rhs.get_type() && _str == rhs.get_str();
}
bool Token::operator!=(const Token& rhs) const
{
    return !(*this == rhs);
}

Token& Token::operator=(const Token& rhs)
{
    _type = rhs._type;
    _str = rhs._str;
    return *this;
}

#ifdef UNIT_TEST
TEST_CASE("Token Class Test")
{
    CHECK(Token::get_typename(Token::COMMA) == "COMMA");
    CHECK(Token::get_typename(Token::SEMI) == "SEMI");
    CHECK(Token::get_typename(Token::DQUOTE) == "DQUOTE");
    CHECK(Token::get_typename(Token::SQUOTE) == "SQUOTE");
    CHECK(Token::get_typename(Token::STRING) == "STRING");
    CHECK(Token::get_typename(Token::LCURLY) == "LCURLY");
    CHECK(Token::get_typename(Token::RCURLY) == "RCURLY");
    CHECK(Token::get_typename(Token::INT) == "INT");
    CHECK(Token::get_typename(Token::ID) == "ID");
    CHECK(Token::get_typename(Token::WHITE_SPACE) == "WHITE_SPACE");
    CHECK(Token::get_typename(Token::COMMENT) == "COMMENT");
    CHECK(Token::get_typename(Token::Type(100)) == "UNKNOWN");
}
#endif