#ifndef Response_class
#define Response_class

#include "Webserv.hpp"
#include <map>
class Response
{
private:
    std::string                         _responseString;
    Server                              _server;
    Request                             _request;
    
    std::string                         _version;
    std::string                         _statusCode;
    std::map<int, std::string>          _statusCodeMsg;
    std::string                         _codeMessage;
    std::vector<std::pair<std::string, std::string> >   _mimeTypes;
    std::map<std::string, std::string>  _headers;
    bool                                _firstLineDone;
    bool                                _headersDone;
    bool                                _responseFinished;
    bool                                _isCgi;
    size_t                              _isLocation;
    size_t                              _rightToAccess;

public:
    Response(Request request, Server server, int clientFd);
    Response();
    ~Response();
    std::string &                           getVersion();
    std::string &                           getStatusCode();
    std::map<int, std::string> &            getStatusCodeMsg();
    std::map<std::string, std::string> &    getHeaders();
    void                                    initStatusMessages();
    void                                    initMimeTypes();
    int                                     isFirstLineFinished();
    int                                     isHeadersFinished();
    int                                     isResponseFinished();

    bool                                    isVersionSupported();
    bool                                    isMethodImplemented();
    void                                    defineFileType(std::string filePath);
};

#endif