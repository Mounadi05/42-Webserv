#ifndef Socket_class
#define Socket_class

#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>

class Socket{
    public:
    sockaddr_in _socket_in;
    int _fd;
    Socket();
    Socket(const Socket &so);
    Socket &operator=(const Socket &so);
    ~Socket();
};

#endif