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
        LCURLY,
        RCURLY,
        INT,
        ID,
        WS,
        ANYCHAR,
    };
    Token(std::string str, Type type) : _str(str), _type(type) {}
    Token(char c, Type type) : _str(std::string(&c, 1)), _type(type) {}
    static std::string getTypeName(Type type) {
        if (type == COMMA)
            return "COMMA";
        if (type == SEMI)
            return "SEMI";
        if (type == DQUOTE)
            return "DQUOTE";
        if (type == LCURLY)
            return "LCURLY";
        if (type == RCURLY)
            return "RCURLY";
        if (type == INT)
            return "INT";
        if (type == ID)
            return "ID";
        if (type == WS)
            return "WS";
        if (type == ANYCHAR)
            return "ANYCHAR";
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
