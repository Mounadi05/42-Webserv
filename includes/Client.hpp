#ifndef Clinet_class
#define Clinet_class

#include "Request.hpp"
#include "Response.hpp"
#include "Socket.hpp"

class Client {
    private :
    public:
        Request _request;
        Response _response;
        Socket _socket;
        Client();
        Client(const Client &cl);
        Client &operator=(const Client &cl);
        ~Client();
};

#endif