#include "AST.hpp"
#include "NonTerminalASTNode.hpp"
#include "NotFound.hpp"
#include "TerminalASTNode.hpp"
#include "Token.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif
AST::AST() : _root(NULL), _buf()
{
}

AST::AST(std::vector<Token> tokens) : _tokens(tokens), _buf(), _root(program())
{
    NonTerminalASTNode *n = dynamic_cast<NonTerminalASTNode *>(_root->get_children()[0]);
}

NonTerminalASTNode *AST::program()
{
    std::vector<ASTNode *> children;
    children.push_back(statement());
    while (true)
    {
        try
        {
            children.push_back(statement());
        }
        catch (NotFound &e)
        {
            break;
        }
    }
    return new NonTerminalASTNode(ASTNode::PROGRAM, children);
}

NonTerminalASTNode *AST::statement()
{
    try
    {
        std::vector<ASTNode *> children;
        NonTerminalASTNode *node = try_simple_statement();
        decide();
        children.push_back(node);
        return new NonTerminalASTNode(ASTNode::STATEMENT, children);
    }
    catch (NotFound &e)
    {
        backtrace();
    }
    try
    {
        std::vector<ASTNode *> children;
        NonTerminalASTNode *node = try_block_statement();
        decide();
        children.push_back(node);
        return new NonTerminalASTNode(ASTNode::STATEMENT, children);
    }
    catch (NotFound &e)
    {
        backtrace();
    }
    throw NotFound("statement not found");
}

NonTerminalASTNode *AST::try_simple_statement()
{
    std::vector<ASTNode *> children = std::vector<ASTNode *>();
    children.push_back(directive());
    ASTNode *parameters_node = parameters();
    children.push_back(parameters_node);
    children.push_back(consume(Token::SEMI));

    return new NonTerminalASTNode(ASTNode::SIMPLE_STATEMENT, children);
}

NonTerminalASTNode *AST::try_block_statement()
{
    std::vector<ASTNode *> children = std::vector<ASTNode *>();
    children.push_back(directive());
    children.push_back(consume(Token::LCURLY));
    children.push_back(statement());
    while (true)
    {
        try
        {
            children.push_back(statement());
        }
        catch (NotFound &e)
        {
            break;
        }
    }
    children.push_back(consume(Token::RCURLY));
    return new NonTerminalASTNode(ASTNode::BLOCK_STATEMENT, children);
}

NonTerminalASTNode *AST::parameters()
{
    std::vector<ASTNode *> children;
    children.push_back(parameter());
    while (true)
    {
        try
        {
            ASTNode *node = parameter();
            children.push_back(node);
        }
        catch (NotFound &e)
        {
            break;
        }
    }
    return new NonTerminalASTNode(ASTNode::PARAMETERS, children);
}

NonTerminalASTNode *AST::parameter()
{
    Token current_token = _tokens.front();
    if (current_token.get_type() == Token::ID)
        return new NonTerminalASTNode(ASTNode::PARAMETER, consume(Token::ID));
    else if (current_token.get_type() == Token::STRING)
        return new NonTerminalASTNode(ASTNode::PARAMETER, consume(Token::STRING));
    else if (current_token.get_type() == Token::INT)
        return new NonTerminalASTNode(ASTNode::PARAMETER, consume(Token::INT));
    throw NotFound("parameter not found");
}

NonTerminalASTNode *AST::directive()
{
    return new NonTerminalASTNode(ASTNode::DIRECTIVE, consume(Token::ID));
}

TerminalASTNode *AST::consume(Token::Type type)
{
    if (_tokens.empty())
        throw NotFound("Unexpected end of file");
    Token current_token = _tokens.front();
    if (current_token.get_type() != type)
        throw NotFound("Unexpected token");
    _buf.push(_tokens[0]);
    _tokens.erase(_tokens.begin());
    return new TerminalASTNode(ASTNode::to_ast_node_type(type), current_token.get_str());
}

void AST::backtrace()
{
    while (!_buf.empty())
    {
        _tokens.insert(_tokens.begin(), _buf.front());
        _buf.pop();
    }
}

void AST::decide()
{
    while (!_buf.empty())
        _buf.pop();
}

void AST::print_tree()
{
    _root->print("");
}

bool AST::operator==(const AST &other) const
{
    return *_root == *other._root;
}

bool AST::operator!=(const AST &other) const
{
    return !(*this == other);
}

NonTerminalASTNode *AST::get_root() const
{
    return dynamic_cast<NonTerminalASTNode *>(_root);
}

#ifdef UNIT_TEST
#include "ASTNode.hpp"
#include "Lexer.hpp"

// テストケースに失敗した場合にどこで失敗したかを
// わかりやすくするためにわざと共通化していない
TEST_CASE("AST::Simple")
{
    TerminalASTNode *id;
    AST ast(Lexer("simple statement;").get_token_list());
    ast.print_tree();
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

TEST_CASE("AST: Nested Block")
{
    std::cout << " x" << std::endl;
    AST ast(Lexer("key1 { key2 { key3 value; } }").get_token_list());
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
    REQUIRE(block_statement->get_children().size() == 2);

    NonTerminalASTNode *directive = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    TerminalASTNode *id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key1");

    block_statement = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[1]);
    REQUIRE(block_statement != 0);
    REQUIRE(block_statement->get_type() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block_statement->get_children().size() == 2);

    directive = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key2");

    block_statement = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[1]);
    REQUIRE(block_statement != 0);
    REQUIRE(block_statement->get_type() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block_statement->get_children().size() == 2);

    directive = dynamic_cast<NonTerminalASTNode *>(block_statement->get_children()[0]);
    REQUIRE(directive != 0);
    REQUIRE(directive->get_type() == ASTNode::DIRECTIVE);
    REQUIRE(directive->get_children().size() == 1);

    id = dynamic_cast<TerminalASTNode *>(directive->get_children()[0]);
    REQUIRE(id != 0);
    REQUIRE(id->get_type() == ASTNode::ID);
    REQUIRE(id->get_value() == "key3");

    TerminalASTNode *value = dynamic_cast<TerminalASTNode *>(block_statement->get_children()[1]);
    REQUIRE(value != 0);
    REQUIRE(value->get_type() == ASTNode::STRING);
    REQUIRE(value->get_value() == "value");
}

#endif