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
        std::string Path;
        std::vector<std::string> index;
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
        int handle_location(void);
        int handle_index(void);
    void send_data(fd_set &r , fd_set &w)
    {
        struct stat st;
        Path = delete_space((_request.Getrequest().at("Path")));
       // std::cout << "en_handle "<< _server.en_handle << std::endl;
        if (is_Valide(r,w))
        {
        
            if (is_Unauthorize(r,w))
            {
                if (_server.en_handle || handle_location())
                {
                    if (_server.en_handle  >= 2 || handle_index())
                    {
                        if(_server.en_handle == 3)
                        {
                            Path = _server.root + Path;
                        }
                        if (access((const char *)Path.c_str(),F_OK) != -1)
                        {
                           _server.en_handle = 3;
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
                }
            }
        } 
    }
};

#endif

