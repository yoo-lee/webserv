#include "ASTNode.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

ASTNode::Type ASTNode::TokenTypeToASTNodeType(Token::Type type)
{
    switch (type)
    {
    case Token::ID:
        return ASTNode::ID;
    case Token::STRING:
        return ASTNode::STRING;
    case Token::INT:
        return ASTNode::INT;
    case Token::SEMI:
        return ASTNode::SEMI;
    case Token::LCURLY:
        return ASTNode::LCURLY;
    case Token::RCURLY:
        return ASTNode::RCURLY;
    default:
        throw std::runtime_error("Unknown token type converted to AST node");
    }
}

std::string ASTNode::ASTNodeTypeToStr(Type type)
{
    switch (type)
    {
    case PROGRAM:
        return "PROGRAM";
    case STATEMENT:
        return "STATEMENT";
    case SIMPLE_STATEMENT:
        return "SIMPLE_STATEMENT";
    case BLOCK_STATEMENT:
        return "BLOCK_STATEMENT";
    case DIRECTIVE:
        return "DIRECTIVE";
    case PARAMETERS:
        return "PARAMETERS";
    case PARAMETER:
        return "PARAMETER";
    case ID:
        return "ID";
    case STRING:
        return "STRING";
    case INT:
        return "INT";
    case SEMI:
        return "SEMI";
    case LCURLY:
        return "LCURLY";
    case RCURLY:
        return "RCURLY";
    default:
        return "UNKNOWN";
    }
}

void ASTNode::print(std::string indent)
{
    std::cout << indent << ASTNode::ASTNodeTypeToStr(_type) << (_value != "" ? ", " + _value : "") << std::endl;
    for (std::size_t i = 0; i < _children.size(); ++i)
        _children[i]->print(indent + "  ");
}

ASTNode::ASTNode(ASTNode::Type type, std::vector<ASTNode *> children) : _type(type), _children(children), _value("")
{
}

ASTNode::ASTNode(ASTNode::Type type, std::string value) : _type(type), _value(value)
{
    _children = std::vector<ASTNode *>();
}

ASTNode::ASTNode(ASTNode::Type type, ASTNode *child) : _type(type), _value("")
{
    _children = std::vector<ASTNode *>();
    _children.push_back(child);
}

ASTNode::ASTNode(ASTNode *at_node) : _type(at_node->_type), _value(at_node->_value)
{
    _children = std::vector<ASTNode *>();
    for (std::size_t i = 0; i < at_node->_children.size(); ++i)
    {
        _children.push_back(new ASTNode(at_node->_children[i]));
    }
}

ASTNode::~ASTNode()
{
    for (std::size_t i = 0; i < _children.size(); ++i)
    {
        delete _children[i];
    }
}

bool ASTNode::operator==(const ASTNode &other) const
{
    if (this->_type != other._type)
        return false;
    if (this->_value != other._value)
        return false;
    if (this->_children.size() != other._children.size())
        return false;
    for (std::size_t i = 0; i < this->_children.size(); ++i)
    {
        if (*this->_children[i] != *other._children[i])
            return false;
    }
    return true;
}

bool ASTNode::operator!=(const ASTNode &other) const
{
    return !(*this == other);
}

ASTNode ASTNode::operator[](ASTNode::Type type)
{
    for (size_t i = 0; i < _children.size(); i++)
        if (_children[i]->_type == type)
            return *_children[i];
    throw std::out_of_range("No child of type " + ASTNodeTypeToStr(type));
}

ASTNode::Type ASTNode::getType() const
{

    return _type;
}
std::string ASTNode::getValue() const
{
    return _value;
}
std::vector<ASTNode *> ASTNode::getChildren() const
{
    return _children;
}

#ifdef UNIT_TEST
TEST_CASE("ASTNode")
{
    SUBCASE("TokenTypeToASTNodeType")
    {
        CHECK(ASTNode::TokenTypeToASTNodeType(Token::ID) == ASTNode::ID);
        CHECK(ASTNode::TokenTypeToASTNodeType(Token::STRING) == ASTNode::STRING);
        CHECK(ASTNode::TokenTypeToASTNodeType(Token::INT) == ASTNode::INT);
        CHECK(ASTNode::TokenTypeToASTNodeType(Token::SEMI) == ASTNode::SEMI);
        CHECK(ASTNode::TokenTypeToASTNodeType(Token::LCURLY) == ASTNode::LCURLY);
        CHECK(ASTNode::TokenTypeToASTNodeType(Token::RCURLY) == ASTNode::RCURLY);
        CHECK_THROWS_AS(ASTNode::TokenTypeToASTNodeType(Token::COMMA), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::TokenTypeToASTNodeType(Token::DQUOTE), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::TokenTypeToASTNodeType(Token::SQUOTE), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::TokenTypeToASTNodeType(Token::WHITE_SPACE), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::TokenTypeToASTNodeType(Token::NONE), std::runtime_error);
    }

    SUBCASE("ASTNodeTypeToStr")
    {
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::PROGRAM) == "PROGRAM");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::STATEMENT) == "STATEMENT");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::SIMPLE_STATEMENT) == "SIMPLE_STATEMENT");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::BLOCK_STATEMENT) == "BLOCK_STATEMENT");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::DIRECTIVE) == "DIRECTIVE");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::PARAMETERS) == "PARAMETERS");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::PARAMETER) == "PARAMETER");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::ID) == "ID");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::STRING) == "STRING");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::INT) == "INT");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::SEMI) == "SEMI");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::LCURLY) == "LCURLY");
        CHECK(ASTNode::ASTNodeTypeToStr(ASTNode::RCURLY) == "RCURLY");
    }

    SUBCASE("==,!=")
    {
        ASTNode a(ASTNode::ID, "a");
        ASTNode b(ASTNode::ID, "b");
        CHECK(a == a);
        CHECK(a != b);
    }

#include <vector>

    SUBCASE("[]")
    {
        std::vector<ASTNode *> buf;
        ASTNode *child = new ASTNode(ASTNode::ID, "id");
        buf.push_back(child);
        ASTNode parent(ASTNode::PROGRAM, buf);
        CHECK(parent[ASTNode::ID] == child);
        CHECK_THROWS_AS(parent[ASTNode::STRING], std::out_of_range);
    }
}
#endif