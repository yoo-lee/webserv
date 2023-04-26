#ifndef TERMINAL_AST_NODE_H
#define TERMINAL_AST_NODE_H
#include "ASTNode.hpp"
#include <ostream>
#include <string>
class TerminalASTNode : public ASTNode
{
  private:
    std::string _value;

  public:
    TerminalASTNode();
    TerminalASTNode(ASTNode::Type type, std::string value);
    TerminalASTNode(ASTNode *node);
    TerminalASTNode(TerminalASTNode *node);
    ~TerminalASTNode();
    void print(std::string indent);
    std::string get_value() const;
    bool operator==(const ASTNode &other) const;
    bool operator!=(const ASTNode &other) const;
    friend std::ostream &operator<<(std::ostream &os, const TerminalASTNode &node);
};

#endif /* TERMINAL_AST_NODE_H */
