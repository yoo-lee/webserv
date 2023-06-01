#ifndef NO_SUCH_FILE_OR_DIRECTORY_H
#define NO_SUCH_FILE_OR_DIRECTORY_H

#include <exception>
class NoSuchFileOrDirectory : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
        return "Path is not exist";
    }
};

#endif /* NO_SUCH_FILE_OR_DIRECTORY_H */
