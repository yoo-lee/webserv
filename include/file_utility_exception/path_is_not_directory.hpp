#ifndef PATH_IS_NOT_DIRECTORY_H
#define PATH_IS_NOT_DIRECTORY_H

#include <exception>
class PathIsNotDirectory : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
        return "Path is not directory";
    }
};

#endif /* PATH_IS_NOT_DIRECTORY_H */
