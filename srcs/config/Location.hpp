#ifndef LOCATION_H
#define LOCATION_H
#include "BlockStatement.hpp"
#include "LimitExcept.hpp"
#include <map>
#include <string>
#include <vector>
class Location
{
  private:
  public:
    std::vector<std::string> urls;
    std::map<std::string, std::vector<std::string> > properties;
    LimitExcept *limit_except;
    Location();
    Location(Statement *location_directive);
    Location(Location *location);
    ~Location();
    std::string operator[](size_t index) const;
};

#endif /* LOCATION_H */
