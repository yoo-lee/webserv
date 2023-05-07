#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

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
    void communicate();

  private:
    void init();
};

#endif /* TCP_SOCKET_H */
