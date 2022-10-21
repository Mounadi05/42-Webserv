#include "../../includes/Webserv.hpp"

Response::Response()
{

}

Response::Response(Request  request,Server  server, int ClientFD)
{
    _request = request;
    _server = server;
    _ClientFD = ClientFD;
}

Response::~Response()
{

}

int Response::getClientFD() const{
    return _ClientFD;
}

Request & Response::getRequest()
{
    return _request;
}

Server & Response::getServer()
{
    return _server;
}