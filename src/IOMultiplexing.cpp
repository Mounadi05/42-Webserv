#include "../includes/WebServer.hpp"

IOMultiplexing::IOMultiplexing() {
    FD_ZERO(&_readFDs);
    FD_ZERO(&_cpy_readFDs);
    FD_ZERO(&_writeFDs);
    FD_ZERO(&_cpy_writeFDs);
    _timeOutVal.tv_sec = 5;
    _timeOutVal.tv_usec = 0;
    _maxFD = 0;
    _selectRet = -2;
}

IOMultiplexing::IOMultiplexing(const IOMultiplexing &io) {
    *this = io;
}

IOMultiplexing &IOMultiplexing::operator=(const IOMultiplexing &io) {
    if (this != &io) {
        _readFDs = io._readFDs;
        _cpy_readFDs = io._cpy_readFDs;
        _writeFDs = io._writeFDs;
        _cpy_writeFDs = io._cpy_writeFDs;
        _timeOutVal = io._timeOutVal;
        _maxFD = io._maxFD;
        _selectRet = io._selectRet;
    }
    return *this;
}

IOMultiplexing::~IOMultiplexing() {
}