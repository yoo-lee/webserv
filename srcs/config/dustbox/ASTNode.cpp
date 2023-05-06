#include "ASTNode.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

ASTNode::Type ASTNode::to_ast_node_type(Token::Type type)
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

std::string ASTNode::ast_node_type_to_str(Type type)
{
    const char *typename_list[] = {"PROGRAM",
                                   "STATEMENT",
                                   "SIMPLE_STATEMENT",
                                   "BLOCK_STATEMENT",
                                   "DIRECTIVE",
                                   "PARAMETERS",
                                   "PARAMETER",
                                   "ID",
                                   "STRING",
                                   "INT",
                                   "SEMI",
                                   "LCURLY",
                                   "RCURLY"};
    if (type <= RCURLY)
        return typename_list[type];
    return "UNKNOWN";
}

ASTNode::ASTNode(ASTNode *at_node) : _type(at_node->_type)
{
}

ASTNode::ASTNode(ASTNode::Type type) : _type(type)
{
}

ASTNode::~ASTNode()
{
}

void ASTNode::print(std::string indent)
{
    std::cout << indent << ASTNode::ast_node_type_to_str(_type) << std::endl;
}

ASTNode::Type ASTNode::get_type() const
{
    return _type;
}

std::ostream &operator<<(std::ostream &os, const ASTNode &node)
{
    os << ASTNode::ast_node_type_to_str(node._type);
    return os;
}

#ifdef UNIT_TEST
TEST_CASE("ASTNode")
{
    SUBCASE("toASTNodeType")
    {
        CHECK(ASTNode::to_ast_node_type(Token::ID) == ASTNode::ID);
        CHECK(ASTNode::to_ast_node_type(Token::STRING) == ASTNode::STRING);
        CHECK(ASTNode::to_ast_node_type(Token::INT) == ASTNode::INT);
        CHECK(ASTNode::to_ast_node_type(Token::SEMI) == ASTNode::SEMI);
        CHECK(ASTNode::to_ast_node_type(Token::LCURLY) == ASTNode::LCURLY);
        CHECK(ASTNode::to_ast_node_type(Token::RCURLY) == ASTNode::RCURLY);
        CHECK_THROWS_AS(ASTNode::to_ast_node_type(Token::COMMA), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::to_ast_node_type(Token::DQUOTE), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::to_ast_node_type(Token::SQUOTE), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::to_ast_node_type(Token::WHITE_SPACE), std::runtime_error);
        CHECK_THROWS_AS(ASTNode::to_ast_node_type(Token::NONE), std::runtime_error);
    }

    SUBCASE("ast_node_type_to_str")
    {
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::PROGRAM) == "PROGRAM");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::STATEMENT) == "STATEMENT");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::SIMPLE_STATEMENT) == "SIMPLE_STATEMENT");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::BLOCK_STATEMENT) == "BLOCK_STATEMENT");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::DIRECTIVE) == "DIRECTIVE");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::PARAMETERS) == "PARAMETERS");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::PARAMETER) == "PARAMETER");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::ID) == "ID");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::STRING) == "STRING");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::INT) == "INT");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::SEMI) == "SEMI");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::LCURLY) == "LCURLY");
        CHECK(ASTNode::ast_node_type_to_str(ASTNode::RCURLY) == "RCURLY");
    }
}
#endif