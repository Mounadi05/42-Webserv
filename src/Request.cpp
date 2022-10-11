#include "../includes/WebServer.hpp"

Request::Request()
{
    _method = "";
    _uri = "";
    _version = "";
    _body = "";
    _headers = std::map<std::string, std::string>();
}

Request::~Request()
{

}

Request::Request(const Request &Req)
{
    *this = Req;
}

Request &Request::operator=(const Request &Req)
{
    if (this != &Req)
    {
        _method = Req._method;
        _uri = Req._uri;
        _version = Req._version;
        _body = Req._body;
        _headers = Req._headers;
    }
    return *this;
}