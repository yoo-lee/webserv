#ifndef BLOCK_STATEMENT_H
#define BLOCK_STATEMENT_H
#include "SimpleStatement.hpp"
#include "Statement.hpp"
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class BlockStatement : public Statement
{
  private:
    vector<Statement const*> _child_statements;

  public:
    BlockStatement(string directive, vector<string> params, vector<Statement const*> child_statements);
    BlockStatement(string directive, vector<Statement const*> child_statements);
    BlockStatement(const BlockStatement& b);
    BlockStatement(Statement const* s);
    ~BlockStatement();

    void print(ostream& os, string indent) const;
    friend ostream& operator<<(ostream& os, const BlockStatement& statement);
    vector<Statement const*> get_children() const;
    vector<SimpleStatement const*> get_simple_statement_children() const;
    vector<BlockStatement const*> get_block_statement_children() const;
    vector<Statement const*> get_children(string directive) const;
    Statement const* operator[](string directive) const;
    virtual Statement* clone() const;
};

#endif /* BLOCK_STATEMENT_H */
