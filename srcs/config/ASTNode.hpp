#ifndef ASTNODE_H
#define ASTNODE_H
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
    ASTNode(ASTNode *ast_node);
    ASTNode(ASTNode::Type type);
    ~ASTNode();
    static std::string ast_node_type_to_str(ASTNode::Type type);
    static ASTNode::Type to_ast_node_type(Token::Type type);
    virtual void print(std::string indent) = 0;
    ASTNode::Type get_type() const;
    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node);

  protected:
    ASTNode::Type _type;
};

#endif /* ASTNODE_H */
