#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
class Token {
   public:
    ~Token();
    enum Type {
        COMMA,
        SEMI,
        DQUOTE,
        SQUOTE,
        STRING,
        LCURLY,
        RCURLY,
        INT,
        ID,
        WHITE_SPACE,
        ANYCHAR,
    };
    Token(std::string str, Type type) : _str(str), _type(type) {}
    Token(char c, Type type) : _str(std::string(&c, 1)), _type(type) {}
    static std::string getTypeName(Type type) {
        const char* type_name_list[] = {
            "COMMA",  "SEMI", "DQUOTE", "SQUOTE",      "STRING", "LCURLY",
            "RCURLY", "INT",  "ID",     "WHITE_SPACE", "ANYCHAR"};
        if (type <= ANYCHAR)
            return type_name_list[type];
        return "UNKNOWN";
    }
    Type getType() const { return _type; }
    std::string getStr() const { return _str; }

   private:
    Type _type;
    std::string _str;
};

Token::~Token() {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.getStr() << " : " << Token::getTypeName(token.getType());
    return os;
}

#endif /* TOKEN_H */
