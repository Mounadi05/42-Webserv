#include "includes/Webserv.hpp"

int main(int argc, char const *argv[])
{
    sockaddr_in client;
    int fd;

    fd = socket(AF_INET,SOCK_STREAM,0);
    client.sin_family = AF_INET;
    client.sin_port = htons(8000);
    inet_aton("10.12.8.6",(in_addr *)&client);
    if (connect(fd,(struct sockadd *)&client,sizeof(client)))
    {
        
    }
    return 0;
}