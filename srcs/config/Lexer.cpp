#include "Lexer.hpp"
#include "Token.hpp"
#include <ctype.h>
#include <iostream>
#include <vector>
#ifdef UNIT_TEST
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

Lexer::Lexer(std::string text) : _text(text), _state(Token::COMMA), _token_list(std::vector<Token>())
{
    while (_text.length() != 0 && _state != Token::NONE)
    {
        std::string buf = "";
        set_state();
        switch (_state)
        {
            char quote;
            Token::Type token_type;
        case Token::DQUOTE:
        case Token::SQUOTE:
            quote = consume();
            while (_text.length() != 0 && _text[0] != quote)
                buf += consume();
            _token_list.push_back(Token(buf, Token::STRING));
            consume();
            break;
        case Token::COMMA:
        case Token::SEMI:
        case Token::LCURLY:
        case Token::RCURLY:
            _token_list.push_back(Token(consume(), _state));
            break;
        case Token::INT:
        case Token::ID:
            token_type = _state;
            while (_text.length() != 0 && _state == token_type)
                buf += update_state();
            _token_list.push_back(Token(buf, token_type));
            break;
        default:
            consume();
            break;
        }
    }
}

Lexer::~Lexer()
{
    _token_list.clear();
}

Token::Type Lexer::get_state(char c) const
{
    if (_state == Token::COMMENT && c != '\n')
        return Token::COMMENT;
    switch (c)
    {
    case '\n':
    case ' ':
    case '\t':
    case '\r':
    case '\f':
        return Token::WHITE_SPACE;
    case '#':
        return Token::COMMENT;
    case '{':
        return Token::LCURLY;
    case '}':
        return Token::RCURLY;
    case ';':
        return Token::SEMI;
    case ',':
        return Token::COMMA;
    case '"':
        return Token::DQUOTE;
    case '\'':
        return Token::SQUOTE;
    }
    if (c >= '0' && c <= '9')
    {
        if (_state == Token::ID)
            return Token::ID;
        if (_state == Token::ID)
            return Token::ID;
        return Token::INT;
    }
    if (isprint(c))
    {
        if (_state == Token::ID)
            return Token::ID;
        return Token::ID;
    }
    if (_text.length() == 0)
        return Token::NONE;
    int i = c;
    std::cerr << "Invalid character: '" << c << "' (" << i << ")" << std::endl;
    throw std::runtime_error("error");
}

void Lexer::set_state()
{
    // std::cout << "'" << Token::get_typename(_state) << "' =>";
    _state = get_state(_text[0]);
    // std::cout << "'" << Token::get_typename(_state) << "' (" << _text[0] <<
    // ")"
    //   << std::endl;
}

char Lexer::update_state()
{
    char c = consume();
    set_state();
    return c;
}

char Lexer::consume()
{
    char buf = _text[0];
    _text = _text.substr(1);
    return buf;
}

std::vector<Token> Lexer::get_token_list() const
{
    return _token_list;
}

std::ostream &operator<<(std::ostream &os, const Lexer &lexer)
{
    for (size_t i = 0; i < lexer._token_list.size(); i++)
        os << lexer._token_list[i] << std::endl;
    return os;
}

#ifdef UNIT_TEST
TEST_CASE("Lexer Basic Char Test")
{
    CHECK(Lexer("").get_token_list().size() == 0);
    CHECK(Lexer("''").get_token_list().size() == 1);
    CHECK(Lexer("ID").get_token_list()[0] == Token("ID", Token::ID));
    CHECK(Lexer("ANYCH4R").get_token_list()[0] == Token("ANYCH4R", Token::ID));
    CHECK(Lexer("4NYCH4R").get_token_list()[0] == Token("4", Token::INT));
    CHECK(Lexer(";").get_token_list()[0] == Token(";", Token::SEMI));
    CHECK(Lexer("{").get_token_list()[0] == Token("{", Token::LCURLY));
    CHECK(Lexer("}").get_token_list()[0] == Token("}", Token::RCURLY));
    CHECK(Lexer(",").get_token_list()[0] == Token(",", Token::COMMA));
    CHECK(Lexer("100").get_token_list()[0] == Token("100", Token::INT));
    CHECK(Lexer("\"string test\"").get_token_list()[0] == Token("string test", Token::STRING));
}

TEST_CASE("Lexer Simple Statement Test")
{
    Lexer l("key param1 param2;");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 4);
    CHECK(tl[0] == Token("key", Token::ID));
    CHECK(tl[1] == Token("param1", Token::ID));
    CHECK(tl[2] == Token("param2", Token::ID));
    CHECK(tl[3] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer Block Statement Test")
{
    Lexer l("key param1 param2 {\n}");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 5);
    CHECK(tl[0] == Token("key", Token::ID));
    CHECK(tl[1] == Token("param1", Token::ID));
    CHECK(tl[2] == Token("param2", Token::ID));
    CHECK(tl[3] == Token("{", Token::LCURLY));
    CHECK(tl[4] == Token("}", Token::RCURLY));
}

TEST_CASE("Lexer Complex Block Statement Test")
{
    Lexer l("key1 param1 param2 {\nkey2 param3 param4 {\nkey3 value;\n}\n}");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 13);
    CHECK(tl[0] == Token("key1", Token::ID));
    CHECK(tl[1] == Token("param1", Token::ID));
    CHECK(tl[2] == Token("param2", Token::ID));
    CHECK(tl[3] == Token("{", Token::LCURLY));
    CHECK(tl[4] == Token("key2", Token::ID));
    CHECK(tl[5] == Token("param3", Token::ID));
    CHECK(tl[6] == Token("param4", Token::ID));
    CHECK(tl[7] == Token("{", Token::LCURLY));
    CHECK(tl[8] == Token("key3", Token::ID));
    CHECK(tl[9] == Token("value", Token::ID));
    CHECK(tl[10] == Token(";", Token::SEMI));
    CHECK(tl[11] == Token("}", Token::RCURLY));
    CHECK(tl[12] == Token("}", Token::RCURLY));
}

TEST_CASE("Lexer Comment Test")
{
    Lexer l("# test test \n d p ;");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 3);
    CHECK(tl[0] == Token("d", Token::ID));
    CHECK(tl[1] == Token("p", Token::ID));
    CHECK(tl[2] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer Comment Test")
{
    Lexer l("d p ; # test test");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 3);
    CHECK(tl[0] == Token("d", Token::ID));
    CHECK(tl[1] == Token("p", Token::ID));
    CHECK(tl[2] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer Comment Test")
{
    Lexer l("# test test;\nd p ; # test test");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 3);
    CHECK(tl[0] == Token("d", Token::ID));
    CHECK(tl[1] == Token("p", Token::ID));
    CHECK(tl[2] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer practical Test1")
{
    Lexer l("log_format main '$remote_addr - $remote_user [$time_local] "
            "\"$request\" '\n    '$status $body_bytes_sent \"$http_referer\" "
            "'\n    '\"$http_user_agent\" \"$http_x_forwarded_for\"';");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 6);
    CHECK(tl[0] == Token("log_format", Token::ID));
    CHECK(tl[1] == Token("main", Token::ID));
    CHECK(tl[2] == Token("$remote_addr - $remote_user [$time_local] \"$request\" ", Token::STRING));
    CHECK(tl[3] == Token("$status $body_bytes_sent \"$http_referer\" ", Token::STRING));
    CHECK(tl[4] == Token("\"$http_user_agent\" \"$http_x_forwarded_for\"", Token::STRING));
    CHECK(tl[5] == Token(";", Token::SEMI));
}

TEST_CASE("Lexer practical Test2")
{
    Lexer l("gzip_disable \"MSIE [1-6]\\.\";");
    std::vector<Token> tl = l.get_token_list();
    REQUIRE(tl.size() == 3);
    CHECK(tl[0] == Token("gzip_disable", Token::ID));
    CHECK(tl[1] == Token("MSIE [1-6]\\.", Token::STRING));
    CHECK(tl[2] == Token(";", Token::SEMI));
}
#endif