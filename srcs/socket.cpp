//#include "sock_fdet.hpp"
#include <string.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include "tcp_socket.hpp"
#include "string.h"
#include <unistd.h>
#include <fcntl.h>
//#include <netdb.h>
//#include <sys/types.h>

using std::cout;
using std::endl;

int Socket::makeSocket()
{
    return (socket(AF_INET, SOCK_STREAM, 0));
}

void Socket::setAddrInfo(struct addrinfo &info)
{
    info.ai_family = AF_INET;
    info.ai_flags = AI_PASSIVE;
    info.ai_socktype = SOCK_STREAM;
}

//bind
void Socket::init()
{
    this->sock_fd= makeSocket();
    //this->clientinfo;
    memset(&(this->clientinfo), 0, sizeof(s_clientinfo));
    this->ev.data.ptr = &clientinfo;
    if (this->sock_fd < 0)
    {
         cout << strerror(errno) << endl;
         //throw std::exception();
         throw std::runtime_error("Failed to create sock_fdet\n");
    }

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    setAddrInfo(hint);
    struct addrinfo *res = NULL;
    int err = getaddrinfo(NULL, this->port.c_str(), &hint, &res);
    if (err != 0)
    {
        this->close_fd();
        cout << "Error getaddrinfo() :" << gai_strerror(err) << endl;
        throw std::runtime_error("Failed to init()\n");
    }
    int yes = 1;
    setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
    if (bind(this->sock_fd, res->ai_addr, res->ai_addrlen) != 0)
    {
        this->close_fd();
        freeaddrinfo(res);
        cout << "Error bind1:" << strerror(errno) << endl;
        throw std::runtime_error("Failed to bind()\n");
    }
    freeaddrinfo(res);

    //// sysctl net.ipv4.tcp_max_syn_backlog
    //// NG in guacamole
    listen(this->sock_fd, _SOCKET_NUM);
}
//bind
// バインド（bind）とは、ソケットにIPアドレスとポート番号を関連付けることです。
Socket::Socket() : sock_fd(0), port("11112")
{
    init();
}

Socket::Socket(std::string port_) : sock_fd(0) ,port(port_)
{
    init();
}

Socket::~Socket()
{
    //delete this->req;
}
Socket::Socket(const Socket &sock_fdet) : sock_fd(sock_fdet.sock_fd), port(sock_fdet.port)
{
    init();
}
Socket& Socket::operator=(const Socket &sock_fdet)
{
    if (&sock_fdet == this)
        return (*this);
    return (*this);
}

void Socket::communication()
{
}

int Socket::getSockFD()
{
    return (this->sock_fd);
}

void Socket::close_fd()
{
    close(this->sock_fd);
}


int Socket::accept_request()
{
    struct sockaddr_in client;
    memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(client);

    int fd = accept(this->sock_fd,(struct sockaddr *)&client, &len);
    if (fd < 0)
    {
        cout << "Error accept():" << strerror(errno) << endl;
        //return ();
    }

    FDManager *fd_m = new FDManager(fd);
    //SocketData *tmp = fd_m;
    //tmp->increment_timeout(1);
    //tmp->increment_timeout(2);
    //Request *req = NULL;
    //Response *res = NULL;
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    //this->_fd_req_map.insert(std::make_pair(fd, req));
    //this->_fd_res_map.insert(std::make_pair(fd, res));
    int cur_flags = fcntl(fd, F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, cur_flags);
    return (fd);
}

/*
Request *Socket::recv(int fd)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    cout << "socket recv No.1 fd=" << this->sock_fd << endl;
    this->fd= accept(fd,(struct sockaddr *)&client, &len);
    cout << "socket recv No.2 fd=" << this->sock_fd << endl;
    if (this->fd < 0)
    {
        cout << "Error accept():" << strerror(errno) << endl;
        return (NULL);
    }
    if (this->req != NULL)
        delete this->req;
    this->req = new Request(this->fd);
    return (this->req);
}
*/

Request *Socket::recv(int fd)
{
    Request *req = this->_fd_map[fd]->get_req();
    if (req != NULL)
        return req;
    try{
        req = new Request(fd);
        this->_fd_map[fd]->insert(req);
    }catch(std::exception &e){
        req = NULL;
        this->_fd_map[fd]->insert(req);
        cout << e.what() << endl; 
        //std::string r_data = "HTTP/1.1 400 NG\n";
        //this->send(r_data);
        //return (NULL);
    }
    return (req);
}

/*
bool Socket::send_err(std::string& data)
{

    char last = '\0';
    write(this->fd, data.c_str(), data.size());
    write(this->fd, &last, 1);
    return (true);
}
*/



bool Socket::send(int fd)
{

    Response *res = get_response(fd);
    char last = '\0';
    write(fd, res->getRes().c_str(), res->getRes().size());
    write(fd, &last, 1);
    return (true);
}

void Socket::set_response(int fd, Response *res)
{
    this->_fd_map[fd]->insert(res);
    /*
    //std::map<int, Response*>::iterator res_iter = this->_fd_map[fd].get_res();
    if (res_iter == this->_fd_res_map.end())
    {
        this->_fd_res_map.insert(std::make_pair(fd, res));
    }
    else
    {
        Response *tmp = (*res_iter).second;
        if (res != NULL)
        {
            delete tmp;
        }
        this->_fd_res_map[fd] = res;
    }
    */
}

Response *Socket::get_response(int fd)
{
    return (this->_fd_map[fd]->get_res());
    /*
    std::map<int, Response*>::iterator res_iter = this->_fd_res_map.find(fd);
    if (res_iter == this->_fd_res_map.end())
    {
        cout << "NULL" << endl;
        return (NULL);
    }
    return (this->_fd_res_map[fd]);
    */
}

/*
void Socket::increment_timeout(std::map<int, SocketData*> &map, int time)
{

}
*/

/*
template < template < typename , typename> class Container>
void Socket::increment_timeout(Container &map, int time)
{
    int fd;
    SocketData *socket_data = map;
    std::map<int, Response*>::iterator iter = map.begin();
    std::map<int, Response*>::iterator end = map.end();

    bool exceed;
    for(; iter != end;iter++){
        cout << "timeout No.4" << endl;
        fd = iter->first;
        socket_data = iter->second;
        exceed = socket_data->increment_timeout(time);
        if (exceed){
            cout << "timeout No.5" << endl;
            map.erase(iter->first);
            delete socket_data;
            close(fd);
        }
    }
}
*/


/*
static void Socket::increment_timeout(std::map<int, Request*> &map, int time)
{
    int fd;
    SocketData *socket_data;
    std::map<int, Request*>::iterator iter = map.begin(); std::map<int, Request*>::iterator end = map.end();

    bool exceed;
    for(; iter != end;iter++){
        cout << "timeout No.2" << endl;
        fd = iter->first;
        socket_data = iter->second;
        exceed = socket_data->increment_timeout(time);
        if (exceed){
            cout << "timeout No.3" << endl;
            map.erase(iter->first);
            delete socket_data;
            close(fd);
        }
    }
}
*/

std::vector<int> Socket::timeout(int time)
{
    std::map<int, FDManager*>::iterator iter = _fd_map.begin(); 
    std::map<int, FDManager*>::iterator end = _fd_map.end();
    cout << "_fd_map size=" << _fd_map.size() << endl;
    std::vector<int> delete_fd;

    FDManager *fd_m;
    int fd;
    bool timeout;
    for(; iter != end; iter++)
    {
        cout << "timeout No.1:" << time << endl;
        fd = iter->first;
        fd_m = iter->second;
        if (fd_m->get_req()){
            cout << "timeout No.2 Request:" << time << endl;
            timeout = increment_timeout(*fd_m->get_req(), time);
        }else if (fd_m->get_res()){
            cout << "timeout No.3 Response:" << time << endl;
            timeout = increment_timeout(*fd_m->get_res(), time);
        }else{
            cout << "timeout No.4 other:" << time << endl;
            printf("fd_m=%p\n", fd_m);
            timeout = increment_timeout(*fd_m, time);
        }
        if (timeout){
            cout << "delete_fd No.1" << endl;
            delete_fd.push_back(fd);
            cout << "delete_fd No.2" << endl;
        }
        cout << "timeout No.5 other:" << time << endl;
    }
    cout << "timeout No.1:" << time << endl;
    return (delete_fd);
    //increment_timeout(_fd_req_map, time);
    //increment_timeout(_fd_res_map, time);
}

void Socket::erase_request(int fd)
{
    this->_fd_map[fd]->delete_req();
}

void Socket::erase_response(int fd)
{
    //Response *tmp = _fd_map[fd].get_res();
    //delete tmp;
    this->_fd_map[fd]->delete_res();
}

void Socket::delete_fd_map(int fd)
{
    cout << "delete_fd_map No.1 fd=" << fd << ",size=" << endl;
    printf("map=%zu\n", (this->_fd_map.size()));
    std::map <int, FDManager*>::iterator ite = this->_fd_map.begin();
    std::map <int, FDManager*>::iterator end = this->_fd_map.end();
    for (; ite != end; ite++){
        cout << "fd=" << ite->first << endl;

    }
        //cout << "delete_fd_map No.1.5 fd=" << this->_fd_map[i].first << endl;

    //}
    FDManager* fd_m = this->_fd_map[fd];
    cout << "delete_fd_map No.2" << endl;
    fd_m->close_fd();
    cout << "delete_fd_map No.3" << endl;
    delete fd_m;
    cout << "delete_fd_map No.4" << endl;
    this->_fd_map.erase(fd);
    cout << "delete_fd_map No.5" << endl;
}
