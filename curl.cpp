#include "includes/Webserv.hpp"

int main(int argc, char const *argv[])
{
    sockaddr_in client;
    int fd;
    int client_fd;
    socklen_t client_len;
    char buffer[1024];
    int bytes_read;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        std::cout << "Error: socket" << std::endl;
    client.sin_family = AF_INET;
    client.sin_port = htons(8000);
    // client.sin_addr.s_addr = INADDR_ANY;
    inet_aton("10.12.8.4",(in_addr*)&client);
    if (connect(fd, (sockaddr *)&client, sizeof(client)) == -1)
        std::cout << "Error: connect" << std::endl;
    char request[] = "GET / HTTP/1.1\r\n\r\n";
    if (send(fd, request, strlen(request), 0) == -1)
        std::cout << "Error: send" << std::endl;
    bytes_read = recv(fd, buffer, 1024, 0);
    if (bytes_read == -1)
        std::cout << "Error: recv" << std::endl;
    buffer[bytes_read] = '\0';
    std::cout << buffer << std::endl;
    return 0;
}