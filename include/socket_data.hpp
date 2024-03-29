#ifndef SOCKET_DATA_HPP
#define SOCKET_DATA_HPP

#include "Config.hpp"
#include "raw_request_reader.hpp"
#include "splitted_string.hpp"
#include <map>
#include <string>

using std::string;

class SocketData
{
  public:
    SocketData(Config const& config);
    ~SocketData();

    bool increment_timeout(int time);
    void clear_timeout();

  protected:
    const static int _timeout = 200;
    int _timeout_cnt;

    Config const* _config;
};

#endif
