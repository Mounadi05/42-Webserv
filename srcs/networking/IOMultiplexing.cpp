#include "../../includes/Webserv.hpp"
#include <utility>

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
    // signal(SIGPIPE,SIG_IGN);
    fd_set readcpy, writecpy;
    int fd_client;
    struct timeval vl;
    vl.tv_sec = 3;
    vl.tv_usec = 0;
    // std::map<Client,Request> ClientRequest;
    std::vector<std::pair<Client, Request> > ClientRequest;
    std::vector<std::pair<Client, Response> > ClientResponse;
    // std::vector<std::pair<Client, Response> > ClientResponse;
    // std::vector<std::pair<Client, Response> > ClientRequest;

    while (1)
    {
        readcpy = io.fdread;
        writecpy = io.fdwrite;
        int r = select(io._fdmax + 1, &readcpy, &writecpy, NULL, &vl);
        if (r == -1)
        {
            std::cout << "Error in select" << std::endl;
            break;
        }
        else if (r == 0)
            continue;
        else
        {
            // int maxfdserver = 0;
            for (size_t j = 0; j < servers.size(); j++)
            {
                int fdserver = servers[j].getSocket().getSocketFd();
                if (FD_ISSET(fdserver, &readcpy))
                {
                    Client newC;
                    fd_client = accept(fdserver, NULL, NULL); // if fd != -1
                    newC.setSocketFd(fd_client);
                    newC.setServer(servers[j]);
                    ClientRequest.push_back(std::pair<Client, Request>(newC, Request()));
                    std::cout << "new clinet " << fd_client << " on server " << fdserver << std::endl;
                    io.setFdRead(fd_client);
                    if (fd_client > io._fdmax)
                        io._fdmax = fd_client;
                }
                // maxfdserver = fdserver;
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
                        // FD_CLR(i,&readcpy);
                        close(ClientRequest[i].first.getSocketFd());
                        // if (std::find(ClientRequest.begin(),ClientRequest.end(),ClientRequest[i]) != ClientRequest.end())
                        //     ClientRequest.erase(std::find(ClientRequest.begin(),ClientRequest.end(),ClientRequest[i]));

                    }
                    
                    else if (r == 0)
                    {
                        FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdread);
                        std::cout << "Client disconnected" << std::endl;
                        // FD_CLR(i,&readcpy);
                        close(ClientRequest[i].first.getSocketFd());
                    }
                    else
                    {
                        request[r] = '\0';
                        
                        // int finished = readRequest
                        ClientRequest[i].second.setRequestBuffer(request);
                        
                        // if (ClientRequest[i].second.getRequestBuffer().find("\n") != std::string::npos ||ClientRequest[i].second.getRequestBuffer().find("\r\n") != std::string::npos )
                        // {
                            std::cout << "first line finished" << std::endl;
                            std::cout << "Request : " << std::endl
                                    << ClientRequest[i].second.getRequestBuffer() << std::endl;
                            FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdread);
                            FD_SET(ClientRequest[i].first.getSocketFd(), &io.fdwrite);
                        // }
                        //if (finished == 1)
                            // Response newORes(Request, servers);
                    }
                }
                if (FD_ISSET(ClientRequest[i].first.getSocketFd(), &writecpy)) // response
                {
                    std::cout << "i = " << ClientRequest[i].first.getSocketFd() << std::endl;
                    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello World!";
                    int r = send(ClientRequest[i].first.getSocketFd(), response.c_str(), response.size(), 0);
                    if (r == -1)
                    {
                        std::cout << "Error in send" << std::endl;
                        FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdwrite);
                        close(ClientRequest[i].first.getSocketFd());
                    }
                    else
                    {
                        std::cout << "Response : " << response << std::endl;
                        FD_CLR(ClientRequest[i].first.getSocketFd(), &io.fdwrite);
                        FD_SET(ClientRequest[i].first.getSocketFd(), &io.fdread);
                    }
                }
            }
        }
    }
}

Response craft_response(std::pair<Client,Request> ClientRequest)
{
    (void) ClientRequest;
    return Response();
}