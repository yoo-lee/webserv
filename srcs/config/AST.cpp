#include "AST.hpp"
#include "BlockStatement.hpp"
#include "NotFound.hpp"
#include "SimpleStatement.hpp"
#include "Statement.hpp"
#include "Token.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::cout;
using std::endl;
using std::stack;
using std::string;
using std::vector;

AST::AST() : _root() {}

AST::AST(const vector<Token>& tokens) : _tokens(tokens)
{
    _root.push_back(statement());
    while (true) {
        try {
            _root.push_back(statement());
        } catch (NotFound& e) {
            break;
        }
    }
    if (_tokens.size() != 0)
        throw NotFound("AST: unexpected token " + _tokens[0].get_str());
}

Statement const* AST::statement()
{
    stack<Token> buf;
    try {
        Statement const* node = try_simple_statement(buf);
        decide(buf);
        return node;
    } catch (NotFound& e) {
        backtrace(buf);
    }
    try {
        BlockStatement const* node = try_block_statement(buf);
        decide(buf);
        return node;
    } catch (NotFound& e) {
        backtrace(buf);
    }
    throw NotFound("AST: statement not found");
}

AST::~AST()
{
    for (size_t i = 0; i < _root.size(); i++)
        delete _root[i];
}

Statement const* AST::try_simple_statement(stack<Token>& buf)
{
    string directive_ = directive(buf);
    string parameter_ = parameter(buf);
    vector<string> parameters_ = parameters(buf);
    parameters_.insert(parameters_.begin(), parameter_);
    consume(Token::SEMI, buf);
    return new SimpleStatement(directive_, parameters_);
}

BlockStatement const* AST::try_block_statement(stack<Token>& buf)
{
    string directive_ = directive(buf);
    vector<string> parameters_ = parameters(buf);
    consume(Token::LCURLY, buf);
    vector<Statement const*> statements;
    statements.push_back(statement());
    while (true) {
        try {
            statements.push_back(statement());
        } catch (NotFound& e) {
            break;
        }
    }
    try {
        consume(Token::RCURLY, buf);
    } catch (NotFound& nf) {
        for (size_t i = 0; i < statements.size(); i++)
            delete statements[i];
        throw NotFound("AST: RCURLY not found");
    }
    return new BlockStatement(directive_, parameters_, statements);
}

vector<string> AST::parameters(stack<Token>& buf)
{
    vector<string> children;
    while (true) {
        try {
            string node = parameter(buf);
            children.push_back(node);
        } catch (NotFound& e) {
            break;
        }
    }
    return children;
}

string AST::parameter(stack<Token>& buf)
{
    Token current_token = _tokens.front();
    if (current_token.get_type() == Token::ID)
        return consume(Token::ID, buf);
    else if (current_token.get_type() == Token::STRING)
        return consume(Token::STRING, buf);
    else if (current_token.get_type() == Token::INT)
        return consume(Token::INT, buf);
    throw NotFound("AST: parameter not found");
}

string AST::directive(stack<Token>& buf)
{
    return consume(Token::ID, buf);
}

string AST::consume(Token::Type type, stack<Token>& buf)
{
    if (_tokens.empty())
        throw NotFound("AST: Unexpected end of file");
    Token current_token = _tokens.front();
    if (current_token.get_type() != type)
        throw NotFound("AST: Unexpected token");
    buf.push(_tokens[0]);
    _tokens.erase(_tokens.begin());
    return current_token.get_str();
}

void AST::backtrace(stack<Token>& buf)
{
    while (buf.size() != 0) {
        _tokens.insert(_tokens.begin(), buf.top());
        buf.pop();
    }
}

void AST::decide(stack<Token>& buf)
{
    while (buf.size() != 0)
        buf.pop();
}

void AST::print_tree()
{

    for (size_t i = 0; i < _root.size(); i++) {
        if (dynamic_cast<BlockStatement const*>(_root[i]))
            std::cout << (*(dynamic_cast<BlockStatement const*>(_root[i]))) << std::endl;
        else
            std::cout << (*(dynamic_cast<SimpleStatement const*>(_root[i]))) << std::endl;
    }
}

vector<Statement const*> const& AST::get_root() const
{
    return _root;
}

Statement const* AST::operator[](string directive) const
{
    for (size_t i = 0; i < _root.size(); i++) {
        if (_root[i]->get_directive() == directive)
            return _root[i];
    }
    throw std::out_of_range("'" + directive + "' not found");
}

#ifdef UNIT_TEST
#include "Lexer.hpp"
TEST_CASE("AST: Simple")
{
    AST ast(Lexer("directive param;").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(ast.get_root()[0]->get_directive() == "directive");
    REQUIRE(ast.get_root()[0]->get_params().size() == 1);
    REQUIRE(ast.get_root()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Simple before comment")
{
    AST ast(Lexer("directive param; # comment").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(ast.get_root()[0]->get_directive() == "directive");
    REQUIRE(ast.get_root()[0]->get_params().size() == 1);
    REQUIRE(ast.get_root()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Simple after comment")
{
    AST ast(Lexer("# comment \n directive param;").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(ast.get_root()[0]->get_directive() == "directive");
    REQUIRE(ast.get_root()[0]->get_params().size() == 1);
    REQUIRE(ast.get_root()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Simple between comment")
{
    AST ast(Lexer("# comment \n directive param; # comment").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(ast.get_root()[0]->get_directive() == "directive");
    REQUIRE(ast.get_root()[0]->get_params().size() == 1);
    REQUIRE(ast.get_root()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Multi Simple")
{
    AST ast(Lexer("directive param; directive param;").get_token_list());
    REQUIRE(ast.get_root().size() == 2);
    REQUIRE(ast.get_root()[0]->get_directive() == "directive");
    REQUIRE(ast.get_root()[0]->get_params().size() == 1);
    REQUIRE(ast.get_root()[0]->get_params()[0] == "param");
    REQUIRE(ast.get_root()[1]->get_directive() == "directive");
    REQUIRE(ast.get_root()[1]->get_params().size() == 1);
    REQUIRE(ast.get_root()[1]->get_params()[0] == "param");
}

TEST_CASE("AST: Multi Param Simple")
{
    AST ast(Lexer("directive p1 p2 p3 p4;").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) == 0);
    REQUIRE(ast.get_root()[0]->get_directive() == "directive");
    REQUIRE(ast.get_root()[0]->get_params().size() == 4);
    REQUIRE(ast.get_root()[0]->get_params()[0] == "p1");
    REQUIRE(ast.get_root()[0]->get_params()[1] == "p2");
    REQUIRE(ast.get_root()[0]->get_params()[2] == "p3");
    REQUIRE(ast.get_root()[0]->get_params()[3] == "p4");
}

TEST_CASE("AST: Block")
{
    AST ast(Lexer("Block { directive param;}").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) != 0);
    BlockStatement const* bs = dynamic_cast<BlockStatement const*>(ast.get_root()[0]);
    REQUIRE(bs->get_directive() == "Block");
    REQUIRE(bs->get_params().size() == 0);
    REQUIRE(bs->get_children().size() == 1);
    REQUIRE(bs->get_children()[0]->get_directive() == "directive");
    REQUIRE(bs->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs->get_children()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Block before comment")
{
    AST ast(Lexer("Block { directive param;} # comment").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) != 0);
    BlockStatement const* bs = dynamic_cast<BlockStatement const*>(ast.get_root()[0]);
    REQUIRE(bs->get_directive() == "Block");
    REQUIRE(bs->get_params().size() == 0);
    REQUIRE(bs->get_children().size() == 1);
    REQUIRE(bs->get_children()[0]->get_directive() == "directive");
    REQUIRE(bs->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs->get_children()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Block after comment")
{
    AST ast(Lexer("# comment \nBlock { directive param;}").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) != 0);
    BlockStatement const* bs = dynamic_cast<BlockStatement const*>(ast.get_root()[0]);
    REQUIRE(bs->get_directive() == "Block");
    REQUIRE(bs->get_params().size() == 0);
    REQUIRE(bs->get_children().size() == 1);
    REQUIRE(bs->get_children()[0]->get_directive() == "directive");
    REQUIRE(bs->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs->get_children()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Block with param")
{
    AST ast(Lexer("Block param { directive param;}").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) != 0);
    BlockStatement const* bs = dynamic_cast<BlockStatement const*>(ast.get_root()[0]);
    REQUIRE(bs->get_directive() == "Block");
    REQUIRE(bs->get_params().size() == 1);
    REQUIRE(bs->get_params()[0] == "param");
    REQUIRE(bs->get_children().size() == 1);
    REQUIRE(bs->get_children()[0]->get_directive() == "directive");
    REQUIRE(bs->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs->get_children()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Multi Block")
{
    AST ast(Lexer("block1{a b;}block2{a b;}").get_token_list());
    REQUIRE(ast.get_root().size() == 2);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) != 0);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[1]) != 0);

    BlockStatement const* bs1 = dynamic_cast<BlockStatement const*>(ast.get_root()[0]);
    REQUIRE(bs1->get_directive() == "block1");
    REQUIRE(bs1->get_params().size() == 0);
    REQUIRE(bs1->get_children().size() == 1);
    REQUIRE(bs1->get_children()[0]->get_directive() == "a");
    REQUIRE(bs1->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs1->get_children()[0]->get_params()[0] == "b");

    BlockStatement const* bs2 = dynamic_cast<BlockStatement const*>(ast.get_root()[1]);
    REQUIRE(bs2->get_directive() == "block2");
    REQUIRE(bs2->get_params().size() == 0);
    REQUIRE(bs2->get_children().size() == 1);
    REQUIRE(bs2->get_children()[0]->get_directive() == "a");
    REQUIRE(bs2->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs2->get_children()[0]->get_params()[0] == "b");
}

TEST_CASE("AST: Nested block statement")
{
    AST ast(Lexer("parent{child{a b;}}").get_token_list());
    REQUIRE(ast.get_root().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(ast.get_root()[0]) != 0);
    BlockStatement const* bs1 = dynamic_cast<BlockStatement const*>(ast.get_root()[0]);
    REQUIRE(bs1->get_directive() == "parent");
    REQUIRE(bs1->get_params().size() == 0);
    REQUIRE(bs1->get_children().size() == 1);
    REQUIRE(dynamic_cast<BlockStatement const*>(bs1->get_children()[0]) != 0);
    BlockStatement const* bs2 = dynamic_cast<BlockStatement const*>(bs1->get_children()[0]);
    REQUIRE(bs2->get_directive() == "child");
    REQUIRE(bs2->get_params().size() == 0);
    REQUIRE(bs2->get_children().size() == 1);
    REQUIRE(bs2->get_children()[0]->get_directive() == "a");
    REQUIRE(bs2->get_children()[0]->get_params().size() == 1);
    REQUIRE(bs2->get_children()[0]->get_params()[0] == "b");
}

TEST_CASE("AST: nginx.conf")
{
    AST ast(Lexer("events {\n"
                  "    worker_connections 1024;\n"
                  "}\n"
                  "\n"
                  "http {\n"
                  "    server {\n"
                  "        listen 80;\n"
                  "        server_name example.com;\n"
                  "        root /var/www/example.com;\n"
                  "\n"
                  "        location / {\n"
                  "            try_files $uri $uri/ =404;\n"
                  "        }\n"
                  "    }\n"
                  "}\n")
                .get_token_list());
    REQUIRE(ast.get_root().size() == 2);
}
#include <fstream>
#include <iostream>
TEST_CASE("AST: subject.nginx.conf")
{
    AST ast(Lexer("http {\n"
                  "    client_max_body_size 10;\n"
                  "    server {\n"
                  "        listen 80 default_server;\n"
                  "        server_name example.com; # not required\n"
                  "        location PATH {\n"
                  "            autoindex on;\n"
                  "            error_page 500 ./error.html;\n"
                  "            index ./index.html;\n"
                  "            limit_except POST {\n"
                  "                deny all;\n"
                  "            }\n"
                  "        }\n"
                  "        location PATH {\n"
                  "            root ./wordpress;\n"
                  "        }\n"
                  "        location ~* \\.php$ {\n"
                  "            cgi_pass /bin/php-cgi;\n"
                  "        }\n"
                  "    }\n"
                  "}\n")
                .get_token_list());
    REQUIRE(ast.get_root().size() == 1);
}

/** BAD TEST CASE **/

TEST_CASE("AST: Nested non lcurly")
{
    CHECK_THROWS_AS(AST(Lexer("parent{child{a b;}").get_token_list()), NotFound);
}

TEST_CASE("AST: Nested non rcurly")
{
    CHECK_THROWS_AS(AST(Lexer("parent{child{a b;}").get_token_list()), NotFound);
}

TEST_CASE("AST: random string")
{
    CHECK_THROWS_AS(AST(Lexer("j;a903]+{kfjda;}").get_token_list()), NotFound);
}

TEST_CASE("AST: Simple statement with no param")
{
    CHECK_THROWS_AS(AST(Lexer("Block param { directive ;}").get_token_list()), NotFound);
}

#endif
