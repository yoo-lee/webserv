#ifndef LOCATION_H
#define LOCATION_H
#include "BlockStatement.hpp"
#include <map>
#include <string>
#include <vector>
class Location
{
  private:
  public:
    std::vector<std::string> urls;
    std::map<std::string, std::string> properties;
    Location();
    Location(BlockStatement const &location_directive);
    Location(Location *location);
    ~Location();
    std::string operator[](size_t index) const;
};

#endif /* LOCATION_H */
