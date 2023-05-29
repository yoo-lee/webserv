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
    string parse_cgi_pass(vector<SimpleStatement const*> statements) const;
    string get_default_error_page(string status_code) const;

  public:
    vector<string> urls;
    map<string, vector<string> > properties;
    LimitExcept const* limit_except;
    string index;
    bool autoindex;
    string root;
    string cgi_pass;

    // こちらではなくget_error_pageを使ってください
    map<string, string> error_page;
    string get_error_page(string status_code) const;
    Location(Statement const* location_directive);
    Location(Location const& location);
    ~Location();
    vector<string> operator[](string index) const;
    Location const& operator=(Location const& other);
};

#endif /* LOCATION_H */
