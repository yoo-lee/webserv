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
    Config(std::string file_path);
    ~Config(){};
};

Config::Config(std::string file_text) : _file_text(file_text) {
    Lexer lexer(_file_text);
    std::vector<Token*> token_list = lexer.getTokenList();
    for (std::size_t i = 0; i < token_list.size(); i++) {
        std::cout << *token_list[i] << std::endl;
    }
}

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
    return 0;
}
#endif

#endif /* CONFIG_H */
