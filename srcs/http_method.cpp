#include "http_method.hpp"

HttpMethod::METHOD HttpMethod::get_method() const
{
    return _method;
}

string HttpMethod::get_str() const
{
    return method_to_str(_method);
}

HttpMethod::HttpMethod()
{
    _method = HttpMethod::INVALID;
}

HttpMethod::HttpMethod(METHOD method)
{
    _method = method;
}

HttpMethod::HttpMethod(string method)
{
    _method = str_to_method(method);
}

HttpMethod::HttpMethod(const HttpMethod& h) : _method(h._method) {}

HttpMethod::~HttpMethod() {}

std::string HttpMethod::method_to_str(HttpMethod::METHOD method)
{
    if (method == HttpMethod::GET) {
        return ("GET");
    } else if (method == HttpMethod::POST) {
        return ("POST");
    } else if (method == HttpMethod::PUT) {
        return ("PUT");
    } else if (method == HttpMethod::HEAD) {
        return ("HEAD");
    } else if (method == HttpMethod::DELETE) {
        return ("DELETE");
    } else if (method == HttpMethod::OPTIONS) {
        return ("OPTIONS");
    } else if (method == HttpMethod::TRACE) {
        return ("TRACE");
    } else if (method == HttpMethod::CONNECT) {
        return ("CONNECT");
    }
    return ("NG");
}

HttpMethod::METHOD HttpMethod::str_to_method(std::string method)
{
    if (method == "GET") {
        return HttpMethod::GET;
    } else if (method == "POST") {
        return HttpMethod::POST;
    } else if (method == "PUT") {
        return HttpMethod::PUT;
    } else if (method == "HEAD") {
        return HttpMethod::HEAD;
    } else if (method == "DELETE") {
        return HttpMethod::DELETE;
    } else if (method == "OPTIONS") {
        return HttpMethod::OPTIONS;
    } else if (method == "TRACE") {
        return HttpMethod::TRACE;
    } else if (method == "CONNECT") {
        return HttpMethod::CONNECT;
    }
    return HttpMethod::INVALID;
}

HttpMethod& HttpMethod::operator=(const HttpMethod& x)
{
    if (&x != this)
        _method = x._method;
    return (*this);
}

bool HttpMethod::operator==(HttpMethod::METHOD method) const
{
    return (_method == method);
}

ostream& operator<<(ostream& os, HttpMethod& hm)
{
    os << hm.get_str();
    return (os);
}