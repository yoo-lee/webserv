#include "Config.hpp"
#include "utility.hpp"
#include "splitted_string.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::ifstream;
using std::istreambuf_iterator;
using std::runtime_error;

Config::Config() {}

Config::Config(string file_text, bool is_file_text)
{
    (void)is_file_text;
    init(file_text);
}

Config::Config(const char* file_path)
{
    ifstream ifs(file_path);
    if (!ifs)
        throw runtime_error("Config: File not found: " + string(file_path));
    string file_text((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    init(file_text);
}

void Config::init(string file_text)
{
    Parser parser(file_text);
    vector<Statement const*> root = parser.get_root();
    if (root.size() != 1)
        throw runtime_error("Invalid config file. May have no root or multiple roots.");
    http = new HTTP(root[0]);
}

Config::~Config()
{
    delete http;
}

Server const& Config::get_default_server() const
{
    for (size_t i = 0; i < http->server.size(); i++)
        if (http->server[i]->is_default_server)
            return *http->server[i];
    return *http->server[0];
}

Config const& Config::operator=(Config const& c)
{
    http = new HTTP(*(c.http));
    return *this;
}

map<pair<string, string>, Server const*> Config::_servers_cache;
map<pair<string, string>, vector<string> > Config::_locations_cache;
map<pair<pair<string, string>, string>, map<string, vector<string> > > Config::_locations_content_cache;
map<pair<pair<string, string>, string>, map<string, vector<string> > > Config::_locations_properties_cache;

Server const* Config::get_server(string const& port, string const& host) const
{
    map<pair<string, string>, Server const*>::iterator cash_ite = _servers_cache.find(make_pair(port, host));
    if (cash_ite != _servers_cache.end())
        return (cash_ite->second);

    vector<Server const*> servers;
    for (size_t i = 0; i < http->server.size(); i++) {
        if (http->server[i]->listen == port && http->server[i]->server_name == host) {
            _servers_cache.insert(make_pair(make_pair(port, host), http->server[i]));
            return (http->server[i]);
        }
    }

    for (size_t i = 0; i < http->server.size(); i++) {
        if (http->server[i]->listen == port && http->server[i]->is_default_server) {
            _servers_cache.insert(make_pair(make_pair(port, host), http->server[i]));
            return (http->server[i]);
        }
    }
    for (size_t i = 0; i < http->server.size(); i++) {
        if (http->server[i]->listen == port) {
            _servers_cache.insert(make_pair(make_pair(port, host), http->server[i]));
            return (http->server[i]);
        }
    }
    return (NULL);
}

Location const* Config::get_location(string const& port, string const& host, string const& path) const
{
    Server const* server = get_server(port, host);
    if (server == NULL)
        throw runtime_error("Config: Server(port: " + port + " , host: " + host + " ) not found");
    vector<Location*> locations = server->location;
    vector<pair<Location*, string> > candidate;
    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = 0; j < locations[i]->urls.size(); j++) {
            if (locations[i]->urls[j].size() <= path.size() &&
                path.substr(0, locations[i]->urls[j].size()) == locations[i]->urls[j]) {
                candidate.push_back(make_pair(locations[i], locations[i]->urls[j]));
                continue;
            }
        }
    }
    size_t max_match_character_count = 0;
    Location const* max_match_location = NULL;
    for (size_t i = 0; i < candidate.size(); i++) {
        if (candidate[i].second.size() > max_match_character_count) {
            max_match_character_count = candidate[i].second.size();
            max_match_location = candidate[i].first;
        }
    }
    return max_match_location;
}

vector<string> Config::get_location_paths(string const& port, string const& host) const
{
    map<pair<string, string>, vector<string> >::iterator cash_ite = _locations_cache.find(make_pair(port, host));
    if (cash_ite != _locations_cache.end()) {
        return (cash_ite->second);
    }
    Server const* servers = get_server(port, host);
    vector<string> locations;

    for (size_t j = 0; j < servers->location.size(); j++) {
        for (size_t k = 0; k < servers->location[j]->urls.size(); k++) {
            locations.push_back(servers->location[j]->urls[k]);
        }
    }
    _locations_cache.insert(make_pair(make_pair(port, host), locations));
    return (locations);
}

map<string, vector<string> > Config::get_locations_contents(string const& port, string const& host,
                                                            string const& location) const
{
    map<pair<pair<string, string>, string>, map<string, vector<string> > >::iterator cash_ite =
        _locations_content_cache.find(make_pair(make_pair(port, host), location));
    if (cash_ite != _locations_content_cache.end()) {
        return (cash_ite->second);
    }

    vector<map<string, vector<string> > > properties;
    Server const* servers = get_server(port, host);
    for (size_t j = 0; j < servers->location.size(); j++) {
        for (size_t k = 0; k < servers->location[j]->urls.size(); k++) {
            if (servers->location[j]->urls[k] == location) {
                _locations_content_cache.insert(
                    make_pair(make_pair(make_pair(port, host), location), servers->location[j]->properties));

                return (servers->location[j]->properties);
            }
        }
    }
    map<string, vector<string> > rval;
    return (rval);
}

static string get_partial_equaled_path(SplittedString& req_path_sp, SplittedString& cgi_path_sp)
{
    string path = "";
    for (size_t i = 0; i < cgi_path_sp.size(); i++) {
        if (req_path_sp[i] == cgi_path_sp[i]) {
            path += "/";
            path += req_path_sp[i];
        } else {
            break;
        }
    }
    return (path);
}

std::map<std::string, std::vector<std::string> > Config::get_locations_properties(const string& port,
                                                                                  const string& host,
                                                                                  const string& filepath) const
{
    map<pair<pair<string, string>, string>, map<string, vector<string> > >::iterator cash_ite =
        _locations_properties_cache.find(make_pair(make_pair(port, host), filepath));
    if (cash_ite != _locations_properties_cache.end()) {
        return (cash_ite->second);
    }

    const std::vector<std::string> lo = Config::get_location_paths(port, host);
    SplittedString req_path_sp(filepath, "/");

    string path;
    string tmp_path_cfg;
    for (size_t i = 0; i < lo.size(); i++) {
        tmp_path_cfg = lo[i];
        SplittedString cgi_path_sp(tmp_path_cfg, "/");
        path = get_partial_equaled_path(req_path_sp, cgi_path_sp);
        if (path != "") {
            break;
        } else if (path == "" && lo[i] == "/") {
            path = "/";
            break;
        }
    }

    std::map<std::string, std::vector<std::string> > properties =
        Config::get_locations_contents(port, host, tmp_path_cfg);
    _locations_properties_cache.insert(make_pair(make_pair(make_pair(port, host), filepath), properties));
    return (properties);
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("Config: constructor test")
{
    Config config("http { client_max_body_size 90;server { listen 80;} }", true);
    CHECK(config.http->client_max_body_size == 90);
    CHECK(config.http->server[0]->listen == "80");
    CHECK(config.http->server[0]->server_name == "");
    CHECK(config.http->server[0]->location.size() == 0);
}

/* config/subject.nginx.conf
http {
    client_max_body_size 80;
    server {
        listen 80 default_server;
        server_name example.com;

        location / {
            autoindex on;
            error_page 404 /404.html;
            index index.html;
            limit_except GET {
                deny all;
            }
        }

        location /static {
            root /var/www/html;
        }

        location \.php$ {
            cgi_pass /var/run/php/php;
        }
    }
}
*/

#include <unistd.h>

TEST_CASE("Config: Total test")
{
    char cwd[1024];
    // config内でmakeした場合とsrcsでmakeした場合でconfへのパスが変わるので分岐させる
    getcwd(cwd, sizeof(cwd));
    Config config;
    if (string(cwd).find("config") == string::npos)
        config = Config("./srcs/config/config/unit-test/subject.nginx.conf");
    else
        config = Config("./config/subject.nginx.conf");
    CHECK(config.http->client_max_body_size == 10);
    CHECK(config.http->server[0]->listen == "80");
    CHECK(config.http->server[0]->server_name == "example.com");
    CHECK(config.http->server[0]->location.size() == 3);
    CHECK(config.http->server[0]->location[0]->urls[0] == "/");
    CHECK(config.http->server[0]->location[0]->autoindex == true);
    CHECK(config.http->server[0]->location[0]->get_error_page("404") == "/404.html");
    CHECK(config.http->server[0]->location[0]->index == "index.html");
    REQUIRE(config.http->server[0]->location[0]->limit_except != NULL);
    REQUIRE(config.http->server[0]->location[0]->limit_except[0].methods.size() == 1);
    CHECK(config.http->server[0]->location[0]->limit_except[0].methods[0] == "GET");
    CHECK(config.http->server[0]->location[0]->limit_except[0].deny_all == true);
    CHECK(config.http->server[0]->location[0]->limit_except[0].deny_list.size() == 0);
    CHECK(config.http->server[0]->location[1]->urls[0] == "/static");
    CHECK(config.http->server[0]->location[1]->root == "/var/www/html");
    CHECK(config.http->server[0]->location[2]->urls[0] == "\\.php$");
    CHECK(config.http->server[0]->location[2]->cgi_pass == "/var/run/php/php");
}

TEST_CASE("Config: empty")
{
    CHECK_THROWS_AS(Config config("", true), exception);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    if (string(cwd).find("config") == string::npos)
        CHECK_THROWS_AS(Config("./srcs/config/config/unit-test/multiple_default_server.conf"), exception);
    else
        CHECK_THROWS_AS(Config("./config/unit-test/multiple_default_server.conf"), exception);
}

TEST_CASE("Config: get_default_server")
{
    CHECK_THROWS_AS(Config config("", true), exception);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    Config config;
    if (string(cwd).find("config") == string::npos) {
        config = Config("./srcs/config/config/unit-test/multiple_server.nginx.conf");
    } else {
        config = Config("./config/unit-test/multiple_server.nginx.conf");
    }
    CHECK(config.get_default_server().listen == "8080");
}

TEST_CASE("Config: get_server from single server")
{
    Config config("http { client_max_body_size 90;server { listen 80; server_name test;} }", true);
    Server const* s = config.get_server("80", "test");
    CHECK(s->listen == "80");
    CHECK(s->server_name == "test");
}

TEST_CASE("Config: get_server from multi server")
{
    Config config("http { client_max_body_size 90;server { listen 80; server_name test;} server{listen 90; "
                  "server_name test2;} }",
                  true);
    Server const* s = config.get_server("90", "test2");
    CHECK(s->listen == "90");
    CHECK(s->server_name == "test2");
}

TEST_CASE("Config: get_server from multi server same name")
{
    Config config("http { client_max_body_size 90;server { listen 80; server_name test;} server{listen 90; "
                  "server_name test;} }",
                  true);
    Server const* s = config.get_server("90", "test");
    CHECK(s->listen == "90");
    CHECK(s->server_name == "test");
}

TEST_CASE("Config: get_server from multi server same port")
{
    Config config("http { client_max_body_size 90;server { listen 80; server_name test;} server{listen 80; "
                  "server_name test2;} }",
                  true);
    Server const* s = config.get_server("80", "test2");
    CHECK(s->listen == "80");
    CHECK(s->server_name == "test2");
}

TEST_CASE("Config: get_server not found")
{
    Config config("http { client_max_body_size 90;server { listen 80; server_name test;} server{listen 80; "
                  "server_name test21;} }",
                  true);
    CHECK(config.get_server("900", "test2") == NULL);
}

TEST_CASE("Config: get_location")
{
    Config config("http {"
                  "    client_max_body_size 10M;"
                  ""
                  "    server {"
                  "        listen 80 default_server;"
                  "        server_name example.com;"
                  ""
                  "        location / {"
                  "            autoindex on;"
                  "            error_page 404 /404.html;"
                  "            index index.html;"
                  "            limit_except GET {"
                  "                deny all;"
                  "            }"
                  "        }"
                  ""
                  "        location /static {"
                  "            root /var/www/html;"
                  "        }"
                  "    }"
                  "    server {"
                  "        listen 8080;"
                  "        server_name example.com;"
                  ""
                  "        location / {"
                  "            autoindex on;"
                  "            error_page 404 /404.html;"
                  "            index index2.html;"
                  "            limit_except GET {"
                  "                deny all;"
                  "            }"
                  "        }"
                  ""
                  "        location /static {"
                  "            root /var/www/html;"
                  "        }"
                  "    }"
                  "}",
                  true);
    Location const* l = config.get_location("80", "example.com", "/");
    REQUIRE(l != NULL);
    CHECK(l->urls.size() == 1);
    CHECK(l->urls[0] == "/");
    CHECK(l->autoindex == true);
    CHECK(l->get_error_page("404") == "/404.html");
    CHECK(l->index == "index.html");
    CHECK(l->limit_except->methods[0] == "GET");
    CHECK(l->limit_except->deny_all == true);
    CHECK(l->limit_except->deny_list.size() == 0);
    l = config.get_location("80", "example.com", "/static/test.html");
    REQUIRE(l != NULL);
    CHECK(l->urls.size() == 1);
    CHECK(l->urls[0] == "/static");
    CHECK((*l)["root"][0] == "/var/www/html");
    CHECK(l->root == "/var/www/html");
    CHECK(l->autoindex == false);
    l = config.get_location("8080", "example.com", "/");
    REQUIRE(l != NULL);
    CHECK(l->urls.size() == 1);
    CHECK(l->urls[0] == "/");
    CHECK(l->autoindex == true);
    CHECK(l->get_error_page("404") == "/404.html");
    CHECK(l->index == "index2.html");
    CHECK(l->limit_except[0].methods[0] == "GET");
    CHECK(l->limit_except[0].deny_all == true);
    CHECK(l->limit_except[0].deny_list.size() == 0);
}

#endif /* UNIT_TEST */
