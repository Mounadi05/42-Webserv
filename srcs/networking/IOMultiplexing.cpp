#include "../../includes/Webserv.hpp"

IOMultiplexing::IOMultiplexing()
{
    _fdmax = 0;
}

IOMultiplexing::~IOMultiplexing()
{
}

void IOMultiplexing::SetupServers(Config &conf)
{
    conf.parse();
    std::vector<Server> &Svec = conf.getServers();
    for (size_t i = 0; i < Svec.size(); i++)
        CreatServer(Svec[i], *this);
    EventLoop(Svec, *this);
}

int CreatServer(Server &server, IOMultiplexing &io)
{
    CreateSocket(server.getSocket(), server.getPort(), io);
    return 0;
}

int CreateSocket(Socket &sock, int port, IOMultiplexing &io)
{
    int fd;
    struct sockaddr_in *sockaddr = sock.getSockAddr();
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    sockaddr->sin_addr.s_addr = INADDR_ANY;
    if (std::find(io.AlreadyBind.begin(), io.AlreadyBind.end(), port) == io.AlreadyBind.end())
    {
        io.AlreadyBind.push_back(port);
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            printError("Socket creation failed");
            return (-1);
        }
        // std::cout << "Socket created " << fd << std::endl;
        int val = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
        {
            printError("setsockopt failed");
            return (-1);
        }
        if (bind(fd, (struct sockaddr *)sockaddr, sizeof(*sockaddr)) == -1)
        {
            printError("Bind failed");
            return (-1);
        }
        // std::cout << "Binded " << port << std::endl;
        if (listen(fd, 10) == -1)
        {
            printError("Listen failed");
            return (-1);
        }
        std::cout << "Listening " << fd << " Port " << port << std::endl;
        sock.setSocketFd(fd);
        io.setFdRead(fd);
        io.setFdMax(fd);
        return (fd);
    }
    return (0);
}

void IOMultiplexing::setFdRead(int fd)
{
    FD_SET(fd, &fdread);
}

int IOMultiplexing::getFdMax() const
{
    return _fdmax;
}
void IOMultiplexing::setFdMax(int fd)
{
    _fdmax = fd;
}

void EventLoop(std::vector<Server> &servers, IOMultiplexing &io)
{
    fd_set readcpy, writecpy;
    int fd_client;
    struct timeval vl;
    vl.tv_sec = 3;
    vl.tv_usec = 0;

    while (1)
    {
        readcpy = io.fdread;
        writecpy = io.fdwrite;
        int r = select(io._fdmax + 1, &readcpy, NULL, NULL, &vl);
        if (r == -1)
        {
            std::cout << "Error in select" << std::endl;
            break;
        }
        if (r == 0)
            continue;
        else
        {
            int maxfdserver = 0;
            for (size_t j = 0; j < servers.size(); j++)
            {
                int fdserver = servers[j].getSocket().getSocketFd();
                if (FD_ISSET(fdserver, &readcpy))
                {
                    fd_client = accept(fdserver, NULL, NULL);
                    std::cout << "new clinet " << fd_client << " on server " << fdserver << std::endl;
                    io.setFdRead(fd_client);
                    if (fd_client > io._fdmax)
                        io._fdmax = fd_client;
                }
                maxfdserver = fdserver;
            }
            for (int i = maxfdserver + 1; i < io._fdmax + 1; i++)
            {
                if (FD_ISSET(i, &readcpy)) //request
                {
                    char request[1024];
                    std::cout << "i = " << i << std::endl;
                    int r = recv(i, request, 1023, 0);
                    if (r == -1)
                    {
                        std::cout << "Error in recv" << std::endl;
                        FD_CLR(i, &io.fdread);
                        close(i);
                    }
                    if (r == 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        FD_CLR(i, &io.fdread);
                        close(i);
                    }
                    else
                    {
                        request[r] = '\0';
                        std::cout << "Request : " << request << std::endl;
                        FD_CLR(i, &io.fdread);
                        FD_SET(i, &io.fdwrite);
                    }
                }
                else if (FD_ISSET(i, &writecpy)) //response
                {

                }
            }
        }
    }
}