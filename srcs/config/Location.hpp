#ifndef LOCATION_H
#define LOCATION_H
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
    Location(std::vector<std::string> url, std::map<std::string, std::string> properties);
    Location(Location *location);
    ~Location();
};

Location::Location()
{
}

Location::Location(std::vector<std::string> urls, std::map<std::string, std::string> properties)
    : urls(urls), properties(properties)
{
}

Location::Location(Location *location) : urls(location->urls), properties(location->properties)
{
}

Location::~Location()
{
}

#endif /* LOCATION_H */
