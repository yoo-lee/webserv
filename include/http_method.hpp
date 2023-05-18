#ifndef HTTP_METHOD_H
#define HTTP_METHOD_H

#include <ostream>
#include <string>
using std::ostream;
using std::string;

class HttpMethod
{
  public:
    typedef enum E_METHOD {
        GET,
        POST,
        PUT,
        HEAD,
        DELETE,
        OPTIONS,
        TRACE,
        CONNECT,
        NG,
        INVALID,
    } METHOD;
    METHOD get_method() const;
    string get_str() const;
    static std::string method_to_str(HttpMethod::METHOD method);
    static HttpMethod::METHOD str_to_method(std::string method);
    HttpMethod();
    HttpMethod(METHOD method);
    HttpMethod(string method);
    HttpMethod(const HttpMethod& h);
    ~HttpMethod();
    HttpMethod& operator=(const HttpMethod& x);
    bool operator==(HttpMethod::METHOD method) const;
    friend std::ostream& operator<<(ostream& os, HttpMethod& hm);

  private:
    METHOD _method;
};

#endif /* HTTP_METHOD_H */
