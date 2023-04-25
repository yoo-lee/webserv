#ifndef CONFIG_H
#define CONFIG_H
#include "Lexer.hpp"
#include "Parser.hpp"
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "Http.hpp"

class Config
{
  private:
  public:
    HTTP http;
    Config(std::string file_path);
    Config(const char *file_path);
    ~Config(){};
};

Config::Config(std::string file_text)
{
}

Config::Config(const char *file_path)
{
    // file_pathからファイルを読み込む
    std::ifstream ifs(file_path);
    if (!ifs)
        throw std::runtime_error("file not found");
    std::string file_text((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    Parser parser(file_text);
    ASTNode *root = parser.get_root();
}

#endif /* CONFIG_H */
