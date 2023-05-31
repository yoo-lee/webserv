#ifndef NOT_AUTHORIZED_H
#define NOT_AUTHORIZED_H
#include <exception>
class NotAuthrized : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
        return "not enough privileges";
    }
};

#endif /* NOT_AUTHORIZED_H */
