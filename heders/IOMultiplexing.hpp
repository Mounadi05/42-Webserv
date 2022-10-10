#ifndef IOMultiplexing_class
#define IOMultiplexing_class

#include "WebServer.hpp"

class IOMultiplexing{
    private :
        fd_set _readFDs;
        fd_set _cpy_readFDs;
        fd_set _writeFDs;
        fd_set _cpy_writeFDs;
        timeval _timeOutVal;
};

#endif