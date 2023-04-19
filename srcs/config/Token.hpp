#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
class Token
{
  public:
    enum Type
    {
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
        NONE
    };
    Token(std::string str, Type type);
    Token(char c, Type type);
    ~Token();
    Type getType() const;
    static std::string getTypeName(Token::Type type);
    std::string getStr() const;
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
    bool operator==(const Token &rhs) const;
    bool operator!=(const Token &rhs) const;
    Token &operator=(const Token &rhs);

  private:
    Type _type;
    std::string _str;
};

#endif /* TOKEN_H */
