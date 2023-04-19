#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include <stdexcept>
#include <string>

class syntax_error : public std::runtime_error
{
  public:
    explicit syntax_error(const std::string &message) : std::runtime_error(message)
    {
    }
};

#endif /* SYNTAX_ERROR_H */
