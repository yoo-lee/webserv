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

class Config
{
  private:
  public:
    HTTP http;
    Config(std::string file_text, bool is_file_text);
    Config(const char *file_path);
    void init(std::string file_text);
    ~Config();
};

#endif /* CONFIG_H */
