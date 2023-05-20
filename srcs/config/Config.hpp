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

using std::string;

class Config
{
  private:
  public:
    HTTP const* http;
    Config();
    Config(string file_text, bool is_file_text);
    Config(const char* file_path);
    void init(string file_text);
    ~Config();
    Server const& get_default_server() const;

    Config const& operator=(Config const& c);
};

#endif /* CONFIG_H */
