#ifndef Request_class
#define Request_class

#include <map>

class Request{
    public :
        std::string _method;
        std::string _uri;
        std::string _version;
        std::string _body;
        std::map<std::string, std::string> _headers;
        Request();
        ~Request();
        Request(const Request &Req);
        Request &operator=(const Request &Req);
};

#endif