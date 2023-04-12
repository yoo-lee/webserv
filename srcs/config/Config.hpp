#ifndef CONFIG_H
#define CONFIG_H
#include <fstream>
#include <string>
#include <vector>
#include "Lexer.hpp"

class Config {
   private:
    std::string _file_text;
    std::string data;

   public:
    Lexer lexer;
    Config(std::string file_path);
    ~Config(){};
};

Config::Config(std::string file_text)
    : _file_text(file_text), lexer((_file_text)) {}

#ifdef TEST
#include <fstream>
int main() {
    // test.nginx.confから読み取ってconfigに渡す
    std::ifstream file("sample.nginx.conf");
    std::string str;
    char c;
    while (file.get(c)) {
        str += c;
    }
    file.close();
    Config config(str);
    std::vector<Token*> token_list = config.lexer.getTokenList();
    for (std::size_t i = 0; i < token_list.size(); i++) {
        std::cout << *token_list[i] << std::endl;
    }
    return 0;
}
#endif

#endif /* CONFIG_H */
