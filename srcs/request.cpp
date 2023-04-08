#include "request.hpp"
#include "split.hpp"
#include "get_next_line.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;

Request::Request(int fd_) : fd(fd_), buf_size(0)
{
    this->parse();
}

Request::~Request()
{
    close(this->fd);
}

METHOD Request::identify_method(string method)
{
    if (method == "GET"){
        return (GET);
    }else if (method == "POST"){
        return (POST);
    }else if (method == "PUT"){
        return (PUT);
    }else if (method == "HEAD"){
        return (HEAD);
    }else if (method == "DELETE"){
        return (DELETE);
    }else if (method == "OPTIONS"){
        return (OPTIONS);
    }else if (method == "TRACE"){
        return (TRACE);
    }else if (method == "CONNECT"){
        return (CONNECT);
    }
    return (NG);
}

std::string Request::identify_method(METHOD method)
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

void Request::print_request()
{
    cout << "method: " << identify_method(this->method) << endl;
    cout << "uri: " << this->uri << endl;
    cout << "version: " << this->version << endl;

    cout << "headers size:" << this->headers.size() << endl;;
    map<string, string>::iterator ite = this->headers.begin();
    map<string, string>::iterator end = this->headers.end();
    for(; ite != end; ite++)
    {
        cout << (*ite).first << " : " << (*ite).second << endl;
    }
}


void Request::parse()
{

    GetNextLine gnl(this->fd);
    string str = gnl.getline();
    Split sp(str, " ");
    if (sp.size() != 3){
        throw std::exception();
    }
    Split::iterator ite = sp.begin();
    this->method = identify_method(*ite);
    this->uri = *(++ite);
    this->version = *(++ite);
    string header;
    string value;
    std::string::size_type pos;
    while((str != gnl.last_str)) {
        str = gnl.getline();
        pos = str.find(" ");
        if (pos == string::npos || str.size() <= 0)
        {
            break;
        }
        header = str.substr(0, pos-1);
        if(header.size() < 2)
        {
            break;
        }
        this->headers.insert(make_pair(str.substr(0, pos-1), str.substr(pos)));
    }
    this->buf_size = gnl.get_extra_buf(this->extra_buf);
}

const METHOD Request::get_method()
{
    return (this->method);
}

const string &Request::get_uri()
{
    return (this->uri);
}

const string &Request::get_version()
{
    return (this->version);
}

const map<string, string> &Request::get_headers()
{
    return (this->headers);
}

int Request::read_buf(char *cp_buf)
{
    if (this->buf_size > 0)
    {
        int tmp = this->buf_size;
        while(this->buf_size--){
            cp_buf[this->buf_size] = this->extra_buf[this->buf_size];
        }
        return (tmp);
    }
    ssize_t tmp = (recv(this->fd, cp_buf, BUF_MAX, MSG_DONTWAIT));
    return (tmp);
}

