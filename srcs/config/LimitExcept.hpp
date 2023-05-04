#ifndef LIMIT_EXCEPT_H
#define LIMIT_EXCEPT_H
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
    vector<string> allow_list;
    LimitExcept();
    LimitExcept(Statement *const limit_except_directive);
    LimitExcept(LimitExcept *const l);
    ~LimitExcept();
};

#endif /* LIMIT_EXCEPT_H */
