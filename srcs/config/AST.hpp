#ifndef AST_H
#define AST_H
#include "ASTNode.hpp"
#include "SyntaxError.hpp"
#include "Token.hpp"
#include <ostream>
#include <queue>
#include <stdexcept>
#include <vector>

/*
program: statement*;
statement: simpleStatement | blockStatement;
simpleStatement: directive parameters? SEMI;
blockStatement: directive parameters? LCURLY statement* RCURLY;
directive: ID;
parameters: parameter | parameter parameters;
parameter: ID | STRING | INT;

STRING:
        DQUOTE [a-zA-Z_0-9]+ DQUOTE
        | SQUOTE [a-zA-Z_0-9]+ SQUOTE;

SQUOTE: '\'';
COMMA: ',';
SEMI: ';';
DQUOTE: '"';
LCURLY: '{';
RCURLY: '}';
INT: [0-9]+;
ID: [a-zA-Z_][a-zA-Z_0-9]*;
WS: [ \t\n\r\f]+ -> skip;
 */
#include "NotFound.hpp"
class AST
{
  private:
    std::vector<Token> _tokens;
    ASTNode *_root;
    std::queue<Token> _buf;
    ASTNode *program();
    ASTNode *statement();
    ASTNode *try_simple_statement();
    ASTNode *try_block_statement();
    ASTNode *directive();
    ASTNode *parameters();
    ASTNode *parameter();
    ASTNode *consume(Token::Type type);
    void backtrace();
    void decide();

  public:
    AST();
    AST(std::vector<Token> tokens);
    void print_tree();
    ASTNode *getRoot() const;
    bool operator==(const AST &other) const;
};

#endif /* AST_H */
