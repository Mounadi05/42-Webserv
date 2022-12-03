#include "../../includes/Webserv.hpp"

Client::Client()
{
    test = 0;
    a = 0;
    len = 0;
}

Client::~Client()
{
    // close(test);
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