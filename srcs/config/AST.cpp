#include "AST.hpp"
#include "NotFound.hpp"
#include "Token.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif
AST::AST() : _root(NULL), _buf()
{
}

AST::AST(std::vector<Token> tokens) : _tokens(tokens), _root(NULL), _buf()
{
    _root = program();
}

ASTNode *AST::program()
{
    std::vector<ASTNode *> children;
    children.push_back(statement());
    while (true)
    {
        try
        {
            children.push_back(statement());
        }
        catch (not_found &e)
        {
            break;
        }
    }
    return new ASTNode(ASTNode::PROGRAM, children);
}

ASTNode *AST::statement()
{
    try
    {
        ASTNode *n = try_simple_statement();
        decide();
        return n;
    }
    catch (not_found &e)
    {
        backtrace();
    }
    try
    {
        ASTNode *n = try_block_statement();
        decide();
        return n;
    }
    catch (not_found &e)
    {
        backtrace();
    }
    throw not_found("statement not found");
}

ASTNode *AST::try_simple_statement()
{
    std::vector<ASTNode *> children = std::vector<ASTNode *>();
    children.push_back(directive());
    ASTNode *parameters_node = parameters();
    children.push_back(parameters_node);
    children.push_back(new ASTNode(consume(Token::SEMI)));
    std::vector<ASTNode *> buf = std::vector<ASTNode *>();
    buf.push_back(new ASTNode(ASTNode::SIMPLE_STATEMENT, children));
    return new ASTNode(ASTNode::STATEMENT, buf);
}

ASTNode *AST::try_block_statement()
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
        catch (not_found &e)
        {
            break;
        }
    }
    children.push_back(consume(Token::RCURLY));
    std::vector<ASTNode *> buf = std::vector<ASTNode *>();
    buf.push_back(new ASTNode(ASTNode::BLOCK_STATEMENT, children));
    return new ASTNode(ASTNode::STATEMENT, buf);
}

ASTNode *AST::parameters()
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
        catch (not_found &e)
        {
            break;
        }
    }
    return new ASTNode(ASTNode::PARAMETERS, children);
}

ASTNode *AST::parameter()
{
    Token current_token = _tokens.front();
    if (current_token.getType() == Token::ID)
        return new ASTNode(ASTNode::PARAMETER, consume(Token::ID));
    else if (current_token.getType() == Token::STRING)
        return new ASTNode(ASTNode::PARAMETER, consume(Token::STRING));
    else if (current_token.getType() == Token::INT)
        return new ASTNode(ASTNode::PARAMETER, consume(Token::INT));
    throw not_found("parameter not found");
}

ASTNode *AST::directive()
{
    return new ASTNode(ASTNode::DIRECTIVE, consume(Token::ID));
}

ASTNode *AST::consume(Token::Type type)
{
    if (_tokens.empty())
        throw not_found("Unexpected end of file");
    Token current_token = _tokens.front();
    if (current_token.getType() != type)
        throw not_found("Unexpected token");
    _buf.push(_tokens[0]);
    _tokens.erase(_tokens.begin());
    return new ASTNode(ASTNode::TokenTypeToASTNodeType(type), current_token.getStr());
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

ASTNode *AST::getRoot() const
{
    return _root;
}

#ifdef UNIT_TEST
#include "ASTNode.hpp"
#include "Lexer.hpp"
// SUBCASE("Empty")
// {
//     std::vector<Token> tokens = std::vector<Token>();
//     CHECK_THROWS_AS(AST(tokens), not_found);
// }
TEST_CASE("AST::Simple")
{
    AST ast(Lexer("simple statement;").getTokenList());
    ASTNode root = ast.getRoot();
    REQUIRE(root.getType() == ASTNode::PROGRAM);
    REQUIRE(root.getChildren().size() == 1);

    ASTNode *statement = root.getChildren()[0];
    REQUIRE(statement->getType() == ASTNode::STATEMENT);
    REQUIRE(statement->getChildren().size() == 1);

    ASTNode *simple_statement = statement->getChildren()[0];
    REQUIRE(simple_statement->getType() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement->getChildren().size() == 3);

    ASTNode *directive = simple_statement->getChildren()[0];
    REQUIRE(directive->getType() == ASTNode::DIRECTIVE);
    REQUIRE(directive->getChildren().size() == 1);
    REQUIRE(directive->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(directive->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(directive->getChildren()[0]->getValue() == "simple");

    ASTNode *parameters = simple_statement->getChildren()[1];
    REQUIRE(parameters->getType() == ASTNode::PARAMETERS);
    REQUIRE(parameters->getChildren().size() == 1);
    ASTNode *parameter = parameters->getChildren()[0];
    REQUIRE(parameter->getType() == ASTNode::PARAMETER);
    REQUIRE(parameter->getChildren().size() == 1);
    REQUIRE(parameter->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(parameter->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(parameter->getChildren()[0]->getValue() == "statement");

    ASTNode *semi = simple_statement->getChildren()[2];
    REQUIRE(semi->getType() == ASTNode::SEMI);
    REQUIRE(semi->getChildren().size() == 0);
    REQUIRE(semi->getValue() == ";");
}

TEST_CASE("AST::Block")
{
    AST ast = AST(Lexer("block { simple statement; }").getTokenList());
    ASTNode root = ast.getRoot();
    REQUIRE(root.getType() == ASTNode::PROGRAM);
    REQUIRE(root.getChildren().size() == 1);

    ASTNode *statement = root.getChildren()[0];
    REQUIRE(statement->getType() == ASTNode::STATEMENT);
    REQUIRE(statement->getChildren().size() == 1);

    ASTNode *block = statement->getChildren()[0];
    REQUIRE(block->getType() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block->getChildren().size() == 4);

    ASTNode *lcurly = block->getChildren()[1];
    REQUIRE(lcurly->getType() == ASTNode::LCURLY);
    REQUIRE(lcurly->getChildren().size() == 0);
    REQUIRE(lcurly->getValue() == "{");

    ASTNode *statement_in_block = block->getChildren()[2];
    REQUIRE(statement_in_block->getType() == ASTNode::STATEMENT);
    REQUIRE(statement_in_block->getChildren().size() == 1);

    ASTNode *simple_statement = statement_in_block->getChildren()[0];
    REQUIRE(simple_statement->getType() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement->getChildren().size() == 3);

    ASTNode *directive = simple_statement->getChildren()[0];
    REQUIRE(directive->getType() == ASTNode::DIRECTIVE);
    REQUIRE(directive->getChildren().size() == 1);
    REQUIRE(directive->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(directive->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(directive->getChildren()[0]->getValue() == "simple");

    ASTNode *parameters = simple_statement->getChildren()[1];
    REQUIRE(parameters->getType() == ASTNode::PARAMETERS);
    REQUIRE(parameters->getChildren().size() == 1);
    ASTNode *parameter = parameters->getChildren()[0];
    REQUIRE(parameter->getType() == ASTNode::PARAMETER);
    REQUIRE(parameter->getChildren().size() == 1);
    REQUIRE(parameter->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(parameter->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(parameter->getChildren()[0]->getValue() == "statement");

    ASTNode *semi = simple_statement->getChildren()[2];
    REQUIRE(semi->getType() == ASTNode::SEMI);
    REQUIRE(semi->getChildren().size() == 0);
    REQUIRE(semi->getValue() == ";");

    ASTNode *rcurly = block->getChildren()[3];
    REQUIRE(rcurly->getType() == ASTNode::RCURLY);
    REQUIRE(rcurly->getChildren().size() == 0);
    REQUIRE(rcurly->getValue() == "}");
}

TEST_CASE("AST::Multi statement in block statement")
{
    AST ast = AST(Lexer("block { simple1 statement1; simple2 statement2; }").getTokenList());
    ASTNode root = ast.getRoot();
    REQUIRE(root.getType() == ASTNode::PROGRAM);
    REQUIRE(root.getChildren().size() == 1);

    ASTNode *statement = root.getChildren()[0];
    REQUIRE(statement->getType() == ASTNode::STATEMENT);
    REQUIRE(statement->getChildren().size() == 1);

    ASTNode *block = statement->getChildren()[0];
    REQUIRE(block->getType() == ASTNode::BLOCK_STATEMENT);
    REQUIRE(block->getChildren().size() == 5);

    ASTNode *lcurly = block->getChildren()[1];
    REQUIRE(lcurly->getType() == ASTNode::LCURLY);
    REQUIRE(lcurly->getChildren().size() == 0);
    REQUIRE(lcurly->getValue() == "{");

    ASTNode *statement_in_block1 = block->getChildren()[2];
    REQUIRE(statement_in_block1->getType() == ASTNode::STATEMENT);
    REQUIRE(statement_in_block1->getChildren().size() == 1);

    ASTNode *simple_statement1 = statement_in_block1->getChildren()[0];
    REQUIRE(simple_statement1->getType() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement1->getChildren().size() == 3);

    ASTNode *directive1 = simple_statement1->getChildren()[0];
    REQUIRE(directive1->getType() == ASTNode::DIRECTIVE);
    REQUIRE(directive1->getChildren().size() == 1);
    REQUIRE(directive1->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(directive1->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(directive1->getChildren()[0]->getValue() == "simple1");

    ASTNode *parameters1 = simple_statement1->getChildren()[1];
    REQUIRE(parameters1->getType() == ASTNode::PARAMETERS);
    REQUIRE(parameters1->getChildren().size() == 1);
    ASTNode *parameter1 = parameters1->getChildren()[0];
    REQUIRE(parameter1->getType() == ASTNode::PARAMETER);
    REQUIRE(parameter1->getChildren().size() == 1);
    REQUIRE(parameter1->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(parameter1->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(parameter1->getChildren()[0]->getValue() == "statement1");

    ASTNode *semi1 = simple_statement1->getChildren()[2];
    REQUIRE(semi1->getType() == ASTNode::SEMI);
    REQUIRE(semi1->getChildren().size() == 0);
    REQUIRE(semi1->getValue() == ";");

    ASTNode *statement_in_block2 = block->getChildren()[3];
    REQUIRE(statement_in_block2->getType() == ASTNode::STATEMENT);
    REQUIRE(statement_in_block2->getChildren().size() == 1);

    ASTNode *simple_statement2 = statement_in_block2->getChildren()[0];
    REQUIRE(simple_statement2->getType() == ASTNode::SIMPLE_STATEMENT);
    REQUIRE(simple_statement2->getChildren().size() == 3);

    ASTNode *directive2 = simple_statement2->getChildren()[0];
    REQUIRE(directive2->getType() == ASTNode::DIRECTIVE);
    REQUIRE(directive2->getChildren().size() == 1);
    REQUIRE(directive2->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(directive2->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(directive2->getChildren()[0]->getValue() == "simple2");

    ASTNode *parameters2 = simple_statement2->getChildren()[1];
    REQUIRE(parameters2->getType() == ASTNode::PARAMETERS);
    REQUIRE(parameters2->getChildren().size() == 1);
    ASTNode *parameter2 = parameters2->getChildren()[0];
    REQUIRE(parameter2->getType() == ASTNode::PARAMETER);
    REQUIRE(parameter2->getChildren().size() == 1);
    REQUIRE(parameter2->getChildren()[0]->getType() == ASTNode::ID);
    REQUIRE(parameter2->getChildren()[0]->getChildren().size() == 0);
    REQUIRE(parameter2->getChildren()[0]->getValue() == "statement2");

    ASTNode *semi2 = simple_statement2->getChildren()[2];
    REQUIRE(semi2->getType() == ASTNode::SEMI);
    REQUIRE(semi2->getChildren().size() == 0);
    REQUIRE(semi2->getValue() == ";");

    ASTNode *rcurly = block->getChildren()[4];
    REQUIRE(rcurly->getType() == ASTNode::RCURLY);
    REQUIRE(rcurly->getChildren().size() == 0);
    REQUIRE(rcurly->getValue() == "}");
}

#endif