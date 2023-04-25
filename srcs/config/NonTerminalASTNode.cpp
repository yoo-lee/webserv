#include "NonTerminalASTNode.hpp"
#include "ASTNode.hpp"
#include "InvalidValue.hpp"
#include "TerminalASTNode.hpp"
#include <exception>
#ifdef UNIT_TEST
#include "doctest.h"
#endif
NonTerminalASTNode::NonTerminalASTNode(ASTNode::Type type, std::vector<ASTNode *> children) : ASTNode(type)
{
    if (type == ASTNode::ID || type == ASTNode::INT || type == ASTNode::STRING || type == ASTNode::SEMI ||
        type == ASTNode::LCURLY || type == ASTNode::RCURLY)
        throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(type) + ") is not a non-terminal type");
    for (size_t i = 0; i < children.size(); ++i)
    {
        if (dynamic_cast<TerminalASTNode *>(children[i]))
            _children.push_back(new TerminalASTNode(children[i]));
        else if (dynamic_cast<NonTerminalASTNode *>(children[i]))
            _children.push_back(new NonTerminalASTNode(children[i]));
        else
            throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(children[i]->get_type()) +
                               ") is not a non-terminal type");
    }
}

NonTerminalASTNode::NonTerminalASTNode(ASTNode::Type type, ASTNode *child) : ASTNode(type)
{
    if (type == ASTNode::ID || type == ASTNode::INT || type == ASTNode::STRING || type == ASTNode::SEMI ||
        type == ASTNode::LCURLY || type == ASTNode::RCURLY)
        throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(type) + ") is not a non-terminal type");
    _children = std::vector<ASTNode *>();
    if (dynamic_cast<TerminalASTNode *>(child))
        _children.push_back(new TerminalASTNode(child));
    else if (dynamic_cast<NonTerminalASTNode *>(child))
        _children.push_back(new NonTerminalASTNode(child));
    else
        throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(child->get_type()) +
                           ") is not a non-terminal type");
}

NonTerminalASTNode::NonTerminalASTNode(NonTerminalASTNode *node) : ASTNode(node)
{
    _children = std::vector<ASTNode *>();
    for (std::size_t i = 0; i < node->_children.size(); ++i)
    {
        if (dynamic_cast<TerminalASTNode *>(node->_children[i]))
            _children.push_back(new TerminalASTNode(node->_children[i]));
        else if (dynamic_cast<NonTerminalASTNode *>(node->_children[i]))
            _children.push_back(new NonTerminalASTNode(node->_children[i]));
        else
            throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(node->_children[i]->get_type()) +
                               ") is not a non-terminal type");
    }
}

NonTerminalASTNode::NonTerminalASTNode(ASTNode *node) : ASTNode(node)
{
    if (dynamic_cast<TerminalASTNode *>(node))
        throw InvalidValue("type ( " + ASTNode::ast_node_type_to_str(node->get_type()) +
                           ") is not a non-terminal type");
    _children = std::vector<ASTNode *>();
    for (std::size_t i = 0; i < ((NonTerminalASTNode *)node)->_children.size(); ++i)
    {
        if (dynamic_cast<TerminalASTNode *>(((NonTerminalASTNode *)node)->_children[i]))
            _children.push_back(new TerminalASTNode(((NonTerminalASTNode *)node)->_children[i]));
        else if (dynamic_cast<NonTerminalASTNode *>(((NonTerminalASTNode *)node)->_children[i]))
            _children.push_back(new NonTerminalASTNode(((NonTerminalASTNode *)node)->_children[i]));
        else
            throw InvalidValue("type ( " +
                               ASTNode::ast_node_type_to_str(((NonTerminalASTNode *)node)->_children[i]->get_type()) +
                               ") is not a non-terminal type");
    }
}

NonTerminalASTNode::~NonTerminalASTNode()
{
    for (std::size_t i = 0; i < _children.size(); ++i)
    {
        delete _children[i];
    }
    ASTNode::~ASTNode();
}

void NonTerminalASTNode::print(std::string indent)
{
    std::cout << indent << ASTNode::ast_node_type_to_str(_type) << std::endl;
    for (std::size_t i = 0; i < _children.size(); ++i)
        _children[i]->print(indent + "  ");
}

std::vector<ASTNode *> NonTerminalASTNode::get_children() const
{
    return _children;
};

bool NonTerminalASTNode::operator==(const ASTNode &other) const
{
    if (dynamic_cast<const NonTerminalASTNode *>(&other) == 0)
        return false;
    if (_type != other.get_type())
        return false;
    if (_children.size() != ((NonTerminalASTNode &)other).get_children().size())
        return false;
    for (std::size_t i = 0; i < _children.size(); ++i)
    {
        if (TerminalASTNode *a = dynamic_cast<TerminalASTNode *>(_children[i]))
        {
            if (TerminalASTNode *b = dynamic_cast<TerminalASTNode *>(((NonTerminalASTNode &)other).get_children()[i]))
            {
                if (*a != *b)
                    return false;
            }
            else
                return false;
        }
        else if (NonTerminalASTNode *a = dynamic_cast<NonTerminalASTNode *>(_children[i]))
        {
            if (NonTerminalASTNode *b =
                    dynamic_cast<NonTerminalASTNode *>(((NonTerminalASTNode &)other).get_children()[i]))
            {
                if (*a != *b)
                    return false;
            }
            else
                return false;
        }
    }
    return true;
}

bool NonTerminalASTNode::operator!=(const ASTNode &other) const
{
    return !(*this == other);
}

ASTNode *NonTerminalASTNode::operator[](ASTNode::Type type)
{
    for (size_t i = 0; i < _children.size(); i++)
        if (_children[i]->get_type() == type)
            return _children[i];
    throw std::out_of_range("No child of type " + ast_node_type_to_str(type));
}

std::ostream &operator<<(std::ostream &os, const NonTerminalASTNode &node)
{
    os << ASTNode::ast_node_type_to_str(node.get_type()) << " : " << node.get_children().size() << std::endl;
    return os;
}

#ifdef UNIT_TEST
TEST_CASE("ASTNode")
{
    SUBCASE("constructor")
    {
        std::vector<ASTNode *> buf;
        NonTerminalASTNode node(ASTNode::PROGRAM, buf);
        CHECK(node.get_type() == ASTNode::PROGRAM);
        CHECK(node.get_children() == buf);
        CHECK_THROWS_AS(NonTerminalASTNode(ASTNode::ID, buf), InvalidValue);
        CHECK_THROWS_AS(NonTerminalASTNode(ASTNode::STRING, buf), InvalidValue);
        CHECK_THROWS_AS(NonTerminalASTNode(ASTNode::INT, buf), InvalidValue);
        CHECK_THROWS_AS(NonTerminalASTNode(ASTNode::SEMI, buf), InvalidValue);
        CHECK_THROWS_AS(NonTerminalASTNode(ASTNode::LCURLY, buf), InvalidValue);
        CHECK_THROWS_AS(NonTerminalASTNode(ASTNode::RCURLY, buf), InvalidValue);
    }

    SUBCASE("==,!=")
    {
        NonTerminalASTNode a(ASTNode::PROGRAM, std::vector<ASTNode *>());
        NonTerminalASTNode b(ASTNode::STATEMENT, std::vector<ASTNode *>());
        CHECK(a == a);
        CHECK(a != b);
    }

#include "TerminalASTNode.hpp"
#include <vector>
    SUBCASE("[]")
    {
        std::vector<ASTNode *> buf;
        TerminalASTNode *child = new TerminalASTNode(ASTNode::ID, "id");
        buf.push_back(child);
        NonTerminalASTNode parent(ASTNode::PROGRAM, buf);
        ASTNode *node = parent[ASTNode::ID];
        CHECK(node->get_type() == ASTNode::ID);
        CHECK_THROWS_AS(parent[ASTNode::STRING], std::out_of_range);
    }
}
#endif