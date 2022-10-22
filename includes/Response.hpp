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
        int fd;
        int size;
        int _send;
        int done;
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
    void send_data(fd_set &r , fd_set &w)
    {
        struct stat st;
        std::string Path = "www";
        if (_request.Getfirst_line() == 1)
        {
            Path = delete_space((Path + _request.Getrequest().at("Path")));
            if (Path == "www/")
                Path = "www/file.html";
            if (access((const char *)Path.c_str(),F_OK) != -1)
            {
                if (!finish)
                {
                    stat((const char *)Path.c_str(), &st);
                    size = st.st_size;
                    fd = open(Path.c_str(),O_RDONLY);
                    bzero(str,1025);
                    std::string header;
                    header = (char *)"HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + delete_space(get_type(Path))+"\r\nConnection: " + delete_space(_request.Getrequest().at("Connection"))+ "\r\n\r\n";
                    write(_ClientFD,header.c_str(),header.size());
                    finish = 10;
                }
                lent = read(fd,str,1024);
                _send = send(_ClientFD,str,lent,0);
                lent_re += _send;
                if (_send == -1)
                {
                    FD_CLR(_ClientFD,&w);
                    FD_SET(_ClientFD,&r);
                    done = 1;
                    close(fd);
                }
                else if (lent_re >= size)
                {
                    FD_CLR(_ClientFD,&w);
                    FD_SET(_ClientFD,&r);
                    close(fd);
                    lent_re = 0;
                    done = 1;
                }
            }
        }
        else if (_request.Getfirst_line() == 0)
        {  
            str = (char *)"HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Length: 73\r\n\r\n<!DOCTYPE html><head><title>NOT FOUND</title></head><body> </body></html>";
            write(_ClientFD,str,strlen(str));
            lent_re = 0;
            done = 1;
            FD_CLR(_ClientFD,&w);
        }
    }

};

#endif

