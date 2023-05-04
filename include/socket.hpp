#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "request.hpp"
#include "response.hpp"
#include "fd_manager.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <iostream>
//#include <sys/types.h>
//#include <netinet/in.h>
//
//

typedef struct clientinfo{
    int fd;
    char buf[1024];
    int n;
    int state;
}s_clientinfo;

class Socket
{
    public:
        Socket();
        Socket(std::string port);
        //Socket(const Socket &socket);
        Socket(const Socket &socket);
        Socket& operator=(const Socket &socket);
        virtual ~Socket();
        virtual void communication();
        void close_fd();
        int getSockFD();
        //Request *recv();
        Request *recv(int fd);
        bool send(int fd);
        int accept_request();
        void set_response(int fd, Response *res);
        Response *get_response(int fd);
        void erase_request(int fd);
        void erase_response(int fd);
        std::vector<int> timeout(int time);
        void delete_fd_map(int fd);

        //template <typename T>
        //void increment_timeout(T& map, int timeout);
        //void increment_timeout(std::map<int, Request*> &map, int timeout);
        //void increment_timeout(std::map<int, Response*> &map, int timeout);
    protected:
        int sock_fd;
        //int fd;
        const static int _SOCKET_NUM = 10;
        //list<int> _fd_list;
        //std::map <int, SocketData*> _fd_map;
        //std::map <int, Request*> _fd_req_map;
        //std::map <int, Response*> _fd_res_map;
        std::map <int, FDManager*> _fd_map;
        std::string port;
        virtual void init();
        virtual int makeSocket();
        virtual void setAddrInfo(struct addrinfo &info);
        //Request *req;
        //std::map <int, Response*> res_map;
        //Response *res;
        struct epoll_event ev;
        s_clientinfo clientinfo;
        bool send_err(std::string err);

        template <typename T>
        bool increment_timeout(T& obj, int time);
        //struct sockaddr_in hint;
};


#include <iostream>
#include <string>
#include <stdio.h>

using std::cout;
using std::endl;
template <typename T>
//template <template <typename, typename T> class Container<T> >
bool Socket::increment_timeout(T& obj, int time)
{
    //int fd;
    SocketData *socket_data = &obj;
    printf("socket_data=%p\n", socket_data);
    //typename T::iterator iter = map.begin();
    //typename T::iterator end = map.end();
    //std::vector<int> delete_fd;

    bool exceed;
    //for(; iter != end;iter++){
        //cout << "timeout No.4 time:" << time << std::endl;
        //fd = iter->first;
        //socket_data = iter->second;
        exceed = socket_data->increment_timeout(time);
        //std::cout << "timeout No.4-1" << std::endl;
        if (exceed){
            //std::cout << "timeout No.5" << std::endl;
            //delete socket_data;
            //jFDManager *fd_m = this->_fd_map[fd];
            //fd_m->close_fd();
            //delete fd_m;
            //this->_fd_map.erase(fd);
            //map.erase(delete_fd[i]);
            //std::cout << "timeout No.6" << std::endl;
            //delete socket_data;
            //std::cout << "timeout No.7" << std::endl;
            //close(fd);
            //delete_fd.push_back(fd);
            //std::cout << "timeout No.6" << std::endl;
            //std::cout << "timeout No.8" << std::endl;
            return (true);
        }
    //}
    //for(size_t i = 0; i < delete_fd.size();i++){
        //fd = delete_fd[i];
        //socket_data = iter->second;
        //std::cout << "timeout No.7" << std::endl;
    //}
    cout << "timeout No.9" << std::endl;
    return (false);
}


#endif
