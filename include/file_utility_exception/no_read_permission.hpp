#ifndef NO_READ_PERMISSION_H
#define NO_READ_PERMISSION_H

#include <exception>
#include <file_utility_exception/no_permission.hpp>
class NoReadPermission : public NoPermission
{
  public:
    virtual const char* what() const throw()
    {
        return "not enough privileges";
    }
};

#endif /* NO_READ_PERMISSION_H */
