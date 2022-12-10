#include "../../includes/Webserv.hpp"

Socket::Socket(){
    _sockaddr = new struct sockaddr_in;
}
Socket::Socket(const Socket &rhs)
{
    if (this != &rhs)
    {
        _sockaddr = new struct sockaddr_in;
        _socket_fd = rhs._socket_fd;
    }
}

Socket & Socket::operator=(const Socket &rhs)
{
    delete _sockaddr;
    _sockaddr = new struct sockaddr_in;
    _socket_fd = rhs._socket_fd;
    return *this;
}

Socket::~Socket()
{
    delete _sockaddr;
}

struct sockaddr_in *Socket::getSockAddr() const{
    return _sockaddr;
}

int Socket::getSocketFd() const{
    return _socket_fd;
}

void Socket::setSocketFd(int fd)
{
    _socket_fd = fd;
}