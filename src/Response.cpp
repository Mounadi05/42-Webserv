#include "../includes/WebServer.hpp"

Response::Response()
{
    this->_version = "";
    this->_codeStatus = 0;
    this->_body =  "";

}

Response::~Response()
{

}

Response::Response(const Response &Resp)
{
    *this = Resp;
}

Response &Response::operator=(const Response &Req)
{
    if (this != &Req)
    {
        this->_codeStatus = Req._codeStatus;
        _version = Req._version;
        _body = Req._body;
        _headers = Req._headers;
    }
    return *this;
}