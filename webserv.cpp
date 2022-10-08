#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    setsockopt(fdserver,SOL_SOCKET,SO_REUSEADDR,(void *)&t,sizeof(t));
    if (bind(fdserver,(sockaddr *)&server,sizeof(server)) == -1)
        std::cout << "bind port "<< ntohs(server.sin_port) << " failed!" << std::endl;
    listen(fdserver,SOMAXCONN);
    fd_set FdsToRead;
    fd_set cpyFdsToRead;
    FD_ZERO(&FdsToRead);
    FD_SET(fdserver,&FdsToRead);
    int max;
    max = fdserver;
    socklen_t cl;
    timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0; 
    while(1)
    {
        cpyFdsToRead = FdsToRead;
        sr = select(max + 1,&cpyFdsToRead,NULL,NULL,&tv);
        if (sr == -1)
            std::cout << "select failed" << std::endl;
        else if (!sr)
        {
            std::cout << "Timeout" << std::endl;
            continue;
        }
        else
        {

            // 3 4
            //max 4
            for (size_t i = 3; i < max + 1; i++) // loop for all set to check if there is any new connection or request
            {
                if (FD_ISSET(i,&cpyFdsToRead))
                {
                    if (i == fdserver) // new connection
                    {
                        cl = sizeof(client);
                        fdclient = accept(fdserver, (sockaddr *) &client,&cl);
                        std::cout << "new connection" << std::endl;
                        FD_SET(fdclient, & FdsToRead);
                        if (fdclient > max)
                            max = fdclient;
                    }
                    else // new request
                    {
                        std::cout << "HERE" << std::endl;
                        //recving request
                        char Buffer[1024];
                        int x = recv(i,Buffer,1024,0);
                        if (x <= 0)
                        {
                            FD_CLR(i, &FdsToRead);
                            // shutdown(i, SHUT_WR);
                            close(i);
                        }
                        Buffer[x] = 0;
                        char *str = new char[1025];
                        if (std::string(Buffer).find("/pong",0) != -1)
                        {
                            std::cout << Buffer << std::endl;
                            int html = open("www/index.html",O_RDONLY);
                            int a = read(html,str,1024);
                            str[a] = '\0';
                            send(i,str,strlen(str),0);
                            while(a)
                            {
                                a = read(html,str,1024);
                                str[a] = '\0';
                                send(i,str,strlen(str),0);
                            }
                        }
                        else if (std::string(Buffer).find("/immg.jpeg",0) != -1)
                        {
                            char *resp =new char[100];
                            resp = (char *)"HTTP/1.1 200 OK\r\nContent-Length: 83129\r\nContent-type: img/jpeg\r\n\r\n";
                            send(i,resp,strlen(resp),0); 
                            int html = open("www/immg.jpeg",O_RDONLY);
                            int a = read(html,str,1024);
                            send(i,str,a,0);
                            while(a)
                            {
                                a = read(html,str,1024);
                                send(i,str,a,0);
                            }
                        }
                        else
                        {
                            int html = open("www/w.html",O_RDONLY);
                            int a = read(html,str,1024);
                            str[a] = '\0';
                            send(i,str,strlen(str),0);
                            while(a)
                            {
                                a = read(html,str,1024);
                                str[a] = '\0';
                                send(i,str,strlen(str),0);
                            }
                        }
                        shutdown(i, SHUT_WR);
                                        //  std::cout << Buffer << std::endl;
                        
                        //sending response
                    }
                }
            }
            
        }
    }
    return 0;
}