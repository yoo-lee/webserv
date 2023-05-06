#include "fd_manager.hpp"
#include <unistd.h>

FDManager::FDManager(int fd) : _fd(fd), _req(NULL), _res(NULL)
{
}

FDManager::~FDManager()
{
    delete _req;
    delete _res;
}

void FDManager::insert(Request* req)
{
    if (this->_req){
        delete this->_req;
    }
    this->_req = req;
}

void FDManager::insert(Response* res)
{
    if (this->_res){
        delete this->_res;
    }
    this->_res = res;
}

Request* FDManager::get_req()
{
    return (this->_req);
}

Response* FDManager::get_res()
{
    return (this->_res);
}

void FDManager::delete_req()
{
    delete (this->_req);
    this->_req = NULL;
}

void FDManager::delete_res()
{
    delete (this->_res);
    this->_res = NULL;
}

void FDManager::close_fd()
{
    close(_fd);
    //this->delete_res();
    //this->delete_req();
}
