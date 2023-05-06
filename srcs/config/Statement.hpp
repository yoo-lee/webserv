#ifndef STATEMENT_H
#define STATEMENT_H
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class Statement
{
  protected:
    string _directive;
    vector<string> _params;

  public:
    Statement(string directive, vector<string> params);
    Statement(string directive, string param);
    Statement(string directive);
    Statement(const Statement& statement);
    virtual ~Statement() = 0;

    virtual void print(ostream& os, string indent) const;
    virtual bool operator==(const Statement& other) const;
    virtual bool operator!=(const Statement& other) const;

    string get_directive() const;
    vector<string> get_params() const;
    string get_param(size_t index) const;

    friend ostream& operator<<(ostream& os, const Statement& statement);
    virtual Statement* clone() const = 0;
};

#endif /* STATEMENT_H */
