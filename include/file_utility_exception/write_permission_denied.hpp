#ifndef WRITE_PERMISSION_DENIED_H
#define WRITE_PERMISSION_DENIED_H

#include <exception>
#include <file_utility_exception/permission_denied.hpp>
class WritePermissionDenied : public PermissionDenined
{
  public:
    virtual const char* what() const throw()
    {
        return "not enough privileges";
    }
};

#endif /* WRITE_PERMISSION_DENIED_H */
