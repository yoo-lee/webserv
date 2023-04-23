#include "AST.hpp"
#include "Token.hpp"
AST::AST() : root(NULL), _buf()
{
}

AST::AST(std::vector<Token> tokens) : _tokens(tokens), _buf()
{
    root = program();
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
        catch (std::exception &e)
        {
            break;
        }
    }
    return new ASTNode(ASTNode::PROGRAM, children);
}

ASTNode *AST::statement() throw(std::exception)
{
    try
    {
        ASTNode *n = try_simple_statement();
        decide();
        return n;
    }
    catch (std::exception &e)
    {
        backtrace();
    }
    try
    {
        ASTNode *n = try_block_statement();
        decide();
        return n;
    }
    catch (std::exception &e)
    {
        backtrace();
    }
    throw std::exception();
}

ASTNode *AST::try_simple_statement() throw(std::exception)
{
    std::vector<ASTNode *> children = std::vector<ASTNode *>();
    children.push_back(directive());
    ASTNode *parameters_node = parameters();
    children.push_back(parameters_node);
    children.push_back(new ASTNode(consume(Token::SEMI)));
    return new ASTNode(ASTNode::STATEMENT, children);
}

ASTNode *AST::try_block_statement() throw(std::exception)
{
    std::vector<ASTNode *> children = std::vector<ASTNode *>();
    children.push_back(directive());
    children.push_back(statement());
    while (true)
    {
        try
        {
            children.push_back(statement());
        }
        catch (std::exception &e)
        {
            break;
        }
    }
    children.push_back(consume(Token::RCURLY));
    return new ASTNode(ASTNode::STATEMENT, children);
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
        catch (std::exception &e)
        {
            break;
        }
    }
    return new ASTNode(ASTNode::PARAMETERS, children);
}

ASTNode *AST::parameter() throw(std::exception)
{
    Token current_token = _tokens.front();
    if (current_token.getType() == Token::ID)
        return new ASTNode(ASTNode::PARAMETER, consume(Token::ID));
    else if (current_token.getType() == Token::STRING)
        return new ASTNode(ASTNode::PARAMETER, consume(Token::STRING));
    else if (current_token.getType() == Token::INT)
        return new ASTNode(ASTNode::PARAMETER, consume(Token::INT));
    throw std::exception();
}

ASTNode *AST::directive()
{
    return new ASTNode(ASTNode::DIRECTIVE, consume(Token::ID));
}

ASTNode *AST::consume(Token::Type type) throw(syntax_error)
{
    if (_tokens.empty())
        throw syntax_error("Unexpected end of file");
    Token current_token = _tokens.front();
    if (current_token.getType() != type)
        throw syntax_error("Unexpected token");
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
    root->print("");
}