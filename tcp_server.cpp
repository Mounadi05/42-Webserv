#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <strings.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

typedef struct sockaddr_in t_sockaddr_in;

int main()
{
  int S_SocketFd, C_socketFd;
  t_sockaddr_in sockaddr_server, sockaddr_client;
  socklen_t clientaddrLength;
  fd_set readfds, copyfd;
  int fdmax;
  struct timeval tv;
  // socket creation
  S_SocketFd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(S_SocketFd, F_SETFL, O_NONBLOCK);
  fdmax = S_SocketFd;
  if (S_SocketFd == -1)
    std::cout << "socket creation failed" << std::endl;
  // binding
  bzero(&sockaddr_server, sizeof(sockaddr_server));
  // inet_aton("127.0.0.1",&sockaddr_server.sin_addr);
  sockaddr_server.sin_family = AF_INET;
  // sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr_server.sin_addr.s_addr = inet_addr("0.0.0.0");
  // std::cout << inet_addr("127.0.0.1") << std::endl;
  // std::cout << inet_ntoa(sockaddr_server.sin_addr) << std::endl;
  sockaddr_server.sin_port = htons(4040);
  int optval = 1;
  setsockopt(S_SocketFd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int));
  if (bind(S_SocketFd, (struct sockaddr *)&sockaddr_server, sizeof(sockaddr_server)) == -1)
    std::cout << "bind failed" << std::endl;
  // listening
  if (listen(S_SocketFd, 1) == -1)
    std::cout << "listen syscall failed" << std::endl;
  else
    std::cout << "Socket listening on port " << ntohs(sockaddr_server.sin_port) << std::endl;
  FD_ZERO(&readfds);
  FD_SET(S_SocketFd, &readfds);
  struct timeval timeout;      
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  while (1)
  {
    copyfd = readfds;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int r = select(fdmax + 1, &copyfd, NULL, NULL, &tv);
    if (r == -1)
    {
      std::cout << "Error in select" << std::endl;
      break;
    }
    if (r == 0)
      continue;
    else
    {
      for (int i = 0; i < fdmax + 1; i++)
      {
        if (FD_ISSET(i, &copyfd))
        {
          if (i == S_SocketFd)
          {
            clientaddrLength = sizeof(sockaddr_client);
            C_socketFd = accept(S_SocketFd, (struct sockaddr *)&sockaddr_client, &clientaddrLength);
            if (C_socketFd == -1)
              std::cout << "accept failed" << std::endl;
            else
            {
              std::cout << "new connection from " << inet_ntoa(sockaddr_client.sin_addr) << " on socket " << C_socketFd << std::endl;
              FD_SET(C_socketFd, &readfds);
              if (C_socketFd > fdmax)
                fdmax = C_socketFd;
            }
          }
          else
          {
            char buff[1024];
            // int r = read(i, buff, 1024);
            int r = recv(i,buff,1024,0);
            if (r <= 0)
            {
              std::cout << "socket " << i << " hung up" << std::endl;
              close(i);
              FD_CLR(i, &readfds);
            }
            else
            {
              // std::cout << "request from socket " << i << std::endl;
              buff[r] = 0;
              std::cout << buff << std::endl;
              std::cout << "respone to socket " << i << std::endl;
              char resp[] = "HTTP/1.1 200 OK\r\n"
                            "Content-Length: 49\r\n"
                            "Content-Type: text/html\r\n"
                            "\r\n"
                            "<html><body><h1> Hello,World !</h1></body></html>";
              send(i, resp, std::string(resp).size(), 0);
            }
          }
        }
      }
    }
  }
  close(S_SocketFd);
  return 0;
}
