#ifndef AST_H
#define AST_H
// #include "ASTNode.hpp"
#include "SyntaxError.hpp"
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
#include "Statement.hpp"
class AST
{
  private:
    std::vector<Token> _tokens;
    std::vector<Statement *> _root;
    std::vector<Statement *> program();
    Statement *statement();
    Statement *try_simple_statement(std::stack<Token> &buf);
    Statement *try_block_statement(std::stack<Token> &buf);
    std::string directive(std::stack<Token> &buf);
    std::vector<std::string> parameters(std::stack<Token> &buf);
    std::string parameter(std::stack<Token> &buf);
    std::string consume(Token::Type type, std::stack<Token> &buf);
    void backtrace(std::stack<Token> &buf);
    void decide(std::stack<Token> &buf);

  public:
    AST();
    AST(std::vector<Token> tokens);
    ~AST();
    void print_tree();
    std::vector<Statement *> get_root() const;
};

#endif /* AST_H */
