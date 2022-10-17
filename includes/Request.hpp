#ifndef Request_class
#define Request_class

#include <iostream>
#include "Response.hpp"

class Request
{
private:
    std::string _Rbuffer;
    // int stats;
    // int _delemiter;

    //std::string _method;
    //std::string _uriPath;
    //std::strign _version;
    //std::vector<std::string> _headers;
    
public:
    Request();
    Request(std::string Buffer);
    ~Request();
    std::string & getRequestBuffer();
    void setRequestBuffer(std::string Buffer);
    // int checkDelemiter(std::string Buffer);
    // void ConcatToBuffer(std::string newBuffer);
};

#endif