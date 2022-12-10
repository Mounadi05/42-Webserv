#ifndef Socket_class
#define Socket_class

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class Socket{
    private:
        int _socket_fd;
        struct sockaddr_in  *_sockaddr;
    public:
        Socket();
        ~Socket();
        struct sockaddr_in *getSockAddr() const;
        int getSocketFd() const;
        void setSocketFd(int fd);
        void setSockAddr(struct sockaddr_in sockAddr);
        Socket &operator=(const Socket &rhs);
        Socket(const Socket &rhs);
};

#endif