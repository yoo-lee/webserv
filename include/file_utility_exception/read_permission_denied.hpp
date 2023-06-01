#ifndef NO_READ_PERMISSION_H
#define NO_READ_PERMISSION_H

#include <exception>
#include <file_utility_exception/permission_denied.hpp>
class ReadPermissionDenied : public PermissionDenined
{
  public:
    virtual const char* what() const throw()
    {
        return "not enough privileges";
    }
};

#endif /* NO_READ_PERMISSION_H */
