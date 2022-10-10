#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>


int main ()
{
    int fdserver,fdclient;
    sockaddr_in client,server;
    int sr;
    fdserver = socket(AF_INET,SOCK_STREAM,0);
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(1212);
    int t = 1;
    // SO_NOSIGPIPE
    setsockopt(fdserver,SOL_SOCKET,SO_REUSEADDR,(void *)&t,sizeof(t));
    setsockopt(fdserver,SOL_SOCKET,SO_NOSIGPIPE,(void *)&t,sizeof(t));
    if (bind(fdserver,(sockaddr *)&server,sizeof(server)) == -1)
        std::cout << "bind port "<< ntohs(server.sin_port) << " failed!" << std::endl;
    listen(fdserver,SOMAXCONN);
    fd_set FdsToRead;
    fd_set FdsToWrite;
    fd_set cpyFdsToRead;
    fd_set cpyFdsToWrite;
    FD_ZERO(&FdsToRead);
    FD_SET(fdserver,&FdsToRead);
    int max;
    max = fdserver;
    socklen_t cl;
    timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    size_t byte_sent = 0;
    int head_sent = 0;
    int content_lenght = 0;
    char * videoB = new char [27072286];
    std::ifstream video;
    video.open("./www/butterfly.mp4");

    int x = 0;

    // signal(SIGPIPE,SIG_IGN);
    while(1)
    {
        cpyFdsToRead = FdsToRead;
        cpyFdsToWrite = FdsToWrite;
        sr = select(max + 1,&cpyFdsToRead,&cpyFdsToWrite,NULL,&tv);
        if (sr == -1)
            std::cout << "select failed" << std::endl;
        else if (!sr)
        {
            continue;
        }
        else
        {
            // 3 4
            //max 4
            for (size_t i = 3; i < max + 1; i++) // loop for all set to check if there is any new connection or request
            {
                char request[2048];
                if (FD_ISSET(i,&cpyFdsToRead))
                {
                    if (i == fdserver) // new connection
                    {
                        cl = sizeof(client);
                        fdclient = accept(fdserver, (sockaddr *) &client,&cl);
                        std::cout << "new connection from " << inet_ntoa(client.sin_addr) << " on socket " << fdclient << std::endl;
                        FD_SET(fdclient, & FdsToRead);
                        if (fdclient > max)
                            max = fdclient;
                    }
                    else
                    {
                        int Rend;
                        if ((Rend = recv(i,request,2047,0)) == -1)
                        {
                            std::cout << "reciving request failed" << std::endl;
                            FD_CLR(i, &FdsToRead);
                            FD_SET(i, &FdsToWrite);
                        }
                        else if (Rend <= 0)
                        {
                            FD_CLR(i, &FdsToRead);
                            close(i);
                        }
                        else
                        {
                            request[Rend] = 0;
                            std::cout <<  request << std::endl;
                            std::cout << Rend << std::endl;
                            FD_CLR(i, &FdsToRead);
                            FD_SET(i, &FdsToWrite);
                        }
                        
                    }
                }
                else if (FD_ISSET(i,&cpyFdsToWrite))
                {
                    if (std::string(request).find("/b.mp4", 0) != -1)
                    {

                    }
                    char resp[] = "HTTP/1.1 200 OK\r\n"
                    "Content-Length: 27072286\r\n"
                    "Content-Type: video/mp4\r\n"
                    // "Cache-Control: no-cache\r\n"
                    "Connection: keep-alive\n"
                    "\r\n";
                    if (!head_sent)
                    {
                        byte_sent = send(i,resp,std::string(resp).size(),0);
                        std::cout << std::string(resp).size() << std::endl;
                        std::cout <<"byte sent " << byte_sent << std::endl;
                        head_sent = 1;

                    }
                    if (head_sent == 1)
                    {
                        if (video.is_open())
                            video.read(videoB, 1024);
                        byte_sent =  send(i,videoB,1024,0);
                        if (byte_sent != -1)
                            content_lenght += byte_sent;
                }
            }
            
        }
    }
    return 0;
}