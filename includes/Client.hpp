#ifndef Client_class
# define Client_class


#include <vector>
#include <iostream>
#include "Socket.hpp"

class Client
{
    private:
        Socket _c_socket;
        Server _server;
    public:
        Client();
        ~Client();
        int getSocketFd();
        void setSocketFd(int fd);
        void setServer(Server & clinet_server);
        Server & getServer();
        int test;
        int a;
        int len;
};

#endif