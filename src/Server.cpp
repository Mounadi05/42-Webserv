#include "../includes/WebServer.hpp"

Server::Server()
{
    _socket = Socket();

}

Server::Server(const Server &src)
{
    *this = src;
}

Server::~Server()
{
    _clients.clear();
}

Server &Server::operator=(const Server &rhs)
{
    if (this != &rhs)
    {
        _socket = rhs._socket;
        _clients = rhs._clients;
    }
    return *this;
}

int Server::ServerSetup(int port)
{
    this->_socket._socket_in.sin_family = AF_INET;
    this->_socket._socket_in.sin_port = htons(port);
    this->_socket._socket_in.sin_addr.s_addr = INADDR_ANY;
    int result = socket(AF_INET, SOCK_STREAM, 0);
    if (result == -1)
        return -1;
    this->_socket._fd = result;
    return 0;
}

int Server::ServerBind()
{
    int result = bind(this->_socket._fd, (struct sockaddr *)&this->_socket._socket_in, sizeof(this->_socket._socket_in));
    if (result == -1)
        return -1;
    return 0;
}

int Server::ServerListen()
{
    int result = listen(this->_socket._fd, SOMAXCONN);
    if (result == -1)
        return -1;
    return 0;
}

int Server::ServerAccept()
{
    int result = accept(this->_socket._fd, (struct sockaddr *)&this->_socket._socket_in, (socklen_t *)&this->_socket._socket_in);
    if (result == -1)
        return -1;
    Client _clinet;
    _clinet._socket._fd = result;
    _clients.push_back(_clinet);
    return 0;
}