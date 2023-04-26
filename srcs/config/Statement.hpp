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
    Statement(std::string directive);
    Statement(const Statement &statement);
    virtual ~Statement();

    std::string get_directive() const;
    std::vector<std::string> get_params() const;

    virtual void print(std::ostream &os, std::string indent) const;
    virtual bool operator==(const Statement &other) const;
    virtual bool operator!=(const Statement &other) const;
};

std::ostream &operator<<(std::ostream &os, const Statement &statement);

#endif /* STATEMENT_H */
