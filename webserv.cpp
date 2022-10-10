#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>


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
    tv.tv_sec = 2;
    tv.tv_usec = 0;
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
                        // char *str = new char[1025];
                        // if (std::string(Buffer).find("/pong",0) != -1)
                        // {
                        //     std::cout << Buffer << std::endl;
                        //     int html = open("www/index.html",O_RDONLY);
                        //     int a = read(html,str,1024);
                        //     str[a] = '\0';
                        //     if (FD_ISSET(i, &cpyFdsToWrite))
                        //     {
                        //         send(i,str,strlen(str),0);
                        //         while(a)
                        //         {
                        //             a = read(html,str,1024);
                        //             str[a] = '\0';
                        //             send(i,str,strlen(str),0);
                        //         }
                        //         shutdown(i, SHUT_WR);
                        //     }

                        // }
                        // else if (std::string(Buffer).find("/immg.jpeg",0) != -1)
                        // {
                        //     FD_SET(i, & FdsToWrite);
                        //     char resp[] = "HTTP/1.1 200 OK\r\n"
                        //         "Content-Length: 82830\r\n"
                        //         "Content-Type: image/jpeg\r\n"
                        //         "Cache-Control: no-cache\r\n"
                        //         // "Connection: close\n"
                        //         "\r\n";
                        //         // "<html><body><h1> Hello,World !</h1></body></html>";
                        //     if (FD_ISSET(i, &cpyFdsToWrite))
                        //     {
                        //         send(i,resp,std::string(resp).size(),0);
                        //         std::ifstream img;
                        //         img.open("./www/immg.jpeg");
                        //         char * imgB = new char [82830];
                        //         img.read(imgB, 82830);
                        //         send(i,imgB,82830,0);
                        //     }
                        //     // close(i);
                        // }
                        else if (std::string(Buffer).find("/b.mp4",0) != -1)
                        {
                                int x = 1;
                                char resp[] = "HTTP/1.1 200 OK\r\n"
                                "Content-Length: 27072286\r\n"
                                "Content-Type: video/mp4\r\n"
                                "Cache-Control: no-cache\r\n"
                                "Connection: keep-alive\n"
                                "\r\n";
                                // "<html><body><h1> Hello,World !</h1></body></html>";
                            if (FD_ISSET(i, &cpyFdsToWrite))
                            {
                                if (x == 1)
                                {
                                    size_t byte_sent = send(i,resp,std::string(resp).size(),0);
                                    x = 0;
                                }
                                std::ifstream video;
                                video.open("./www/butterfly.mp4");
                                if (video.is_open())
                                {
                                    char * videoB = new char [27072286];
                                    video.read(videoB, 27072286);
                                    send(i,videoB,27072286,0);
                                }
                            }
                            FD_SET(i, & FdsToWrite);
                        }
                        // else
                        // {
                        //     int html = open("www/w.html",O_RDONLY);
                        //     int a = read(html,str,1024);
                        //     str[a] = '\0';
                        //     if (FD_ISSET(i, &cpyFdsToWrite))
                        //     {
                        //         send(i,str,strlen(str),0);
                        //         while(a)
                        //         {
                        //             a = read(html,str,1024);
                        //             str[a] = '\0';
                        //             send(i,str,strlen(str),0);
                        //         }
                        //     }
                        // }
                        // shutdown(i, SHUT_WR);

                                        //  std::cout << Buffer << std::endl;
                        
                        //sending response
                    }
                }
                else if (FD_ISSET(i,&))
                {

                }
            }
            
        }
    }
    return 0;
}