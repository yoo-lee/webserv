#ifndef PATH_IS_NOT_FILE_H
#define PATH_IS_NOT_FILE_H

#include <exception>
class PathIsNotFile : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
        return "Path is not file";
    }
};

#endif /* PATH_IS_NOT_FILE_H */
