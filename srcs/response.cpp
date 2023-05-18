#include "response.hpp"
#include "Config.hpp"
#include "http_method.hpp"
#include <cstring>
#include <stdexcept>
#include <utility.hpp>

/*
Response::Response()
{
    ;
}
*/

// Constructor for Response class that takes a Request object as argument
Response::Response(Request& request) : SocketData(*(request.get_config())), _request(request)
{
    // Set the status code based on the request method
    HttpMethod method = request.get_method();
    if (method == HttpMethod::GET) {
        set_status(200);
    } else {
        set_status(501);
    }

    // Set the response body
    _body = "<html><body><h1>Hello, World!</h1></body></html>";

    // Build the response
    build_response();
}

Response::Response(Request& request, int code) : SocketData(*(request.get_config())), _request(request), _code(code)
{
    _res = "HTTP/1.1 " + Utility::to_string(_code) + " " + get_status_message(_code) + "\r\n";
    _res += "Content-Type: text/html\r\n";
    _res += "Content-Length: 0\r\n\r\n";
}

// HTTP response as string
std::string Response::get_response_string() const
{
    return _res;
}

// Length of HTTP response
int Response::getLen() const
{
    return _res.length();
}

// Set status code for HTTP response
void Response::set_status(int code)
{
    _code = code;
}

// Build HTTP response
void Response::build_response()
{
    // header 文字列で返す。
    //  Set status line
    _res = "HTTP/1.1 " + Utility::to_string(_code) + " " + get_status_message(_code) + "\r\n";

    // tmp = Utility::to_string(_body.size());
    //  Set response headers
    _res += "Content-Type: text/html\r\n";
    _res += "Content-Length: " + Utility::to_string(_body.size()) + "\r\n";

    // char＊ で渡す。
    //  file名
    //  Set response body
    // NEW しない為にファイルを渡す
    // CONFIG チェックの上相対パスを送付する。
    _res += "\r\n";
    _res += _body;

    // Terminate response
    _res += "\r\n";
}

// Get HTTP response status message for a given status code
std::string Response::get_status_message(int code)
{
    switch (code) {
    case 200:
        return "OK";
    case 204:
        return "Not Content";
    case 206:
        return "Partial Content";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Not Found";
    case 303:
        return "See Other";
    case 304:
        return "Not Modified";
    case 307:
        return "Temporary Redirect";
    case 400:
        return "Bad Request";
    case 401:
        return "Bad Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 500:
        return "Internal Server Error";
    case 503:
        return "Service Unavailable";
    // Add other status codes as needed
    default:
        return "Unknown Status";
    }
}