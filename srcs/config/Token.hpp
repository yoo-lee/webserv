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
        COMMENT,
        NONE
    };
    Token();
    Token(std::string str, Type type);
    Token(char c, Type type);
    Token(const Token &token);
    ~Token();
    Type get_type() const;
    static std::string get_typename(Token::Type type);
    std::string get_str() const;
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
    bool operator==(const Token &rhs) const;
    bool operator!=(const Token &rhs) const;
    Token &operator=(const Token &rhs);

  private:
    Type _type;
    std::string _str;
};

#endif /* TOKEN_H */
