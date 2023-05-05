#include "Config.hpp"

using std::ifstream;
using std::istreambuf_iterator;
using std::runtime_error;

Config::Config(string file_text, bool is_file_text)
{
    init(file_text);
}

Config::Config(const char *file_path)
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
    vector<Statement const *> root = parser.get_root();
    if (root.size() != 1)
        throw runtime_error("Invalid config file. May have no root or multiple roots.");
    http = new HTTP(root[0]);
}

Config::~Config()
{
    delete http;
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("Config: constructor test")
{
    Config config("http { client_max_body_size 90;server { listen 80;} }", true);
    CHECK(config.http->client_max_body_size == 90);
    CHECK(config.http->server[0]->listen == 80);
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

TEST_CASE("Config: Total test")
{
    Config config("./config/subject.nginx.conf");
    CHECK(config.http->client_max_body_size == 10);
    CHECK(config.http->server[0]->listen == 80);
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

#endif /* UNIT_TEST */