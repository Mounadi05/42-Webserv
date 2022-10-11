#ifndef Response_class
#define Response_class

#include <map>
class Response{
    public :
        int _codeStatus;
        std::string _version;
        std::string _body;
        std::map<std::string, std::string> _headers;
        Response();
        ~Response();
        Response(const Response &Res);
        Response &operator=(const Response &Res);
};

#endif