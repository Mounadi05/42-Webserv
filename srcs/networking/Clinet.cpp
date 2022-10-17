#include "../../includes/Webserv.hpp"

Client::Client()
{

}

Client::~Client()
{

}

int Client::getSocketFd()
{
   return (this->_c_socket.getSocketFd());
}

void Client::setSocketFd(int fd)
{
    this->_c_socket.setSocketFd(fd);
}

void Client::setServer(Server & clinet_server)
{
    _server = clinet_server;
}

Server & Client::getServer()
{
    return _server;
}