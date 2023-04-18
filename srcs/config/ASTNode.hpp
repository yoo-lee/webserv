#ifndef ATNODE_H
#define ATNODE_H
#include <vector>
class ATNode {
   public:
    enum Type {
        PROGRAM,
        STATEMENT,
        SIMPLE_STATEMENT,
        BLOCK_STATEMENT,
        DIRECTIVE,
        PARAMETERS,
        PARAMETER
    };
    ATNode(Type type, std::vector<ATNode*> children);
    ~ATNode();

   private:
    Type _type;
    std::vector<ATNode*> _children;
};

#endif /* ATNODE_H */
