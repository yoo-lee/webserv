#include "Config.hpp"
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
    if (string(cwd).find("config") == string::npos) {
        Config config("./srcs/config/config/unit-test/subject.nginx.conf");
        CHECK(config.http->client_max_body_size == 10);
        CHECK(config.http->server[0]->listen == "80");
        CHECK(config.http->server[0]->server_name == "example.com");
        CHECK(config.http->server[0]->location.size() == 3);
        CHECK(config.http->server[0]->location[0]->urls[0] == "/");
        CHECK(config.http->server[0]->location[0]->autoindex == true);
        CHECK(config.http->server[0]->location[0]->error_page["404"] == "/404.html");
        CHECK(config.http->server[0]->location[0]->index == "index.html");
        CHECK(config.http->server[0]->location[0]->limit_except[0].methods[0] == "GET");
        CHECK(config.http->server[0]->location[0]->limit_except[0].deny_all == true);
        CHECK(config.http->server[0]->location[0]->limit_except[0].deny_list.size() == 0);
        CHECK(config.http->server[0]->location[1]->urls[0] == "/static");
        CHECK((*(config.http->server[0]->location[1]))["root"][0] == "/var/www/html");
        CHECK(config.http->server[0]->location[2]->urls[0] == "\\.php$");
        CHECK((*(config.http->server[0]->location[2]))["cgi_pass"][0] == "/var/run/php/php");
    } else {
        Config config("./config/subject.nginx.conf");
        CHECK(config.http->client_max_body_size == 10);
        CHECK(config.http->server[0]->listen == "80");
        CHECK(config.http->server[0]->server_name == "example.com");
        CHECK(config.http->server[0]->location.size() == 3);
        CHECK(config.http->server[0]->location[0]->urls[0] == "/");
        CHECK(config.http->server[0]->location[0]->autoindex == true);
        CHECK(config.http->server[0]->location[0]->error_page["404"] == "/404.html");
        CHECK(config.http->server[0]->location[0]->index == "index.html");
        CHECK(config.http->server[0]->location[0]->limit_except[0].methods[0] == "GET");
        CHECK(config.http->server[0]->location[0]->limit_except[0].deny_all == true);
        CHECK(config.http->server[0]->location[0]->limit_except[0].deny_list.size() == 0);
        CHECK(config.http->server[0]->location[1]->urls[0] == "/static");
        CHECK((*(config.http->server[0]->location[1]))["root"][0] == "/var/www/html");
        CHECK(config.http->server[0]->location[2]->urls[0] == "\\.php$");
        CHECK((*(config.http->server[0]->location[2]))["cgi_pass"][0] == "/var/run/php/php");
    }
}

TEST_CASE("Config: empty")
{
    CHECK_THROWS_AS(Config config("", true), exception);
    // config内でmakeした場合とsrcsでmakeした場合でconfへのパスが変わるので分岐させる
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
    // config内でmakeした場合とsrcsでmakeした場合でconfへのパスが変わるので分岐させる
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    if (string(cwd).find("config") == string::npos) {
        Config config("./srcs/config/config/unit-test/multiple_server.nginx.conf");
        CHECK(config.get_default_server().listen == "8080");
    } else {
        Config config("./config/unit-test/multiple_server.nginx.conf");
        CHECK(config.get_default_server().listen == "8080");
    }
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
    Config config(
        "http { client_max_body_size 90;server { listen 80; server_name test;} server{listen 90; server_name test;} }",
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

#endif /* UNIT_TEST */
