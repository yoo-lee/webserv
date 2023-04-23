#ifndef NOTFOUND_H
#define NOTFOUND_H

#include <stdexcept>
#include <string>

class not_found : public std::runtime_error
{
  public:
    explicit not_found(const std::string &message) : std::runtime_error(message)
    {
    }
};

#endif /* NOTFOUND_H */
