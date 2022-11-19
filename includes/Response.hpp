#ifndef Response_class
#define Response_class

#include "Request.hpp"
#include "Server.hpp"
#include <sys/stat.h>
class Response
{
    private:
        int _ClientFD;
        Request _request;
        Server _server;
        char *str;
        int lent;
        int finish;
        int lent_re;
        // int fd;
        int size;
        int _send;
        int done;
        int _state;
        // int _sindex; // server index
        int _lindex; // location index
    public:
        Response();
        Response(Request request,Server  server, int ClientFD);
        ~Response();
        int getClientFD() const;
        Request & getRequest();
        Server & getServer();
        int &get_done(void);
        std::string get_extension(std::string str);
        std::string get_type(std::string path);
        std::string delete_space(std::string str);
        int handler(fd_set &r , fd_set &w);
        int is_Valide(fd_set &r , fd_set &w);
        int is_Unauthorize(fd_set &r , fd_set &w);
        int defineLocation(std::string path);
        int checkMethod(void);
        int sendResponse(fd_set &r, fd_set &w , std::string filePath = "");
        std::string DefineErrorPath(std::string ErrorCode);
        // std::string stringtrim(std::string str);
    // void send_data(fd_set &r , fd_set &w)
    // {
    //     struct stat st;
    //     std::string Path = "";
    //     std::string pathtosearch = delete_space((_request.Getrequest().at("Path")));
    //     for (size_t i = 0; i < _server.getLocations().size(); i++)
    //     {
    //         if (_server.getLocations()[i].getLocationPath() == pathtosearch)
    //             Path = delete_space(_server.getLocations()[i].getRoot() + pathtosearch);
    //     }
    //     std::cout << Path << std::endl;
    //     // Path = delete_space((Path + _request.Getrequest().at("Path")));
    //     // if (Path == "www/")
    //     //     Path = "www/file.html";
    //     // while(1);
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

