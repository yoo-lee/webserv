#include "request.hpp"
#include "split.hpp"
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

Request::Request(int fd_) : fd(fd_)
{
    //cout << "Request constractor" << endl;
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


    string str;
    dup2(this->fd, 0);
    getline(cin, str);
    Split sp(str, " ");
    try {
        Split::iterator ite = sp.begin();
        if (sp.size() != 3){
            throw std::exception();
        }

        this->method = identify_method(*ite);
        this->uri = *(++ite);
        this->version = *(++ite);
        string header;
        string value;
        //getline(cin, str);

        while((getline(cin, header,  ' ')) ) {
            //cout << "header:" << header << endl;
            getline(cin, value);
            //cout << "value:" << value << endl;
            int a1 = header.c_str()[0];
            int a2 = header.c_str()[1];
            if (a1 == 13 && a2 == 10){
                break;
            }
            this->headers.insert(make_pair(header.substr(0, header.size()-1), value));
            //cout << "next" << endl << std::flush ;
        }
    } catch (std::exception &e){
        e.what();
    }
    this->print_request();
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

