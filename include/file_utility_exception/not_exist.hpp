#ifndef NOT_EXIST_H
#define NOT_EXIST_H

#include <exception>
class NotExist : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
        return "Path is not exist";
    }
};

#endif /* NOT_EXIST_H */
