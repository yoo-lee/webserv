#include "socket_data.hpp"
#include <iostream>


using std::string;
using std::cout;
using std::endl;

SocketData::SocketData() : _timeout_cnt(0)
{
    ;
}

SocketData::~SocketData()
{
    ;
}

METHOD SocketData::identify_method(string method)
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

std::string SocketData::identify_method(METHOD method)
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

bool SocketData::increment_timeout(int time)
{
    this->_timeout_cnt =this->_timeout_cnt + time;
    if (_timeout_cnt > _timeout)
    {
        return (true);
    }
    return (false);
}

void SocketData::clear_timeout()
{
    _timeout_cnt = 0;
}
