#include "AST.hpp"
#include "BlockStatement.hpp"
#include "NotFound.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif
AST::AST() : _root()
{
}

AST::AST(std::vector<Token> tokens) : _tokens(tokens)
{
    _root.push_back(statement());
    while (true)
    {
        try
        {
            _root.push_back(statement());
        }
        catch (NotFound &e)
        {
            break;
        }
    }
}

Statement *AST::statement()
{
    std::stack<Token> buf;
    while (true)
    {
        try
        {
            Statement *node = try_simple_statement(buf);
            decide(buf);
            return node;
        }
        catch (NotFound &e)
        {
            backtrace(buf);
        }
        try
        {
            BlockStatement *node = try_block_statement(buf);
            decide(buf);
            return node;
        }
        catch (NotFound &e)
        {
            backtrace(buf);
        }
    }
    throw NotFound("statement not found");
}

AST::~AST()
{
    for (size_t i = 0; i < _root.size(); i++)
    {
        if (dynamic_cast<BlockStatement *>(_root[i]))
            delete dynamic_cast<BlockStatement *>(_root[i]);
        else
            delete _root[i];
    }
}

Statement *AST::try_simple_statement(std::stack<Token> &buf)
{
    return new Statement(directive(buf), parameters(buf));
}

BlockStatement *AST::try_block_statement(std::stack<Token> &buf)
{
    std::string directive_ = directive(buf);
    std::vector<std::string> parameters_ = parameters(buf);
    consume(Token::LCURLY, buf);
    std::vector<Statement *> statements;
    statements.push_back(statement());
    while (true)
    {
        try
        {
            statements.push_back(statement());
        }
        catch (NotFound &e)
        {
            break;
        }
    }
    consume(Token::RCURLY, buf);
    return new BlockStatement(directive_, parameters_, statements);
}

std::vector<std::string> AST::parameters(std::stack<Token> &buf)
{
    std::vector<std::string> children;
    while (true)
    {
        try
        {
            std::string node = parameter(buf);
            children.push_back(node);
        }
        catch (NotFound &e)
        {
            break;
        }
    }
    return children;
}

std::string AST::parameter(std::stack<Token> &buf)
{
    Token current_token = _tokens.front();
    if (current_token.get_type() == Token::ID)
        return consume(Token::ID, buf);
    else if (current_token.get_type() == Token::STRING)
        return consume(Token::STRING, buf);
    else if (current_token.get_type() == Token::INT)
        return consume(Token::INT, buf);
    throw NotFound("parameter not found");
}

std::string AST::directive(std::stack<Token> &buf)
{
    return consume(Token::ID, buf);
}

std::string AST::consume(Token::Type type, std::stack<Token> &buf)
{
    if (_tokens.empty())
        throw NotFound("Unexpected end of file");
    Token current_token = _tokens.front();
    if (current_token.get_type() != type)
        throw NotFound("Unexpected token");
    buf.push(_tokens[0]);
    _tokens.erase(_tokens.begin());
    return current_token.get_str();
}

void AST::backtrace(std::stack<Token> &buf)
{
    while (buf.size() != 0)
    {
        _tokens.insert(_tokens.begin(), buf.top());
        buf.pop();
    }
}

void AST::decide(std::stack<Token> &buf)
{
    while (!buf.empty())
        buf.pop();
}

void AST::print_tree()
{

    for (size_t i = 0; i < _root.size(); i++)
    {
        if (dynamic_cast<BlockStatement *>(_root[i]))
            std::cout << (*(dynamic_cast<BlockStatement *>(_root[i]))) << std::endl;
        else
            std::cout << (*(dynamic_cast<Statement *>(_root[i]))) << std::endl;
    }
}

std::vector<Statement *> AST::get_root() const
{
    return _root;
}

#ifdef UNIT_TEST
#include "Lexer.hpp"
TEST_CASE("AST: Simple")
{
    AST ast(Lexer("directive param;").get_token_list());
    CHECK(ast.get_root().size() == 1);
    CHECK(ast.get_root()[0]->get_directive() == "directive");
    CHECK(ast.get_root()[0]->get_params().size() == 1);
    CHECK(ast.get_root()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Multi Simple")
{
    AST ast(Lexer("directive param; directive param;").get_token_list());
    CHECK(ast.get_root().size() == 2);
    CHECK(ast.get_root()[0]->get_directive() == "directive");
    CHECK(ast.get_root()[0]->get_params().size() == 1);
    CHECK(ast.get_root()[0]->get_params()[0] == "param");
    CHECK(ast.get_root()[1]->get_directive() == "directive");
    CHECK(ast.get_root()[1]->get_params().size() == 1);
    CHECK(ast.get_root()[1]->get_params()[0] == "param");
}

TEST_CASE("AST: Multi Param Simple")
{
    AST ast(Lexer("directive p1 p2 p3 p4;").get_token_list());
    CHECK(ast.get_root().size() == 1);
    CHECK(ast.get_root()[0]->get_directive() == "directive");
    CHECK(ast.get_root()[0]->get_params().size() == 4);
    CHECK(ast.get_root()[0]->get_params()[0] == "p1");
    CHECK(ast.get_root()[0]->get_params()[1] == "p2");
    CHECK(ast.get_root()[0]->get_params()[2] == "p3");
    CHECK(ast.get_root()[0]->get_params()[3] == "p4");
}

TEST_CASE("AST: Block")
{
    AST ast(Lexer("Block { directive param;}").get_token_list());
    CHECK(ast.get_root().size() == 1);
    CHECK(dynamic_cast<BlockStatement *>(ast.get_root()[0]) != 0);
    BlockStatement *bs = dynamic_cast<BlockStatement *>(ast.get_root()[0]);
    CHECK(bs->get_directive() == "Block");
    CHECK(bs->get_params().size() == 0);
    CHECK(bs->get_child_statements().size() == 1);
    CHECK(bs->get_child_statements()[0]->get_directive() == "directive");
    CHECK(bs->get_child_statements()[0]->get_params().size() == 1);
    CHECK(bs->get_child_statements()[0]->get_params()[0] == "param");
}

TEST_CASE("AST: Block with param")
{
    AST ast(Lexer("Block param { directive param;}").get_token_list());
    CHECK(ast.get_root().size() == 1);
    CHECK(dynamic_cast<BlockStatement *>(ast.get_root()[0]) != 0);
    BlockStatement *bs = dynamic_cast<BlockStatement *>(ast.get_root()[0]);
    CHECK(bs->get_directive() == "Block");
    CHECK(bs->get_params().size() == 0);
    CHECK(bs->get_child_statements().size() == 1);
    CHECK(bs->get_child_statements()[0]->get_directive() == "directive");
    CHECK(bs->get_child_statements()[0]->get_params().size() == 1);
    CHECK(bs->get_child_statements()[0]->get_params()[0] == "param");
}

#endif