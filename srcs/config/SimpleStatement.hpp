#ifndef SIMPLE_STATEMENT_H
#define SIMPLE_STATEMENT_H
#include "Statement.hpp"
#include <string>
#include <vector>
class SimpleStatement : public Statement
{
  private:
    std::string _value;

  public:
    SimpleStatement(std::string directive, std::vector<std::string> _params, std::string value);
    SimpleStatement(std::string directive, std::string value);
    SimpleStatement(const SimpleStatement &s);
    ~SimpleStatement();

    std::string get_value() const;
    void print(std::ostream &os) const;
    friend std::ostream &operator<<(std::ostream &os, const SimpleStatement &statement);
    bool operator==(const SimpleStatement &other) const;
    bool operator!=(const SimpleStatement &other) const;
};

#endif /* SIMPLE_STATEMENT_H */
