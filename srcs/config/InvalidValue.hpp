#ifndef INVALID_VALUE_H
#define INVALID_VALUE_H

#include <stdexcept>
#include <string>

class InvalidValue : public std::runtime_error
{
  public:
    InvalidValue(const std::string& message);
};

#endif /* INVALID_VALUE_H */
