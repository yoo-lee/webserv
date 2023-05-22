#ifndef LOCATION_H
#define LOCATION_H
#include "BlockStatement.hpp"
#include "LimitExcept.hpp"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class Location
{
  private:
    Location();

  public:
    vector<string> urls;
    map<string, vector<string> > properties;
    LimitExcept const* limit_except;
    string index;
    bool autoindex;
    map<string, string> error_page;
    Location(Statement const* location_directive);
    Location(Location const& location);
    ~Location();
    vector<string> operator[](string index);
};

#endif /* LOCATION_H */
