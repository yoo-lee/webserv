#ifndef CONFIG_H
#define CONFIG_H
#include "Http.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

class Config
{
  private:
    static map<pair<string, string>, Server const*> _servers_cache;
    static map<pair<string, string>, vector<string> > _locations_cache;
    static map<pair<pair<string, string>, string>, map<string, vector<string> > > _locations_content_cache;
    static map<pair<pair<string, string>, string>, map<string, vector<string> > > _locations_properties_cache;

  public:
    HTTP const* http;
    Config();
    Config(string file_text, bool is_file_text);
    Config(const char* file_path);
    void init(string file_text);
    ~Config();
    Server const& get_default_server() const;
    bool is_cgi(string path) const;

    Config const& operator=(Config const& c);

    Server const* get_server(string const& port, string const& host) const;
    Location const* get_location(string const& port, string const& host, string const& path) const;
    vector<string> get_location_paths(string const& port, string const& host) const;
    map<string, vector<string> > get_locations_contents(string const& port, string const& host,
                                                        string const& location) const;
    map<string, vector<string> > get_locations_properties(string const& port, string const& host,
                                                          string const& filepath) const;
};

#endif /* CONFIG_H */
