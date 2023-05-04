#include "Config.hpp"
Config::Config(std::string file_text, bool is_file_text)
{
    init(file_text);
}

Config::Config(const char *file_path)
{
    std::ifstream ifs(file_path);
    if (!ifs)
        throw std::runtime_error("Config: File not found: " + std::string(file_path));
    std::string file_text((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    init(file_text);
}

void Config::init(std::string file_text)
{
    Parser parser(file_text);
    std::vector<Statement *> root = parser.get_root();
    if (root.size() != 1)
        throw std::runtime_error("Invalid config file. May have no root or multiple roots.");
    http = *(new HTTP(root[0]));
}

Config::~Config()
{
    // delete &http;
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("Config: constructor test")
{
    Config config("http { client_max_body_size 90;server { listen 80;} }", true);
    CHECK(config.http.client_max_body_size == 90);
    CHECK(config.http[0].listen == 80);
    CHECK(config.http[0].server_name == "");
    CHECK(config.http[0].location.size() == 0);
}

TEST_CASE("Config: Total test")
{
    Config config("./config/subject.nginx.conf");
}

#endif /* UNIT_TEST */