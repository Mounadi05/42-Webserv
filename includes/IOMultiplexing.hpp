#ifndef IOMultiplexing_class
#define IOMultiplexing_class

#include "Config.hpp"
#include "Server.hpp"

class IOMultiplexing
{
private:

public:
    fd_set fdwrite;
    fd_set fdread;
    int _fdmax;
    std::vector<std::pair<int ,int> > AlreadyBind;
    IOMultiplexing();
    ~IOMultiplexing();
    void SetupServers(Config &conf);
    // fd_set getFdRead();
    void setFdRead(int fd);
    int getFdMax() const;
    void setFdMax(int fd);
};

void EventLoop(std::vector<Server> &servers, IOMultiplexing &io);
int CreateSocket(Socket &sock, int port, IOMultiplexing &io);
// int Bind();
// int Listen();
int CreatServer(Server &server, IOMultiplexing &io);

#endif