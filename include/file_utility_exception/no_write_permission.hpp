#ifndef NO_WRITE_PERMISSION_H
#define NO_WRITE_PERMISSION_H

#include <exception>
#include <file_utility_exception/no_permission.hpp>
class NoWritePermission : public PermissionDenined
{
  public:
    virtual const char* what() const throw()
    {
        return "not enough privileges";
    }
};

#endif /* NO_WRITE_PERMISSION_H */
