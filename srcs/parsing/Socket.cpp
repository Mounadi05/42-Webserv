#include "../../includes/Webserv.hpp"

Socket::Socket(){
    _sockaddr = new struct sockaddr_in;
}

Socket::~Socket()
{
    // delete _sockaddr;
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