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

int Response::handler(){
    // std::cout << this->_request.body << std::endl;
    // std::cout << this->_request.Getrequest().at("Method") << std::endl;
    // std::cout << this->_request.Getrequest().at("Path") << std::endl;
    // std::cout << this->_server.getRoot() << std::endl;
    // std::cout << "i = " << this->_ClientFD << std::endl;
    // std::string response;
    // response = (char *)"HTTP/1.1 200 OK\r\nContent-Length: 21\r\nContent-type: text/html\r\nConnection: keep-alive\r\n\r\n<H1>Hello World!</H1>";
    // send(this->_ClientFD, response.c_str(), response.size(), 0);
    // FD_CLR(ReadyResponse[i].getClientFD(), &io.fdwrite);
    // FD_SET(ReadyResponse[i].getClientFD(), &io.fdread);
    return (1);
}