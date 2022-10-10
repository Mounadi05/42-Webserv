#ifndef Clinet_class
#define Clinet_class

#include "WebServer.hpp"

class Client {
    private :
        Request _request;
        Response _response;
        Socket _socket;
};

#endif