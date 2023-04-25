#ifndef NON_TERMINAL_AST_NODE_H
#define NON_TERMINAL_AST_NODE_H
#include "ASTNode.hpp"
#include <ostream>
#include <vector>
class NonTerminalASTNode : public ASTNode
{
  private:
    std::vector<ASTNode *> _children;

  public:
    NonTerminalASTNode(ASTNode::Type type, std::vector<ASTNode *> children);
    NonTerminalASTNode(ASTNode::Type type, ASTNode *child);
    NonTerminalASTNode(NonTerminalASTNode *node);
    NonTerminalASTNode(ASTNode *node);
    ~NonTerminalASTNode();
    void print(std::string indent);
    std::vector<ASTNode *> get_children() const;
    std::vector<ASTNode *> get_non_terminal_children() const;
    bool operator==(const ASTNode &other) const;
    bool operator!=(const ASTNode &other) const;
    ASTNode *operator[](ASTNode::Type type);
    friend std::ostream &operator<<(std::ostream &os, const NonTerminalASTNode &node);
};

#endif /* NON_TERMINAL_AST_NODE_H */
