#ifndef LIMITEXCEPT_H
#define LIMITEXCEPT_H
#include "Statement.hpp"
#include <string>
#include <vector>
using namespace std;

class LimitExcept
{
  private:
  public:
    vector<string> methods;
    vector<string> deny_list;
    bool deny_all;
    vector<string> allow_list;
    bool allow_all;
    LimitExcept();
    LimitExcept(Statement *const limit_except_directive);
    LimitExcept(LimitExcept *const l);
    ~LimitExcept();
};

#endif /* LIMITEXCEPT_H */
