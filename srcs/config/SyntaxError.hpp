#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include <stdexcept>
#include <string>

class syntax_error : public std::runtime_error
{
  public:
    syntax_error(const std::string &message);
};

#endif /* SYNTAX_ERROR_H */
