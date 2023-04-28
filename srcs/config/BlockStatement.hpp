#ifndef BLOCK_STATEMENT_H
#define BLOCK_STATEMENT_H
#include "Statement.hpp"
#include <string>
#include <vector>

class BlockStatement : public Statement
{
  private:
    std::vector<Statement *> _child_statements;

  public:
    BlockStatement(std::string directive, std::vector<std::string> params, std::vector<Statement *> child_statements);
    BlockStatement(std::string directive, std::vector<Statement *> child_statements);
    BlockStatement(const BlockStatement &b);
    ~BlockStatement();

    void print(std::ostream &os, std::string indent) const;
    friend std::ostream &operator<<(std::ostream &os, const BlockStatement &statement);
    std::vector<Statement *> get_child_statements() const;
    std::vector<Statement *> get_child_statements(std::string directive) const;
    Statement *operator[](std::string directive) const;
};

#endif /* BLOCK_STATEMENT_H */
