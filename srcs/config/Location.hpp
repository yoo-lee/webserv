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

    LimitExcept* parse_limit_except(vector<BlockStatement const*> statements) const;
    string parse_index(vector<SimpleStatement const*> statements) const;
    bool parse_autoindex(vector<SimpleStatement const*> statements) const;
    map<string, string> parse_error_pages(vector<SimpleStatement const*> statements) const;
    string parse_root(vector<SimpleStatement const*> statements) const;

  public:
    vector<string> urls;
    map<string, vector<string> > properties;
    LimitExcept const* limit_except;
    string index;
    bool autoindex;
    string root;

    // error_pageには以下のように保存されている
    // {"404" :"404用のエラーページ","403" :"403用のエラーページ"}
    map<string, string> error_page;
    Location(Statement const* location_directive);
    Location(Location const& location);
    ~Location();
    vector<string> operator[](string index) const;
};

#endif /* LOCATION_H */
