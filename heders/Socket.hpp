#ifndef Socket_class
#define Socket_class

#include "WebServer.hpp"

class Socket{
    private :
        int _fd;
        sockaddr_in _socket;
};

#endif