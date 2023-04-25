#include "TerminalASTNode.hpp"
#include "InvalidValue.hpp"
#include "NonTerminalASTNode.hpp"
#include <sstream>
#ifdef UNIT_TEST
#include "doctest.h"
#endif
TerminalASTNode::TerminalASTNode(ASTNode::Type type, std::string value) : ASTNode(type), _value(value)
{
    if (!(type == ASTNode::ID || type == ASTNode::INT || type == ASTNode::STRING || type == ASTNode::SEMI ||
          type == ASTNode::LCURLY || type == ASTNode::RCURLY))
        throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(type) + " ) is not a terminal type");
}

TerminalASTNode::TerminalASTNode(ASTNode *node) : ASTNode(node)
{
    if (dynamic_cast<NonTerminalASTNode *>(node))
    {
        std::stringstream s;
        if (node)
            s << *node;
        else
            s << "null";
        throw InvalidValue("fail to cast ASTNode to TerminalASTNode: " + s.str());
    }
    _value = ((TerminalASTNode *)node)->get_value();
}

TerminalASTNode::TerminalASTNode(TerminalASTNode *node) : ASTNode(node), _value(node->_value)
{
}

TerminalASTNode::~TerminalASTNode()
{
}

void TerminalASTNode::print(std::string indent)
{
    std::cout << indent << ASTNode::ast_node_type_to_str(_type) << " " << _value << std::endl;
}

std::string TerminalASTNode::get_value() const
{
    return _value;
};

bool TerminalASTNode::operator==(const ASTNode &other) const
{
    if (dynamic_cast<const TerminalASTNode *>(&other) == 0)
        return false;
    return _type == other.get_type() && _value == ((TerminalASTNode &)other).get_value();
}

bool TerminalASTNode::operator!=(const ASTNode &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const TerminalASTNode &node)
{
    os << ASTNode::ast_node_type_to_str(node.get_type()) << " " << node.get_value();
    return os;
}

#ifdef UNIT_TEST
TEST_CASE("ASTNode")
{
    SUBCASE("==,!=")
    {
        TerminalASTNode a(ASTNode::ID, "a");
        TerminalASTNode b(ASTNode::ID, "b");
        CHECK(a == a);
        CHECK(a != b);
    }
}
#endif