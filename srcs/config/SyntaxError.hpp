#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error
{
  public:
    SyntaxError(const std::string& message);
};

#endif /* SYNTAX_ERROR_H */
