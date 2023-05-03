
#include "response.hpp"
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
Response::Response(Request& request) : _request(request) {

    // Set the status code based on the request method
    METHOD method = request.get_method();
    if (method == GET) {
        setStatus(200);
    } else {
        setStatus(501);
    }

    // Set the response body
    _body = "<html><body><h1>Hello, World!</h1></body></html>";

    // Build the response
    buildResponse();
}

// HTTP response as string
std::string Response::getRes() const {
    return _res;
}

// Length of HTTP response
int Response::getLen() const {
    return _res.length();
}

// Set status code for HTTP response
void Response::setStatus(int code) {
    _code = code;
}

// Build HTTP response
void Response::buildResponse()
{
	//header 文字列で返す。
    // Set status line
    _res = "HTTP/1.1 " + Utility::to_string(_code) + " " + getStatusMessage(_code) + "\r\n";

    //tmp = Utility::to_string(_body.size());
    // Set response headers
    //_res += "Content-Type: text/html\r\n";
    _res += "Content-Length: " + Utility::to_string(_body.size()) + "\r\n";


	//char＊ で渡す。
	// file名
	// Set response body
	//NEW しない為にファイルを渡す
	//CONFIG チェックの上相対パスを送付する。
    _res += "\r\n";
    _res += _body;

    // Terminate response
    _res += "\r\n";
}

METHOD Response::identify_method(std::string method)
{
    if (method == "GET") {
        return GET;
    } else if (method == "POST") {
        return POST;
    } else if (method == "PUT") {
        return PUT;
    } else if (method == "HEAD") {
        return HEAD;
    } else if (method == "DELETE") {
        return DELETE;
    } else if (method == "OPTIONS") {
        return OPTIONS;
    } else if (method == "TRACE") {
        return TRACE;
    } else if (method == "CONNECT") {
        return CONNECT;
    }
    return INVALID;
}

std::string Response::identify_method(METHOD method)
{
    if (method == GET){
        return ("GET");
    }else if (method == POST){
        return ("POST");
    }else if (method == PUT){
        return ("PUT");
    }else if (method == HEAD){
        return ("HEAD");
    }else if (method == DELETE){
        return ("DELETE");
    }else if (method == OPTIONS){
        return ("OPTIONS");
    }else if (method == TRACE){
        return ("TRACE");
    }else if (method == CONNECT){
        return ("CONNECT");
    }
    return ("NG");
}

// Get HTTP response status message for a given status code
std::string Response::getStatusMessage(int code) {
    switch (code) {
        case 200:
            return "OK";
        case 404:
            return "Not Found";
        // Add other status codes as needed
        default:
            return "Unknown Status";
    }
}
