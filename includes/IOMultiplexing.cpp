#include "../../includes/Webserv.hpp"
#include <utility>

#define DONE 1
#define NOT_FINISHED 0

int craftResponse(Request request, Server server, int clientFd)
{
    if (request.Getstatus_code() == 400)
    {

    }
    else
    {

    }

    return DONE;
}

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
    if (Svec.size())
    {
        for (size_t i = 0; i < Svec.size(); i++)
            CreatServer(Svec[i], *this);
        EventLoop(Svec, *this);
    }
    else
        printError("Bad config file " + conf.getFilePath());
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
        fcntl(fd, F_SETFL, O_NONBLOCK);
        if (bind(fd, (struct sockaddr *)sockaddr, sizeof(*sockaddr)) == -1)
        {
            printError("Bind failed");
            return (-1);
        }
        // std::cout << "Binded " << port << std::endl;
        if (listen(fd, SOMAXCONN) == -1)
        {
            printError("Listen failed");
            return (-1);
        }
        std::cout << "Listening " << fd << " Port " << port << std::endl;
        sock.setSocketFd(fd);
        io.setFdRead(fd);
        io.setFdMax(fd);
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &val, sizeof(val));
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
int send_data(int fd, int test)
{
    char *str = new char[1025];
    int a = read(test, str, 1024);
    if (a <= 0)
    {
        std::cout << "error or finished" << std::endl;
        return a;
    }
    int t = send(fd, str, a, 0);
    std::cout << "send " << t << std::endl;
    return t;
}

void EventLoop(std::vector<Server> &servers, IOMultiplexing &io)
{
    fd_set readcpy, writecpy;
    int fd_client;
    struct timeval vl;
    vl.tv_sec = 3;
    vl.tv_usec = 0;
    std::vector<std::pair<Client, Request> > ClientRequest;
    std::vector<std::pair<Client, Response> > ClientResponse;

    while (1)
    {
        readcpy = io.fdread;
        writecpy = io.fdwrite;
        int r = select(io._fdmax + 1, &readcpy, &writecpy, NULL, &vl);
        if (r == -1)
        {
            std::cout << "Error in select" << std::endl;
        }
        else if (r == 0)
        {
            std::cout << "timeout" << std::endl;
            continue;
        }
        else
        {
            for (size_t j = 0; j < servers.size(); j++)
            {
                int fdserver = servers[j].getSocket().getSocketFd();
                if (FD_ISSET(fdserver, &readcpy))
                {
                    Client newC;
                    if ((fd_client = accept(fdserver, NULL, NULL)) != -1)
                    {
                        fcntl(fd_client, F_SETFL, O_NONBLOCK);
                        newC.setSocketFd(fd_client);
                        newC.setServer(servers[j]);
                        ClientRequest.push_back(std::pair<Client, Request>(newC, Request()));
                        std::cout << "new clinet " << fd_client << " on server " << fdserver << std::endl;
                        io.setFdRead(fd_client);
                        if (fd_client > io._fdmax)
                            io._fdmax = fd_client;
                    }
                }
            }
            for (u_int i = 0; i < ClientRequest.size(); i++)
            {
                if (FD_ISSET(ClientRequest[i].first.getSocketFd(), &readcpy)) // request
                {
                    char request[1024];
                    std::cout << "i = " << ClientRequest[i].first.getSocketFd() << std::endl;
                    int r = recv(ClientRequest[i].first.getSocketFd(), request, 1023, 0);
                    if (r == -1)
                    {
                        std::cout << "Error in recv" << std::endl;
                        FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdread);
                        close(ClientRequest[i].first.getSocketFd());
                        ClientRequest.erase(ClientRequest.begin() + i);
                    }
                    else if (r == 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdread);
                        close(ClientRequest[i].first.getSocketFd());
                        ClientRequest.erase(ClientRequest.begin() + i);
                    }
                    else
                    {
                        request[r] = '\0';
                        ClientRequest[i].second.handle_request(request);
                        std::cout << "Method " <<ClientRequest[i].second.Getrequest().at("Method") << std::endl;
                        std::cout << "version " <<ClientRequest[i].second.Getrequest().at("Version") << std::endl;
                        FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdread);
                        FD_SET(ClientRequest[i].first.getSocketFd(), &io.fdwrite);
                        ClientRequest[i].first.test = 0;
                    }
                }
                else if (FD_ISSET(ClientRequest[i].first.getSocketFd(), &writecpy)) // response
                {
                    int responseSent = 0;

                    responseSent = craftResponse(ClientRequest[i].second, ClientRequest[i].first.getServer(), ClientRequest[i].first.getSocketFd());
                    // if (responseSent == -1)
                    // {
                    //    
                    // }
                    // else if (responseSent == DONE)
                    // {
                    //     // check if connection is Keep-Alive or not
                    //     if (ClientRequest[i].second.Getrequest().at("Connection").compare("Keep-Alive") == 0)
                    //     {
                    //         ClientRequest.pushback(std::make_pair<Client, Request> (ClientRequest.first, Request()));
                    //         FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdwrite);
                    //         FD_SET(ClientRequest[i].first.getSocketFd(), &io.fdread);
                    //     } 
                    //     else
                    //     {
                    //         FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdwrite);
                    //         close(ClientRequest[i].first.getSocketFd());
                    //         ClientRequest[i].erase(); // set index
                    //     }
                    // }
                }
            }
        }
    }
}

