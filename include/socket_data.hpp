#ifndef SOCKET_DATA_HPP
#define SOCKET_DATA_HPP

#include "get_next_line.hpp"
#include "split.hpp"
#include <map>
#include <string>

using std::string;

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

    static string method_to_str(METHOD method);
    static METHOD str_to_method(string method);
    bool increment_timeout(int time);
    void clear_timeout();

  protected:
    const static int _timeout = 200;
    int _timeout_cnt;
};

#endif
