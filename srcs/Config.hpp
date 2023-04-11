#ifndef CONFIG_H
#define CONFIG_H

/**
nginx-config ::= statement*

statement ::= simple-statement | block-statement

simple-statement ::= directive parameters? ';'

block-statement ::= directive parameters? '{' statement* '}'

directive ::= identifier

parameters ::= parameter | parameter parameters

parameter ::= identifier | string | number

identifier ::= [a-zA-Z_][a-zA-Z0-9_]*

string ::= '"' any-char* '"'

number ::= [0-9]+

any-char ::= 任意の文字
*/
#include <fstream>
#include <string>
#include <vector>
class Config {
   private:
    std::string _file_text;
    std::string data;
    std::vector<std::string> lexer();

   public:
    Config(std::string file_path);
    ~Config();
};

Config::Config(std::string file_text) : _file_text(file_text) {}
Config::~Config() {}
std::vector<std::string> Config::lexer() {}

#endif /* CONFIG_H */
