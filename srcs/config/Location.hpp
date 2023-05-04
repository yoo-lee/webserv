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
    Location();

  public:
    std::vector<std::string> urls;
    std::map<std::string, std::vector<std::string> > properties;
    LimitExcept *limit_except;
    std::string index;
    bool autoindex;
    std::map<std::string, std::string> error_page;
    Location(Statement *location_directive);
    Location(Location *location);
    ~Location();
    std::vector<std::string> operator[](std::string index);
};

#endif /* LOCATION_H */
