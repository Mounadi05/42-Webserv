#ifndef Server_class
#define Server_class

#include "Socket.hpp"
#include "Client.hpp"

class Server{
    public :
    Socket _socket;
    std::vector<Client> _clients;

    Server();
    Server(const Server &se);
    Server &operator=(const Server &se);
    ~Server();
    int ServerSetup(int port);
    int ServerBind();
    int ServerListen();
    int ServerAccept();
};

#endif