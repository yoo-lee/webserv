#ifndef NOTFOUND_H
#define NOTFOUND_H

#include <stdexcept>
#include <string>

class NotFound : public std::runtime_error
{
  public:
    NotFound(const std::string& message);
};

#endif /* NOTFOUND_H */
