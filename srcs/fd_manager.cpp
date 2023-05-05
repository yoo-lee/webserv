#include "fd_manager.hpp"
#include <unistd.h>

FDManager::FDManager(int fd) : _fd(fd), req(NULL), res(NULL)
{
}

FDManager::~FDManager()
{
    delete req;
    delete res;
}

void FDManager::insert(Request* req)
{
    if (this->req){
        delete this->req;
    }
    this->req = req;
}

void FDManager::insert(Response* res)
{
    if (this->res){
        delete this->res;
    }
    this->res = res;
}

Request* FDManager::get_req()
{
    return (this->req);
}

Response* FDManager::get_res()
{
    return (this->res);
}

void FDManager::delete_req()
{
    delete (this->req);
    this->req = NULL;
}

void FDManager::delete_res()
{
    delete (this->res);
    this->res = NULL;
}

void FDManager::close_fd()
{
    close(_fd);
    //this->delete_res();
    //this->delete_req();
}
