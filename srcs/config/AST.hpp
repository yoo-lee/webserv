#ifndef AST_H
#define AST_H
#include "ASTNode.hpp"
#include "SyntaxError.hpp"
#include "Token.hpp"
#include <exception>
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
class AST
{
  private:
    std::vector<Token> _tokens;
    ASTNode *root;
    std::queue<Token> _buf;
    ASTNode *program();
    ASTNode *statement() throw(std::exception);
    ASTNode *try_simple_statement() throw(std::exception);
    ASTNode *try_block_statement() throw(std::exception);
    ASTNode *directive();
    ASTNode *parameters();
    ASTNode *parameter() throw(std::exception);
    ASTNode *consume(Token::Type type) throw(syntax_error);
    void backtrace();
    void decide();

  public:
    AST();
    AST(std::vector<Token> tokens);
    void print_tree();
    bool operator==(const AST &other) const;
};

#endif /* AST_H */
