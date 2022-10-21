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
        int h;
        int lent;
        int finish;
        int lent_re;
        int fd;
        int size;
    public:
        Response();
        Response(Request request,Server  server, int ClientFD);
        ~Response();
        int getClientFD() const;
        Request & getRequest();
        Server & getServer();
        std::string get_extension(std::string str)
        {
            std::string tmp ;
            int i = str.size();
            while(i != 0 && i--)
            {
                    if (str[i] != '.')
                        tmp.insert(tmp.begin(),str[i]);
                    else
                        return tmp;
            }
            return tmp;
        }

    std::string get_type(std::string path)
    {
        std::string tmp = get_extension(path);
        std::string str;
        std::ifstream file;
        file.open("www/mime.types") ;
        while(getline(file,str))
        {
            if ((int)str.find(tmp,0) != -1 )
            {
                str = str.substr(0,str.find("|",0));
                return str;
            }
            str.clear();
        }
        return path;
    }

    std::string delete_space(std::string str)
    {   
        for(int i = 0; str[i]; i++)
            if (str[i] == ' ')
                str.erase(i,1);
        return str;
    }
    void send_data(fd_set &r , fd_set &w)
    {
        struct stat st;
        std::string Path = "www";
        Path = delete_space((Path + _request.Getrequest().at("Path")));
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
                std::cout << header << std::endl;
                write(_ClientFD,header.c_str(),header.size());
                finish = 10;
            }
            lent = read(fd,str,1024);
            lent_re += send(_ClientFD,str,lent,0);
            std::cout<< "send : " << lent_re << std::endl;
            if (lent_re >= size)
            {
                FD_CLR(_ClientFD,&w);
                FD_SET(_ClientFD,&r);
                close(fd);
                finish = 0;
                lent_re = 0;
            }
        }
        else
        {  
            str = (char *)"HTTP/1.0 404 Not Found\r\n\r\n";
            write(_ClientFD,str,strlen(str));
            lent_re = 0;
            FD_CLR(_ClientFD,&w);
            FD_SET(_ClientFD,&r);
        }
    }
    int handler(fd_set &r , fd_set &w)
    {
        send_data(r,w);
        return 1;
    }

};

#endif

