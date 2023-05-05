#ifndef SOCKET_DATA_HPP
#define SOCKET_DATA_HPP

#include "get_next_line.hpp"
#include "split.hpp"
#include <map>
#include <string>

enum E_METHOD {
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
};

typedef enum E_METHOD METHOD;

class SocketData
{
  public:
    SocketData();
    ~SocketData();

    static std::string identify_method(METHOD method);
    static METHOD identify_method(std::string method);
    bool increment_timeout(int time);
    void clear_timeout();

  protected:
    const static int _timeout = 200;
    int _timeout_cnt;
};

#endif
