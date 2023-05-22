#include "socket_data.hpp"
#include "Config.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

SocketData::SocketData(Config const& config) : _timeout_cnt(0), _config(&config) {}

SocketData::~SocketData() {}

bool SocketData::increment_timeout(int time)
{
    this->_timeout_cnt = this->_timeout_cnt + time;
    if (_timeout_cnt > _timeout) {
        return (true);
    }
    return (false);
}

void SocketData::clear_timeout()
{
    _timeout_cnt = 0;
}
