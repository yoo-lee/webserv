#include "request.hpp"
#include "split.hpp"
#include "get_next_line.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <algorithm>
#include <sstream>
#include "utility.hpp"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;

Request::Request(int fd_) : fd(fd_), _body_size(0), _body(NULL),method(NG),  err_line("")
{
    this->parse();
}

Request::~Request()
{
    delete _body;
    //close(this->fd);
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
    cout << "Print Request!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "method: " << identify_method(this->method) << endl;
    cout << "uri: " << this->uri << endl;
    cout << "version: " << this->version << endl;

    cout << "headers size:" << this->headers.size() << endl;;
    map<string, string>::iterator ite = this->headers.begin();
    map<string, string>::iterator end = this->headers.end();
    for(; ite != end; ite++)
    {
        cout << (*ite).first << ":" << (*ite).second << endl;
    }
}

void Request::load_header(GetNextLine& gnl, char *buf)
{

    string str = gnl.getline(buf, BUF_MAX);
    if (str == gnl.last_str){
        return;
    }
    Split sp(str, " ");
    if (sp.size() != 3){
        cout << "size:" << sp.size() << endl;
        cout << "str:[" << str << "]" << endl;
        str = gnl.getline(buf, BUF_MAX);
        cout << "str:[" << str << "]" << endl;
        cout << "Error:not 3 factor" << endl;
        throw std::exception();
    }
    Split::iterator ite = sp.begin();
    this->method = identify_method(*ite);
    this->uri = *(++ite);
    const char *path = this->uri.c_str();
    size_t cnt = 0;
    while (path && *path){
        if(*path != '/')
            break;
        cnt++;
        path++;
    }
    this->path = this->uri.substr(cnt);
    this->path = Utility::delete_space(this->path);
    this->version = *(++ite);
    this->version = Utility::delete_space(this->version);
    string header;
    string value;
    std::string::size_type pos;
    while((str != gnl.last_str)) {
        str = gnl.getline(buf, BUF_MAX);
        pos = str.find(":");
        if (pos == string::npos || str.size() <= 0)
        {
            break;
        }
        header = str.substr(0, pos);
        header = Utility::delete_space(header);
        header = Utility::delete_space(header);
        if(header.size() < 2)
        {
            break;
        }
        std::transform(header.begin(), header.end(), header.begin(), tolower);
        value = str.substr(pos+1);
        value = Utility::delete_space(value);
        this->headers.insert(make_pair(header, value));
    }

}

void Request::parse()
{
    char extra_buf[BUF_MAX];
    GetNextLine gnl(this->fd);
    load_header(gnl, extra_buf);
    this->print_request();
    load_body(gnl, extra_buf);

}

METHOD Request::get_method()
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


void Request::load_body(GetNextLine& gnl, char *buf)
{
    string size_str = this->search_header("content-length");
    int size = -1;
    if (size_str != ""){
        std::stringstream ss;
        ss << size_str;
        ss >> size;
    }
    this->_body_size = size;
    int extra_buf_size = gnl.get_extra_buf(&buf);
    string transfer = this->search_header("transfer-encoding");
    string accept_char = this->search_header("accept-charset");
    /*
    if (accept_char == "utf-8")
        cout << "OKOKOKOK accept-charset:" << accept_char << endl;
    else {
        cout << "NGNGNGNGNG [accept-charset] is not utf-8:" << accept_char << endl;
        cout << "NGNGNGNGNG [accept-charset] is not utf-8 size:" << accept_char.size() << endl;
    }
    cout << "transfer-encoding:" << transfer << endl;
    */
    //size -= extra_buf_size;
    //  transfer-encoding:chunked
    if (transfer == "chunked")
    {
        //cout << "chunked No.1" << endl;
    }
    else if (extra_buf_size > 0)
    {
        //cout << "content-length No.2 size:" << this->_body_size << endl;
        this->_body = new char(this->_body_size);
        Utility::memcpy(this->_body, buf, extra_buf_size);
        string str = gnl.getline(&(this->_body[extra_buf_size]), this->_body_size - extra_buf_size);
    }
}

int Request::read_buf(char **cp_buf)
{
    //char *test_nu = NULL;
    *cp_buf = this->_body;
    //cout << "this->_body address:" << &(this->_body) << endl;
    //cout << "test NULL:" << test_nu << endl;
    //cout << "this->_body:" << (this->_body) << endl;
    return (this->_body_size);
    /*
    if (this->buf_size > 0)
    {
        int tmp = this->buf_size;
        while(this->buf_size--){
            cp_buf[this->buf_size] = this->extra_buf[this->buf_size];
        }
        return (tmp);
    }
    ssize_t tmp = (read_line(this->fd, cp_buf, BUF_MAX, MSG_DONTWAIT));
    return (tmp);
    */
}

const string& Request::get_path()
{
    return (this->path);
}

string Request::get_ip_address()
{
    //todo
    return ("127.0.0.1");
}

string Request::get_domain()
{
    //todo
    return ("test.com");
}

string Request::search_header(string header)
{
    (void)header;
    std::map<std::string, std::string>::const_iterator ite;
    ite = this->headers.find(header);
    if (ite == this->headers.end())
    {
        return ("");
    }
    return (ite->second);
}
