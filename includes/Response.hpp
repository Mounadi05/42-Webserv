#ifndef Response_class
#define Response_class

#include "Request.hpp"
#include "Server.hpp"

class Response
{
private:
    int _ClientFD;
    Request _request;
    Server _server;
public:
    Response();
    Response(Request & request,Server & server, int ClientFD);
    ~Response();
    int getClientFD() const;
    Request & getRequest();
    Server & getServer();
};

#endif