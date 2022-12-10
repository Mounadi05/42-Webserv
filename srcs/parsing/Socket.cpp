#include "../../includes/Webserv.hpp"

Socket::Socket(){
    _sockaddr = new struct sockaddr_in;
    std::cout << sizeof(_sockaddr) << std::endl;
}

Socket::~Socket()
{
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