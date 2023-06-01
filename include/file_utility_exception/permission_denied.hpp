#ifndef NO_PERMISSION_H
#define NO_PERMISSION_H
#include <exception>
class PermissionDenined : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
        return "not enough privileges";
    }
};

#endif /* NO_PERMISSION_H */
