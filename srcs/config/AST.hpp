#ifndef AST_H
#define AST_H
#include "ASTNode.hpp"
#include "NonTerminalASTNode.hpp"
#include "SyntaxError.hpp"
#include "TerminalASTNode.hpp"
#include "Token.hpp"
#include <ostream>
#include <queue>
#include <stack>
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
    NonTerminalASTNode *_root;
    NonTerminalASTNode *program();
    NonTerminalASTNode *statement();
    NonTerminalASTNode *try_simple_statement(std::stack<Token> &buf);
    NonTerminalASTNode *try_block_statement(std::stack<Token> &buf);
    NonTerminalASTNode *directive(std::stack<Token> &buf);
    NonTerminalASTNode *parameters(std::stack<Token> &buf);
    NonTerminalASTNode *parameter(std::stack<Token> &buf);
    TerminalASTNode *consume(Token::Type type, std::stack<Token> &buf);
    void backtrace(std::stack<Token> &buf);
    void decide(std::stack<Token> &buf);

  public:
    AST();
    AST(std::vector<Token> tokens);
    void print_tree();
    NonTerminalASTNode *get_root() const;
    bool operator==(const AST &other) const;
    bool operator!=(const AST &other) const;
};

#endif /* AST_H */
