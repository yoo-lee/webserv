#ifndef STATEMENT_H
#define STATEMENT_H
#include <ostream>
#include <string>
#include <vector>
class Statement
{
  protected:
    std::string _directive;
    std::vector<std::string> _params;

  public:
    Statement(std::string directive, std::vector<std::string> params);
    Statement(std::string directive, std::string param);
    Statement(const Statement &statement);
    ~Statement();

    std::string get_directive() const;
    std::vector<std::string> get_params() const;

    virtual void print(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const Statement &statement);

#endif /* STATEMENT_H */
