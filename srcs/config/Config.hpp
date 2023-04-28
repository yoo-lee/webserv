#ifndef CONFIG_H
#define CONFIG_H
#include "Events.hpp"
#include "Http.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

class Config
{
  private:
  public:
    Events events;
    HTTP http;
    Config(std::string file_path);
    Config(const char *file_path);
    ~Config(){};
};

Config::Config(std::string file_text) {}

Config::Config(const char *file_path)
{
    // file_pathからファイルを読み込む
    std::ifstream ifs(file_path);
    if (!ifs)
        throw std::runtime_error("file not found");
    std::string file_text((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    Parser parser(file_text);
}

#endif /* CONFIG_H */
