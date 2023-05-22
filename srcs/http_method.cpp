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
    if (_method == HttpMethod::INVALID) {
        throw std::invalid_argument(
            "Invalid HTTP method. (webserv is not support HEAD, PUT, CONNECT, OPTIONS, TRACE, PATCH method)");
    }
}

HttpMethod::HttpMethod(const HttpMethod& h) : _method(h._method) {}

HttpMethod::~HttpMethod() {}

std::string HttpMethod::method_to_str(HttpMethod::METHOD method)
{
    if (method == HttpMethod::GET) {
        return ("GET");
    } else if (method == HttpMethod::POST) {
        return ("POST");
    } else if (method == HttpMethod::DELETE) {
        return ("DELETE");
    }
    return "INVALID";
}

HttpMethod::METHOD HttpMethod::str_to_method(std::string method)
{
    if (method == "GET") {
        return HttpMethod::GET;
    } else if (method == "POST") {
        return HttpMethod::POST;
    } else if (method == "DELETE") {
        return HttpMethod::DELETE;
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