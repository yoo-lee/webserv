#ifndef AT_H
#define AT_H
#include <vector>
#include "ASTNode.hpp"
#include "Token.hpp"
class AT {
   private:
    std::vector<Token*> _tokens;

   public:
    ATNode* program();
    ATNode* statement();
    ATNode* simple_statement();
    ATNode* block_statement();
    ATNode* directive();
    ATNode* parameters();
    ATNode* parameter();
    AT(std::vector<Token*> tokens);
};

AT::AT(std::vector<Token*> tokens) : _tokens(tokens) {}

ATNode* AT::program() {
    std::vector<ATNode*> children;
    while (true) {
        ATNode* statement_node = statement();
        if (statement_node == NULL) {
            break;
        }
        children.push_back(statement_node);
    }
    return new ATNode(ATNode::PROGRAM, children);
}

ATNode* AT::statement() {
    ATNode* simple_statement_node = simple_statement();
    if (simple_statement_node != NULL) {
        return simple_statement_node;
    }
    ATNode* block_statement_node = block_statement();
    if (block_statement_node != NULL) {
        return block_statement_node;
    }
    return NULL;
}

ATNode* AT::simple_statement() {
    ATNode* directive_node = directive();
    if (directive_node == NULL) {
        return NULL;
    }
    return new ATNode(ATNode::STATEMENT, {directive_node});
}

ATNode* AT::block_statement() {
    ATNode* directive_node = directive();
    if (directive_node == NULL) {
        return NULL;
    }
    ATNode* parameters_node = parameters();
    if (parameters_node == NULL) {
        return NULL;
    }
    return new ATNode(ATNode::STATEMENT, {directive_node, parameters_node});
}

ATNode* AT::parameters() {
    std::vector<ATNode*> children;
    while (true) {
        ATNode* parameter_node = parameter();
        if (parameter_node == NULL) {
            break;
        }
        children.push_back(parameter_node);
    }
    return new ATNode(ATNode::PARAMETERS, children);
}

ATNode* AT::parameter() {
    ATNode* simple_statement_node = simple_statement();
    if (simple_statement_node != NULL) {
        return simple_statement_node;
    }
    ATNode* block_statement_node = block_statement();
    if (block_statement_node != NULL) {
        return block_statement_node;
    }
    return NULL;
}

ATNode* AT::directive() {
    if ()
        return NULL;
}

#endif /* AT_H */
