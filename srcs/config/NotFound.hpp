#ifndef NOTFOUND_H
#define NOTFOUND_H

#include <stdexcept>
#include <string>

class not_found : public std::runtime_error
{
  public:
    not_found(const std::string &message);
};

#endif /* NOTFOUND_H */
