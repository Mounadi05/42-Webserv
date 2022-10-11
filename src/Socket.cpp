#include "../includes/WebServer.hpp"

Socket::Socket() {
    _socket_in = sockaddr_in();
    _fd = 0;
}

Socket::Socket(const Socket &so) {
    *this = so;
}

Socket &Socket::operator=(const Socket &so) {
    if (this != &so) {
        _socket_in = so._socket_in;
        _fd = so._fd;
    }
    return *this;
}

Socket::~Socket() {
}