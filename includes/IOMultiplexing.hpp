#ifndef IOMultiplexing_class
#define IOMultiplexing_class

#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>

class IOMultiplexing{
    public :
        fd_set _readFDs;
        fd_set _cpy_readFDs;
        fd_set _writeFDs;
        fd_set _cpy_writeFDs;
        timeval _timeOutVal;
        int _maxFD;
        int _selectRet;
        IOMultiplexing();
        IOMultiplexing(const IOMultiplexing &io);
        IOMultiplexing &operator=(const IOMultiplexing &io);
        ~IOMultiplexing();
};

#endif