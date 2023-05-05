#ifndef FD_STATE_HPP
#define FD_STATE_HPP

#include "socket_data.hpp"
#include "request.hpp"
#include "response.hpp"

class FDManager : public SocketData
{
    public:
        FDManager(int fd);
        ~FDManager();
        void insert(Request* req);
        void insert(Response* res);
        Request* get_req(void);
        Response* get_res(void);
        void delete_req();
        void delete_res();
        void close_fd();
    private:
        int _fd;
        //SocketData* socket_data;
        Request* req;
        Response* res;
};
#endif
