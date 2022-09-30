#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>
int main ()
{
        int fd_c;
        char *str = new char[1000];
        char *s = new char[1000];
        int len;
        int a = 0;
        std::string buff;
        struct sockaddr_in client;
        fd_c = socket(AF_INET,SOCK_STREAM,0);
        if (fd_c == -1)
            std::cout << "failed to creat socket" << std::endl;
        bzero(&client,sizeof(client));
        client.sin_family=AF_INET;
        client.sin_addr.s_addr = inet_addr("10.12.8.4");
        client.sin_port = htons(1111);
        //write(fd_c,"test\n",6);
         if(connect(fd_c,(struct sockaddr *)&client, sizeof(client)) == -1)
            std::cout << "failed to connect" << std::endl,exit(0);
        else
            std::cout  << "connect ..." << std::endl;
        while(1)
        {
            write(1,"> : ",strlen("> : "));
            a = read(0,s,999);
            s[a] = '\0';
            write(fd_c,s,strlen(s));
            len = read(fd_c,str,999);
            str[len]='\0';
            std::cout <<"user1 : " <<str;
        }
        close(fd_c);
}