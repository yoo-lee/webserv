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
    AST ast1(Lexer("block param { simple statement; }").get_token_list());
    ast1.print_tree();
}
/* --------------内部構造を大きく変えたので動きません--------------
// テストケースに失敗した場合にどこで失敗したかを
// わかりやすくするためにわざと共通化していない
TEST_CASE("AST::Simple")
{
    TerminalASTNode *id;
    AST ast(Lexer("simple statement;").get_token_list());
    // ast.print_tree();
    NonTerminalASTNode root = ast.get_root();
    REQUIRE(root.get_type() == ASTNode::PROGRAM);
    REQUIRE(root.get_children().size() == 1);

    NonTerminalASTNode *statement = dynamic_cast<NonTerminalASTNode *>(root.get_children()[0]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    NonTerminalASTNode *simple_statement = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(simple_statement != 0);
    REQUIRE(simple_statement->get_type() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement->get_children().size() == 3);

    NonTerminalASTNode *directive = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "simple");

    NonTerminalASTNode *parameters = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[1]);
    REQUIRE(parameters != 0);
    REQUIRE(parameters->get_type() == ASTNode::PARAMETERS);
    REQUIRE(parameters->get_children().size() == 1);
    NonTerminalASTNode *parameter = dynamic_cast<NonTerminalASTNode *>(parameters->get_children()[0]);
    REQUIRE(parameter != 0);
    REQUIRE(parameter->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter->get_children().size() == 1);
    id = dynamic_cast<TerminalASTNode *>(parameter->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "statement");

    TerminalASTNode *semi = dynamic_cast<TerminalASTNode *>(simple_statement->get_children()[2]);
    REQUIRE(semi != 0);
    REQUIRE(semi->get_type() == ASTNode::SEMI);
    REQUIRE(semi->get_value() == ";");
}

TEST_CASE("AST::Block")
{
    TerminalASTNode *id;
    AST ast = AST(Lexer("block { simple statement; }").get_token_list());
    NonTerminalASTNode root = ast.get_root();
    REQUIRE(root.get_type() == ASTNode::PROGRAM);
    REQUIRE(root.get_children().size() == 1);

    NonTerminalASTNode *statement = dynamic_cast<NonTerminalASTNode *>(root.get_children()[0]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    NonTerminalASTNode *block = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(block != 0);
    REQUIRE(block->get_type() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block->get_children().size() == 4);

    TerminalASTNode *lcurly = dynamic_cast<TerminalASTNode *>(block->get_children()[1]);
    REQUIRE(lcurly != 0);
    REQUIRE(lcurly->get_type() == ASTNode::LCURLY);
    REQUIRE(lcurly->get_value() == "{");

    NonTerminalASTNode *statement_in_block = dynamic_cast<NonTerminalASTNode *>(block->get_children()[2]);
    REQUIRE(statement_in_block != 0);
    REQUIRE(statement_in_block->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement_in_block->get_children().size() == 1);

    NonTerminalASTNode *simple_statement = dynamic_cast<NonTerminalASTNode *>(statement_in_block->get_children()[0]);
    REQUIRE(simple_statement != 0);
    REQUIRE(simple_statement->get_type() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement->get_children().size() == 3);

    NonTerminalASTNode *directive = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);
    id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "simple");

    NonTerminalASTNode *parameters = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[1]);
    REQUIRE(parameters != 0);
    REQUIRE(parameters->get_type() == ASTNode::PARAMETERS);
    REQUIRE(parameters->get_children().size() == 1);
    NonTerminalASTNode *parameter = dynamic_cast<NonTerminalASTNode *>(parameters->get_children()[0]);
    REQUIRE(parameter != 0);
    REQUIRE(parameter->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter->get_children().size() == 1);
    id = dynamic_cast<TerminalASTNode *>(parameter->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "statement");

    TerminalASTNode *semi = dynamic_cast<TerminalASTNode *>(simple_statement->get_children()[2]);
    REQUIRE(semi != 0);
    REQUIRE(semi->get_type() == ASTNode::SEMI);
    REQUIRE(semi->get_value() == ";");

    TerminalASTNode *rcurly = dynamic_cast<TerminalASTNode *>(block->get_children()[3]);
    REQUIRE(rcurly != 0);
    REQUIRE(rcurly->get_type() == ASTNode::RCURLY);
    REQUIRE(rcurly->get_value() == "}");
}

TEST_CASE("AST::Multi statement in block statement")
{
    TerminalASTNode *id;
    AST ast = AST(Lexer("block { simple1 statement1; simple2 statement2; }").get_token_list());
    NonTerminalASTNode root = ast.get_root();
    REQUIRE(root.get_type() == ASTNode::PROGRAM);
    REQUIRE(root.get_children().size() == 1);

    NonTerminalASTNode *statement = dynamic_cast<NonTerminalASTNode *>(root.get_children()[0]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    NonTerminalASTNode *block = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(block != 0);
    REQUIRE(block->get_type() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block->get_children().size() == 5);

    TerminalASTNode *lcurly = dynamic_cast<TerminalASTNode *>(block->get_children()[1]);
    REQUIRE(lcurly != 0);
    REQUIRE(lcurly->get_type() == ASTNode::LCURLY);
    REQUIRE(lcurly->get_value() == "{");

    NonTerminalASTNode *statement_in_block1 = dynamic_cast<NonTerminalASTNode *>(block->get_children()[2]);
    REQUIRE(statement_in_block1 != 0);
    REQUIRE(statement_in_block1->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement_in_block1->get_children().size() == 1);

    NonTerminalASTNode *simple_statement1 = dynamic_cast<NonTerminalASTNode *>(statement_in_block1->get_children()[0]);
    REQUIRE(simple_statement1 != 0);
    REQUIRE(simple_statement1->get_type() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement1->get_children().size() == 3);

    NonTerminalASTNode *directive1 = dynamic_cast<NonTerminalASTNode *>(simple_statement1->get_children()[0]);
    REQUIRE(directive1 != 0);
    REQUIRE(directive1->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive1->get_children().size() == 1);
    id = dynamic_cast<TerminalASTNode *>(directive1->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "simple1");

    NonTerminalASTNode *parameters1 = dynamic_cast<NonTerminalASTNode *>(simple_statement1->get_children()[1]);
    REQUIRE(parameters1 != 0);
    REQUIRE(parameters1->get_type() == ASTNode::PARAMETERS);
    REQUIRE(parameters1->get_children().size() == 1);
    NonTerminalASTNode *parameter1 = dynamic_cast<NonTerminalASTNode *>(parameters1->get_children()[0]);
    REQUIRE(parameter1 != 0);
    REQUIRE(parameter1->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter1->get_children().size() == 1);
    id = dynamic_cast<TerminalASTNode *>(parameter1->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "statement1");

    TerminalASTNode *semi = dynamic_cast<TerminalASTNode *>(simple_statement1->get_children()[2]);
    REQUIRE(semi != 0);
    REQUIRE(semi->get_type() == ASTNode::SEMI);
    REQUIRE(semi->get_value() == ";");

    NonTerminalASTNode *statement_in_block2 = dynamic_cast<NonTerminalASTNode *>(block->get_children()[3]);
    REQUIRE(statement_in_block2 != 0);
    REQUIRE(statement_in_block2->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement_in_block2->get_children().size() == 1);

    NonTerminalASTNode *simple_statement2 = dynamic_cast<NonTerminalASTNode *>(statement_in_block2->get_children()[0]);
    REQUIRE(simple_statement2 != 0);
    REQUIRE(simple_statement2->get_type() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement2->get_children().size() == 3);

    NonTerminalASTNode *directive2 = dynamic_cast<NonTerminalASTNode *>(simple_statement2->get_children()[0]);
    REQUIRE(directive2 != 0);
    REQUIRE(directive2->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive2->get_children().size() == 1);
    id = dynamic_cast<TerminalASTNode *>(directive2->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "simple2");

    NonTerminalASTNode *parameters2 = dynamic_cast<NonTerminalASTNode *>(simple_statement2->get_children()[1]);
    REQUIRE(parameters2 != 0);
    REQUIRE(parameters2->get_type() == ASTNode::PARAMETERS);
    REQUIRE(parameters2->get_children().size() == 1);
    NonTerminalASTNode *parameter2 = dynamic_cast<NonTerminalASTNode *>(parameters2->get_children()[0]);
    REQUIRE(parameter2 != 0);
    REQUIRE(parameter2->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter2->get_children().size() == 1);

    id = dynamic_cast<TerminalASTNode *>(parameter2->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "statement2");

    semi = dynamic_cast<TerminalASTNode *>(simple_statement2->get_children()[2]);
    REQUIRE(semi != 0);
    REQUIRE(semi->get_type() == ASTNode::SEMI);
    REQUIRE(semi->get_value() == ";");

    TerminalASTNode *rcurly = dynamic_cast<TerminalASTNode *>(block->get_children()[4]);
    REQUIRE(rcurly != 0);
    REQUIRE(rcurly->get_type() == ASTNode::RCURLY);
    REQUIRE(rcurly->get_value() == "}");
}

TEST_CASE("AST: Nested block")
{
    AST ast(Lexer("key1 { key2 { key3 value; } }").get_token_list());
    // ast.print_tree();
    NonTerminalASTNode *root = ast.get_root();
    REQUIRE(root != 0);
    REQUIRE(root->get_type() == ASTNode::PROGRAM);
    REQUIRE(root->get_children().size() == 1);

    NonTerminalASTNode *statement = dynamic_cast<NonTerminalASTNode *>(root->get_children()[0]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    NonTerminalASTNode *block_statement = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(block_statement != 0);
    REQUIRE(block_statement->get_type() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block_statement->get_children().size() == 4);

    NonTerminalASTNode *directive = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    TerminalASTNode *id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key1");

    TerminalASTNode *lcurly = dynamic_cast<TerminalASTNode *>(block_statement->get_children()[1]);
    REQUIRE(lcurly != 0);
    REQUIRE(lcurly->get_type() == ASTNode::LCURLY);
    REQUIRE(lcurly->get_value() == "{");

    TerminalASTNode *rcurly = dynamic_cast<TerminalASTNode *>(block_statement->get_children()[3]);
    REQUIRE(rcurly != 0);
    REQUIRE(rcurly->get_type() == ASTNode::RCURLY);
    REQUIRE(rcurly->get_value() == "}");

    statement = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[2]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    block_statement = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(block_statement != 0);
    REQUIRE(block_statement->get_type() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block_statement->get_children().size() == 4);

    directive = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key2");

    lcurly = dynamic_cast<TerminalASTNode *>(block_statement->get_children()[1]);
    REQUIRE(lcurly != 0);
    REQUIRE(lcurly->get_type() == ASTNode::LCURLY);
    REQUIRE(lcurly->get_value() == "{");

    rcurly = dynamic_cast<TerminalASTNode *>(block_statement->get_children()[3]);
    REQUIRE(rcurly != 0);
    REQUIRE(rcurly->get_type() == ASTNode::RCURLY);
    REQUIRE(rcurly->get_value() == "}");

    statement = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[2]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    NonTerminalASTNode *simple_statement = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(simple_statement != 0);
    REQUIRE(simple_statement->get_type() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement->get_children().size() == 3);

    directive = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key3");

    NonTerminalASTNode *parameters = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[1]);
    REQUIRE(parameters != 0);
    REQUIRE(parameters->get_type() == ASTNode::PARAMETERS);
    REQUIRE(parameters->get_children().size() == 1);

    NonTerminalASTNode *parameter = dynamic_cast<NonTerminalASTNode *>(parameters->get_children()[0]);
    REQUIRE(parameter != 0);
    REQUIRE(parameter->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter->get_children().size() == 1);

    TerminalASTNode *value = dynamic_cast<TerminalASTNode *>(parameter->get_children()[0]);
    REQUIRE(value != 0);
    REQUIRE(value->get_type() == ASTNode::ID);
    REQUIRE(value->get_value() == "value");

    TerminalASTNode *semi = dynamic_cast<TerminalASTNode *>(simple_statement->get_children()[2]);
    REQUIRE(semi != 0);
    REQUIRE(semi->get_type() == ASTNode::SEMI);
    REQUIRE(semi->get_value() == ";");
}

TEST_CASE("AST: Multi parameter simple statement")
{
    AST ast(Lexer("key param1 param2;").get_token_list());
    NonTerminalASTNode root = ast.get_root();
    REQUIRE(root.get_type() == ASTNode::PROGRAM);
    REQUIRE(root.get_children().size() == 1);

    NonTerminalASTNode *statement = dynamic_cast<NonTerminalASTNode *>(root.get_children()[0]);
    REQUIRE(statement != 0);
    REQUIRE(statement->get_type() == ASTNode::STATEMENT);
    REQUIRE(statement->get_children().size() == 1);

    NonTerminalASTNode *simple_statement = dynamic_cast<NonTerminalASTNode *>(statement->get_children()[0]);
    REQUIRE(simple_statement != 0);
    REQUIRE(simple_statement->get_type() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement->get_children().size() == 3);

    NonTerminalASTNode *directive = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    TerminalASTNode *id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key");

    NonTerminalASTNode *parameters = dynamic_cast<NonTerminalASTNode *>(simple_statement->get_children()[1]);
    REQUIRE(parameters != 0);
    REQUIRE(parameters->get_type() == ASTNode::PARAMETERS);
    REQUIRE(parameters->get_children().size() == 2);

    NonTerminalASTNode *parameter = dynamic_cast<NonTerminalASTNode *>(parameters->get_children()[0]);
    REQUIRE(parameter != 0);
    REQUIRE(parameter->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter->get_children().size() == 1);

    TerminalASTNode *param1 = dynamic_cast<TerminalASTNode *>(parameter->get_children()[0]);
    REQUIRE(param1 != 0);
    REQUIRE(param1->get_type() == ASTNode::ID);
    REQUIRE(param1->get_value() == "param1");

    parameter = dynamic_cast<NonTerminalASTNode *>(parameters->get_children()[1]);
    REQUIRE(parameter != 0);
    REQUIRE(parameter->get_type() == ASTNode::PARAMETER);
    REQUIRE(parameter->get_children().size() == 1);

    TerminalASTNode *param2 = dynamic_cast<TerminalASTNode *>(parameter->get_children()[0]);
    REQUIRE(param2 != 0);
    REQUIRE(param2->get_type() == ASTNode::ID);
    REQUIRE(param2->get_value() == "param2");

    TerminalASTNode *semi = dynamic_cast<TerminalASTNode *>(simple_statement->get_children()[2]);
    REQUIRE(semi != 0);
    REQUIRE(semi->get_type() == ASTNode::SEMI);
    REQUIRE(semi->get_value() == ";");
}
 */
#endif