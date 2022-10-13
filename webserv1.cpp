#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main ()
{
    int img = open("www/immg.jpeg",O_RDONLY);
    char *str = new char[1025];
    int mp4 = open("www/amy.mp4",O_RDONLY);
    char *request = new char[1025];
    int h = open("www/w.html",O_RDONLY);
    int html = open("www/index.html",O_RDONLY);
    int up = open("www/up.html",O_RDONLY);
    int js = open("www/index.js",O_RDONLY);
    int fdserver,fdclient;
    sockaddr_in client,server;
    int sr;
    fdserver = socket(AF_INET,SOCK_STREAM,0);
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(1010);
    int t = 1;
    setsockopt(fdserver,SOL_SOCKET,SO_REUSEADDR,(char *)&t,sizeof(t));
    setsockopt(fdserver,SOL_SOCKET,SO_NOSIGPIPE,(char *)&t,sizeof(t));
    if (bind(fdserver,(sockaddr *)&server,sizeof(server)) == -1)
        std::cout << "bind port "<< ntohs(server.sin_port) << " failed!" << std::endl;
    listen(fdserver,SOMAXCONN);
    fd_set FdsToRead;
    fd_set cpyFdsToWrite;
    fd_set cpyFdsToRead;
    fd_set FdsToWrite;
    FD_ZERO(&FdsToRead);
    FD_SET(fdserver,&FdsToRead);
    int max;
    max = fdserver;
    socklen_t cl;
    timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    int r; 
    int a = 0;
    int c = 0;
    int d = 0;
    int l = 0;
    int k = 0;
    int jj = 0;
    int jss = 0;
    int hhh = 0;

    int flag[150] = {0};
    while(1)
    {
         cpyFdsToRead = FdsToRead;
         cpyFdsToWrite = FdsToWrite;
       // memcpy(&cpyFdsToRead,&FdsToRead,sizeof(FdsToRead));
       // memcpy(&cpyFdsToWrite,&FdsToWrite,sizeof(FdsToWrite));
        r = select(max+1,&cpyFdsToRead,&cpyFdsToWrite,NULL,&tv);
        if ( r == -1)
            write(1,"failed\n",8);
        else if(!r) 
        {
            write(1,"timeout\n",9);
            continue;
        }
        else
        {
            for(int i = 0; i < max + 1 ;i++)
            {
                if(FD_ISSET(i, &cpyFdsToRead))
                {
                    if (i == fdserver)
                    {
                        fdclient = accept(fdserver,(sockaddr *)&client,&cl);
                        if (fdclient > max)
                            max = fdclient;
                        FD_SET(fdclient, &FdsToRead);
                        std::cout << "new connection from " << fdclient <<std::endl;
                    }
                    else
                    {
                        int o = read(i,request,1024);
                        request[o] = 0;
                        if(o <= 0)
                        {
                            FD_CLR(i,&FdsToRead);
                           // close(i);
                            //std::cerr << "close connection" << std::endl;
                        }
                        if(flag[i] == 0)
                        {
                            if (std::string(request).find("GET /favicon.ico HTTP/1.1",0) != -1)
                            {
                                std::cout << "new Reaquest from " << i <<std::endl;
                                std::cout << "check favicon " << i <<std::endl;;
                                std::cout << request << std::endl;
                                flag[i] = 3;
                                  FD_SET(i, &FdsToWrite);
                                std::cout << "set " << i << " to write" << std::endl; 
                                 FD_CLR(i,&FdsToRead);
                                std::cout << "clear " << i << " from read" << std::endl;
                            }
                            else if (std::string(request).find("GET /amy.mp4 HTTP/1.1",0) != -1)
                            {
                                std::cout << "new Reaquest from " << i <<std::endl;
                                std::cout << "check amy " << i <<std::endl;
                                std::cout << request << std::endl;
                                 flag[i] = 2;
                                 FD_SET(i, &FdsToWrite);
                                std::cout << "set " << i << " to write" << std::endl; 
                                 FD_CLR(i,&FdsToRead);
                                std::cout << "clear " << i << " from read" << std::endl;
                            }
                            else if (std::string(request).find("GET / HTTP/1.1",0) != -1)
                            {
                                std::cout << "new Reaquest from " << i <<std::endl;
                                flag[i] = 1;
                                std::cout << "check root " << i <<std::endl;
                                std::cout << request << std::endl;
                                  FD_SET(i, &FdsToWrite);
                                std::cout << "set " << i << " to write" << std::endl; 
                                 FD_CLR(i,&FdsToRead);
                                std::cout << "clear " << i << " from read" << std::endl;
                            }
                            else if (std::string(request).find("GET /index.html HTTP/1.1",0) != -1)
                            {
                                std::cout << "new Reaquest from " << i <<std::endl;
                                std::cout << "check index " << i <<std::endl;;
                                std::cout << request << std::endl;
                                flag[i] = 4;
                                  FD_SET(i, &FdsToWrite);
                                std::cout << "set " << i << " to write" << std::endl; 
                                 FD_CLR(i,&FdsToRead);
                                std::cout << "clear " << i << " from read" << std::endl;
                            }
                            else if (std::string(request).find("GET /index.js HTTP/1.1",0) != -1)
                            {
                                std::cout << "new Reaquest from " << i <<std::endl;
                                std::cout << "check index.js " << i <<std::endl;;
                                std::cout << request << std::endl;
                                flag[i] = 5;
                                  FD_SET(i, &FdsToWrite);
                                std::cout << "set " << i << " to write" << std::endl; 
                                 FD_CLR(i,&FdsToRead);
                                std::cout << "clear " << i << " from read" << std::endl;
                            }
                        }
                        else
                            FD_SET(i, &FdsToRead);
                    }
                }
                else if (FD_ISSET(i,&FdsToWrite))
                {
                    //if (std::string(request).find("GET /favicon.ico HTTP/1.1",0) != -1 && flag[i] == 3)
                    if(flag[i] == 3)
                    {
                       if(l == 0)
                        {
                            char *resp =new char[1024];
        resp = (char *)"HTTP/1.1 200 OK\r\nContent-Length: 82830\r\nContent-type: image/jpeg\r\nConnection: keep-alive\r\n\r\n";                        
                           send(i,resp,strlen(resp),0); 
                            l = 10;
                            bzero(request,1024);

                       }
                        else
                        {
                            a = read(img,str,1024);
                            if(a)
                            {
                                FD_CLR(i,&FdsToWrite);
                                FD_SET(i,&FdsToRead);
                                close(img);
                                img = open("www/immg.jpeg",O_RDONLY);
                                l = 0;
                                // std::cout << "***************" << std::endl;
                            }
                            else
                            {
                                send(i,str,a,0);
                                FD_SET(i, &FdsToWrite);
                                std::cout << "send img to " << i << " size : " << a <<std::endl;
                            }
                        }
                        bzero(str,1024);
                    }
                   // else if (std::string(request).find("GET /amy.mp4 HTTP/1.1",0) != -1 && flag[i] == 2)
                    else if(flag[i] == 2)
                    {
                        if (k == 0)
                        {
                            char *resp =new char[1024];
    resp = (char *)"HTTP/1.1 200 OK\r\nContent-Length: 64631923\r\nContent-type: video/mp4\r\nConnection: keep-alive\r\nPragma: no-cache\r\nCache-Control: no-cacher\r\n\r\n";
                            send(i,resp,strlen(resp),0); 
                            k = 10;
                        }
                        else
                        {
                            a = read(mp4,str,1024);
                            if(!a)
                            {
                                FD_CLR(i, &FdsToWrite);
                                FD_SET(i,&FdsToRead);
                                close(mp4);
                                mp4 = open("www/amy.mp4",O_RDONLY);
                                flag[i] = 0;
                                k = 0;
                            }
                            else
                            {
                                int  f = send(i,str,a,0);
                                if (f < 0)
                                {
                                    close(mp4);
                                    mp4 = open("www/amy.mp4",O_RDONLY);
                                    FD_CLR(i,&FdsToWrite);
                                    flag[i] = 0;
                                    k = 0;
                                }
                                else
                                    FD_SET(i, &FdsToWrite);
                                std::cout << "send mp4 to " << i << " size : " << f <<std::endl;
                            }
                        }
                    }
                    else if (flag[i] == 1)
                    {
                        a = read(h,str,1024);
                        send(i,str,a,0);
                        bzero(str,1024);
                        FD_CLR(i,&FdsToWrite);
                        FD_SET(i,&FdsToRead);
                        close(h);
                        h = open("www/w.html",O_RDONLY);
                        flag[i] = 0;
                    }
                    else if (flag[i] == 4)
                    {
                           
                        if (hhh == 0)
                        {
                            char *resp =new char[1024];
    resp = (char *)"HTTP/1.1 200 OK\r\nContent-Length: 2122\r\nContent-type: text/html\r\nConnection: keep-alive\r\nPragma: no-cache\r\nCache-Control: no-cacher\r\n\r\n";
                            send(i,resp,strlen(resp),0); 
                            hhh = 10;
                        }
                        else
                        {
                            a = read(html,str,1024);
                            if(!a)
                            {
                                std::cout << "clear" << std::endl;
                                FD_CLR(i, &FdsToWrite);
                                FD_SET(i,&FdsToRead);
                                close(html);
                                html = open("www/index.html",O_RDONLY);
                                hhh = 0;
                                flag[i] = 0;

                            }
                            else
                            {
                                int  f = send(i,str,a,0);
                                FD_SET(i, &FdsToWrite);
                                std::cout << "send html to " << i << " size : " << f <<std::endl;
                            }
                        }
                    }
                    else if (flag[i] == 5)
                    {
                        if (jss == 0)
                        {
                            char *resp =new char[1024];
    resp = (char *)"HTTP/1.1 200 OK\r\nContent-Length: 4580\r\nContent-type: javascript\r\nConnection: keep-alive\r\nPragma: no-cache\r\nCache-Control: no-cacher\r\n\r\n";
                            send(i,resp,strlen(resp),0); 
                            jss = 10;
                        }
                        else
                        {
                            a = read(js,str,1024);
                            if(!a)
                            {
                                FD_CLR(i, &FdsToWrite);
                                FD_SET(i,&FdsToRead);
                                jss = 0;
                                close(js);
                                html = open("www/index.js",O_RDONLY);
                               flag[i] = 0;
                            }
                            else
                            {
                                int  f = send(i,str,a,0);
                                FD_SET(i, &FdsToWrite);
                                std::cout << "send js to " << i << " size : " << f <<std::endl;
                            }
                        }   
                    }
                }
            }
        }
    }
    return 0;
}
