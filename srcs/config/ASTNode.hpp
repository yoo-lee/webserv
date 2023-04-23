#ifndef AST_NODE_H
#define AST_NODE_H
#include "Token.hpp"
#include <ostream>
#include <vector>
class ASTNode
{
  public:
    enum Type
    {
        PROGRAM,
        STATEMENT,
        SIMPLE_STATEMENT,
        BLOCK_STATEMENT,
        DIRECTIVE,
        PARAMETERS,
        PARAMETER,
        ID,
        STRING,
        INT,
        SEMI,
        LCURLY,
        RCURLY
    };
    ASTNode(ASTNode::Type type, std::vector<ASTNode *> children);
    ASTNode(ASTNode::Type type, std::string value);
    ASTNode(ASTNode::Type type, ASTNode *child);
    ASTNode(ASTNode *at_node);
    ~ASTNode();
    static std::string ASTNodeTypeToStr(ASTNode::Type type);
    static ASTNode::Type TokenTypeToASTNodeType(Token::Type type);
    void print(std::string indent);
    bool operator==(const ASTNode &other) const;
    bool operator!=(const ASTNode &other) const;
    ASTNode operator[](ASTNode::Type type);

  private:
    ASTNode::Type _type;
    std::vector<ASTNode *> _children;
    std::string _value;
};

#endif /* AST_NODE_H */
