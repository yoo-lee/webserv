#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "socket.hpp"
#include <netdb.h>
#include <string>
#include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>

class TCPSocket : public Socket
{
  public:
    TCPSocket();
    TCPSocket(const TCPSocket& socket);
    TCPSocket& operator=(const TCPSocket& socket);
    ~TCPSocket();
    void communication();
  private:
    void init();
};

#endif
