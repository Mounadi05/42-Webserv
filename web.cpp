#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
int main ()
{
        int fd_s;
        int fd_c;
        int a;
       socklen_t client_add;
        struct sockaddr_in server , client;
        fd_s = socket(AF_INET,SOCK_STREAM,0);
        if (fd_s == -1)
            std::cout << "failed to creat socket" << std::endl;
        else
            std::cout << "create socket" << std::endl;
        bzero(&server,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(1112);
        if (bind(fd_s,(struct sockaddr *)&server,sizeof(server)) == -1)
            std::cout << "failed to bind" << std::endl;
        else
            std::cout << "bind is done " << std::endl;
        if (listen(fd_s,1) == -1)
            std::cout << "failed to listen" << std::endl;
        else
            std::cout << "listen ...." << std::endl;
        bzero(&client,sizeof(client));
        while(1)
        {
            fd_c = accept(fd_s,(struct sockaddr *)&client ,&client_add);
            if(fd_c == -1)
                std::cout << "failed to accept" << std::endl;
            else
                std::cout << "accept request" << std::endl;
            //client_add = ntohl(client_add);
            //std::cout << client_add << std::endl;
            char *str = new char[1000];
            char *s = new char[1000];
            while(1)
            {
                a  = read(fd_c,s,999);
                s[a] = '\0';
                std::cout<<"user2 : " << s;
                write(1,"> : ",strlen("> : "));
                a = read(0,str,999);
                str[a] = '\0';
                write(fd_c,str,strlen(str));
            }
            close(fd_c);
        }


}