#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>

using std::ostream;
using std::string;

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
    Token(string str, Type type);
    Token(char c, Type type);
    Token(Token const &token);
    ~Token();
    Type get_type() const;
    static string get_typename(Token::Type type);
    string get_str() const;
    friend ostream &operator<<(ostream &os, Token const &token);
    bool operator==(Token const &rhs) const;
    bool operator!=(Token const &rhs) const;
    Token &operator=(Token const &rhs);

  private:
    Type _type;
    string _str;
};

#endif /* TOKEN_H */
