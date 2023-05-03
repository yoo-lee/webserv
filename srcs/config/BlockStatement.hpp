#ifndef BLOCKSTATEMENT_H
#define BLOCKSTATEMENT_H
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
    BlockStatement(Statement *s);
    ~BlockStatement();

    void print(std::ostream &os, std::string indent) const;
    friend std::ostream &operator<<(std::ostream &os, const BlockStatement &statement);
    std::vector<Statement *> get_children() const;
    std::vector<Statement *> get_children(std::string directive) const;
    Statement *operator[](std::string directive) const;
    virtual Statement *clone() const;
};

#endif /* BLOCKSTATEMENT_H */
