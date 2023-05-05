#ifndef AST_H
#define AST_H
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
#include "BlockStatement.hpp"
#include "NotFound.hpp"
#include "Statement.hpp"

using std::stack;
using std::string;
using std::vector;

// TODO: ASTではなくなった。もっと適切な名前に変更する。
class AST
{
  private:
    vector<Token> _tokens;
    vector<Statement const*> _root;

    vector<Statement*> program();
    Statement const* statement();
    Statement const* try_simple_statement(stack<Token>& buf);
    BlockStatement const* try_block_statement(stack<Token>& buf);
    string directive(stack<Token>& buf);
    vector<string> parameters(stack<Token>& buf);
    string parameter(stack<Token>& buf);
    string consume(Token::Type type, stack<Token>& buf);
    void backtrace(stack<Token>& buf);
    void decide(stack<Token>& buf);

  public:
    AST();
    AST(const vector<Token>& tokens);
    ~AST();
    void print_tree();
    vector<Statement const*> const& get_root() const;
    Statement const* operator[](string directive) const;
};

#endif /* AST_H */
