#include "../includes/WebServer.hpp"

Client::Client() {
    _request = Request();
    _response = Response();
    _socket = Socket();
}

Client::Client(const Client &cl) {
    *this = cl;
}

Client &Client::operator=(const Client &cl) {
    if (this != &cl) {
        _request = cl._request;
        _response = cl._response;
        _socket = cl._socket;
    }
    return *this;
}

Client::~Client() {
}
