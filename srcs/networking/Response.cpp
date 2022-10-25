#include "../../includes/Webserv.hpp"

Response::Response()
{
}

Response::Response(Request request, Server server, int ClientFD)
{
    _request = request;
    _server = server;
    _ClientFD = ClientFD;
    str = new char[1025];
    lent = 0;
    finish = 0;
    lent_re = 0;
    size = 0;
    _send = 0;
    done = 0;
}

Response::~Response()
{
}

int Response::getClientFD() const
{
    return _ClientFD;
}

Request &Response::getRequest()
{
    return _request;
}

Server &Response::getServer()
{
    return _server;
}

int &Response::get_done(void)
{
    return done;
}

std::string Response::get_extension(std::string str)
{
    std::string tmp;
    int i = str.size();
    while (i != 0 && i--)
    {
        if (str[i] != '.')
            tmp.insert(tmp.begin(), str[i]);
        else
            return tmp;
    }
    return tmp;
}

std::string Response::delete_space(std::string str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i, 1);
    return str;
}

std::string Response::get_type(std::string path)
{
    std::string tmp = get_extension(path);
    for (int i = 0; i < (int)_server.getmime_types().size(); i++)
        if ((int)_server.getmime_types()[i].find(tmp, 0) != -1)
            return _server.getmime_types()[i].substr(0, _server.getmime_types()[i].find("|", 0));
    return path;
}
int flag  = 0;
int Response::handler(fd_set &r, fd_set &w)
{
    std::string pathtosearch = delete_space((_request.Getrequest().at("Path")));
    std::cout << pathtosearch << std::endl;
    std::string Path = valide_location_path(pathtosearch);
    std::cout << Path << std::endl;
    // while (1);
    send_data(r, w, Path);
    return 1;
}

int Response::is_Valide(fd_set &r, fd_set &w)
{

    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if (Method != "GET" && Method != "POST" && Method != "PUT" && Method != "PATCH" && Method != "DELETE" && Method != "COPY" && Method != "HEAD" && Method != "OPTIONS" && Method != "LINK" && Method != "UNLINK" && Method != "PURGE" && Method != "LOCK" && Method != "UNLOCK" && Method != "PROPFIND" && Method != "VIEW" && Version != "HTTP/1.1" && Version != "HTTP/1.0" && Version != "HTTP/2.0" && Version != "HTTP/3.0")
    {
        std::string message = (char *)"HTTP/1.1 400 \r\nConnection: close\r\nContent-Length: 75\r\n\r\n<!DOCTYPE html><head><title>Bad Request</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}
int Response::is_Unauthorize(fd_set &r, fd_set &w)
{
    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if ((Method != "GET" && Method != "POST" && Method != "DELETE"))
    {
        std::string message = (char *)"HTTP/1.1 501 \r\nConnection: close\r\nContent-Length: 79\r\n\r\n<!DOCTYPE html><head><title>Not Implemented</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    if ((Version != "HTTP/1.1" && Version != "HTTP/1.0"))
    {
        std::string message = (char *)"HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 90\r\n\r\n<!DOCTYPE html><head><title>HTTP Version Not Supported</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}
