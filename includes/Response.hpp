#ifndef Response_class
#define Response_class

#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include <fstream>

class Response
{
    private:
        int _ClientFD;
        Request _request;
        Server _server;
        std::string queryParams;
        size_t _statusCode;
        char *str;
        int lent;
        int done;
        int finish;
        int fd;
        int size;
        int _send;
    public:
        Response();
        Response(Request request,Server  server, int ClientFD);
        ~Response();
        int getClientFD() const;
        Request & getRequest();
        Server & getServer();
        std::string & getQuerys() {
            return this->queryParams;
        };
        size_t & getStatusCode() {
            return this->_statusCode;
        };
        int & get_done() {
            return done;
        };
        std::string extractQueryParams(std::string path);
        std::string get_extension(std::string str);
        std::string get_type(std::string path);
        std::string delete_space(std::string str);
        int         handler(fd_set &r , fd_set &w);
        int         defineLocation(std::vector<Location> location, std::string uriPath);
        std::string setFullPath(Server server, std::string uriPath, int locationIndex);
        int         isBadRequest(); // check for bad request
        int         is_unsupportedVersion(); // check for http version
        int         is_notImplemented();
        int         isAllowedMethod(Server server, Location locationBlock, std::string requestedMethod); // check for methods allowed by server
        int         isPayloadTooLarge(Server server, Location locationBlock, int contentLengthRequested);
        int         defineFileType(std::string pathToResource);
        int         shouldListContent(Server server, int locationIndex);
        int         shouldRedirectUrl(Location locationBlock, std::string pathtosearch);
        int         isForbiddenResource(std::string resource, int locationIndex);
        int         deleteDir(std::string pathToDir);
        int         deleteRequest(std::string pathToDelete);
        void        craftErrorPage(std::string errorMsg, size_t statusCode);
        void        craftResponse(std::string path, std::string msg, size_t statusCode, bool isError);
        int         sendResponse(fd_set &r , fd_set &w);
        size_t      getSizeOfFile(std::string file);
        std::string defineMimeType(std::vector<std::string> mimeTypes, std::string path);
        // this function should be refactored
        // talk to me il explain
    // void send_data(fd_set &r , fd_set &w, std::string Path)
    // {
    //     struct stat st;

    //     if (is_Valide(r,w))
    //     {
    //         if (is_Unauthorize(r,w))
    //         {
    //             if (access((const char *)Path.c_str(),F_OK) != -1)
    //             {
    //                 if (!finish)
    //                 {
    //                     stat((const char *)Path.c_str(), &st);
    //                     size = st.st_size;
    //                     fd = open(Path.c_str(),O_RDONLY);
    //                     bzero(str,1025);
    //                     std::string header;
    //                     header = (char *)"HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + delete_space(get_type(Path))+"\r\nConnection: " + delete_space(_request.Getrequest().at("Connection"))+ "\r\n\r\n";
    //                     write(_ClientFD,header.c_str(),header.size());
    //                     finish = 10;
    //                 }
    //                 lent = read(fd,str,1024);
    //                 _send = send(_ClientFD,str,lent,0);
    //                 lent_re += _send;
    //                 if (_send == -1)
    //                 {
    //                     FD_CLR(_ClientFD,&w);
    //                     FD_SET(_ClientFD,&r);
    //                     done = 1;
    //                     close(fd);
    //                 }
    //                 else if (lent_re >= size)
    //                 {
    //                     FD_CLR(_ClientFD,&w);
    //                     FD_SET(_ClientFD,&r);
    //                     close(fd);
    //                     lent_re = 0;
    //                     done = 1;
    //                 }
    //             } 
    //         }
    //     } 
    // }
};

#endif

