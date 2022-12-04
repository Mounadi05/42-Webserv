#include "../../includes/Webserv.hpp"

Response::Response()
{

}

Response::Response(Request  request,Server  server, int ClientFD)
{
    _request = request;
    _server = server;
    _ClientFD = ClientFD;
    str = new char[1025];
    lent  = 0;
    ok = 0;
    lent_re = 0;
    size = 0;
    _send = 0;
    done = 0;
    en_handle = 0;
    post = 0;
    fd_upload = 0;
    lent_upload = 0;
    lent_chunked = 0;
    lent_server = 0;
}

Response::~Response()
{

}

int Response::getClientFD() const{
    return _ClientFD;
}

Request & Response::getRequest()
{
    return _request;
}

Server & Response::getServer()
{
    return _server;
}

int &Response::get_done(void)
{
    return done;
}

std::string Response::get_extension(std::string str)
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

std::string Response::delete_space(std::string str)
{   
    for(int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i,1);
    return str;
}

std::string Response::get_type(std::string path)
{
    std::string tmp = get_extension(path);
    for(int i = 0; i < (int) _server.getmime_types().size(); i++)
        if ((int)_server.getmime_types()[i].find(tmp,0) != -1)
            return _server.getmime_types()[i].substr(0,_server.getmime_types()[i].find("|",0));
    return path;
}

int Response::check_location(fd_set &r , fd_set &w)
{
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    {
        if ((int)Path.find(_server.getLocations().at(a).getLocationPath() + "/") != -1 || Path == _server.getLocations().at(a).getLocationPath())
        {
            root = _server.getLocations().at(a).getRoot();
            return 1;
        }
    }
    std::string message=(char *)"HTTP/1.1 404 \r\nConnection: close\r\nContent-Length: 73";
    message +="\r\n\r\n<!DOCTYPE html><head><title>Not Found</title></head><body> </body></html>";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
    return 0;
}
 

int Response::is_Valide(fd_set &r , fd_set &w)
{

    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if (Method != "GET" && Method != "POST" && Method != "PUT" && Method != "PATCH" && Method != "DELETE"
    && Method != "COPY" && Method != "HEAD" && Method != "OPTIONS" && Method != "LINK" && Method != "UNLINK"
    && Method != "PURGE" && Method != "LOCK" && Method != "UNLOCK" &&Method!= "PROPFIND"&& Method != "VIEW"
    && Version != "HTTP/1.1"&& Version != "HTTP/1.0" && Version != "HTTP/2.0" && Version != "HTTP/3.0")
    {
        std::string message=(char *)"HTTP/1.1 400 \r\nConnection: close\r\nContent-Length: 75\r\n\r\n";
        message +="<!DOCTYPE html><head><title>Bad Request</title></head><body> </body></html>";
        send(_ClientFD,message.c_str(),message.size(),0);
        FD_CLR(_ClientFD,&w);
        FD_SET(_ClientFD,&r);
        done = 1;
        return 0;
    }
    return 1;
}

int Response::handle_redirection(fd_set &r , fd_set &w)
{
    Path = delete_space((_request.Getrequest().at("Path")));
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    {
        if (Path == _server.getLocations().at(a).getLocationPath())
        {
            if (!_server.getLocations().at(a).getRedirection().second.empty())
            {std::string message = (char *)"HTTP/1.1 302 Found\r\nLocation: ";
            message +=_server.getLocations().at(a).getRedirection().second;
            message += "\r\nContent-Length: 0\r\n\r\n";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return 0;}
        }
    }
    return 1;
}

 int Response::handle_method(fd_set &r , fd_set &w)
{
    Path = delete_space((_request.Getrequest().at("Path")));
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    {
        if ((int)Path.find(_server.getLocations().at(a).getLocationPath()) != -1)
        {
            if (!_server.getLocations().at(a).getClientMaxBodySize().empty())
                lent_server = strtoull(_server.getLocations().at(a).getClientMaxBodySize().c_str(),NULL, 10);
            else
                lent_server = -1;
            for(int i = 0; i < (int)_server.getLocations().at(a).getAllowedMethods().size();i++)
                if (delete_space(_server.getLocations().at(a).getAllowedMethods().at(i)) == delete_space(_request.Getrequest().at("Method")))
                    return 1;
            std::string message=(char *)"HTTP/1.1 405 \r\nConnection: close\r\nContent-Length: 82\r\n\r\n";
            message +="<!DOCTYPE html><head><title>Method Not Allowed</title></head><body> </body></html>";
            send(_ClientFD,message.c_str(),message.size(),0);
            FD_CLR(_ClientFD,&w);
            FD_SET(_ClientFD,&r);
            done = 1;
            return 0;
        }
    }
    return 1;
}

int Response::handle_index()
{
    std::string str = Path.substr(0,Path.find("/",1));
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    {
        if (Path == _server.getLocations().at(a).getLocationPath() || Path == (_server.getLocations().at(a).getLocationPath()+"/"))
        {
           for(int i = 0; i < (int)_server.getLocations()[a].getIndex().size(); i++)
            {
                full_path = root + _server.getLocations()[a].getIndex().at(i);
                if( access((const char *)full_path.c_str(),F_OK) != -1)
                    return 1;
            }
        }
    }
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    {
        if (str == _server.getLocations().at(a).getLocationPath() && Path  != _server.getLocations().at(a).getLocationPath() + "/" && !_server.getLocations()[a].getIndex().empty())
        {
            full_path = Path.replace(0,_server.getLocations().at(a).getLocationPath().length(),_server.getLocations().at(a).getRoot());
            if( access((const char *)full_path.c_str(),F_OK) != -1)
                return 1;
        }
    }
    return 0;
}

int Response::is_Unauthorize(fd_set &r , fd_set &w)
{
    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if ((Method != "GET" && Method != "POST" && Method != "DELETE"))
    {
        std::string message=(char *)"HTTP/1.1 501 \r\nConnection: close\r\nContent-Length: 79\r\n\r\n";
        message +="<!DOCTYPE html><head><title>Not Implemented</title></head><body> </body></html>";
        send(_ClientFD,message.c_str(),message.size(),0);
        FD_CLR(_ClientFD,&w);
        FD_SET(_ClientFD,&r);
        done = 1;
        return 0;
    }
    if ((Version != "HTTP/1.1" && Version != "HTTP/1.0"))
    {
        std::string message=(char *)"HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 90\r\n\r\n";
        message +="<!DOCTYPE html><head><title>HTTP Version Not Supported</title></head><body> </body></html>";
        send(_ClientFD,message.c_str(),message.size(),0);
        FD_CLR(_ClientFD,&w);
        FD_SET(_ClientFD,&r);
        done = 1;
        return 0;
    }
    return 1;
}

int   Response::redirect_path(fd_set &r , fd_set &w)
{
    for(int a = 0;a < (int)_server.getLocations().size(); a++)
    {
        if (Path == _server.getLocations().at(a).getLocationPath() || ((int) Path.find(".") == -1 && Path[Path.length()-1] != '/'))
        {
            std::string message = (char *)"HTTP/1.1 301 Moved Permanently\r\nLocation: ";
            message += Path + "/";
            message += "\r\nContent-Length: 0\r\n\r\n";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return 0;
        }
    }
    return 1;
}

void Response::send_data(fd_set &r , fd_set &w)
{
    struct stat st;
    if (access((const char *)full_path.c_str(),F_OK) != -1)
    {
        if (!ok)
        {
            stat((const char *)full_path.c_str(), &st);
            size = st.st_size;
            fd = open(full_path.c_str(), O_RDONLY);
            bzero(str, 1025);
            std::string header;
            header = (char *)"HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: "; 
            header += delete_space(get_type(full_path)) + "\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
            write(_ClientFD, header.c_str(), header.size());
            ok = 10;
        }
        lent = read(fd,str,1024);
        _send = send(_ClientFD,str,lent,0);
        lent_re += _send;
        if (_send == -1)
        {
            FD_CLR(_ClientFD,&w);
            FD_SET(_ClientFD,&r);
            done = 1;
         //  if ((int)full_path.find("www/autoindex/") != -1)
          //      remove(full_path.c_str());
            close(fd);
        }
        else if (lent_re >= size)
        {
            FD_CLR(_ClientFD,&w);
            FD_SET(_ClientFD,&r);
            close(fd);
            lent_re = 0;
            done = 1;
           // if ((int)full_path.find("www/autoindex/") != -1)
             //   remove(full_path.c_str());
        }
    }
}

int Response::handle_autoindex(fd_set &r , fd_set &w)
{
    //srand(time(0));
    //int n = rand() % 10;
    std::ofstream file;
    std::string str = Path.substr(0,Path.find("/",1));
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    {
        if (Path == _server.getLocations().at(a).getLocationPath() || Path == (_server.getLocations().at(a).getLocationPath()+"/"))
        {
            if (_server.getLocations().at(a).getAutoIndex() == "on")
            {
                full_path = "www/autoindex/e.html";
                file.open(full_path);
                file.clear();
                file << generate_autoindex(_server.getLocations().at(a).getRoot());
                file.close();
                return 1;
            }
        }
    }
    for(int a = 0;a < (int) _server.getLocations().size(); a++)
    { 
        if (str == _server.getLocations().at(a).getLocationPath() && Path  != _server.getLocations().at(a).getLocationPath() + "/")
        {
            if (_server.getLocations().at(a).getAutoIndex() == "on")
            {
                if (((int) Path.find(".") == -1 && Path[Path.length()-1] == '/'))
                {
                    full_path = "www/autoindex/e.html";
                    file.open(full_path);
                    file.clear();
                    file << generate_autoindex(Path.replace(0,_server.getLocations().at(a).getLocationPath().length(),_server.getLocations().at(a).getRoot()));
                    file.close();
                }
                else
                    full_path = Path.replace(0,_server.getLocations().at(a).getLocationPath().length(),_server.getLocations().at(a).getRoot());
                return 1;
            }
        }
    }
    std::string message=(char *)"HTTP/1.1 403 \r\nConnection: close\r\nContent-Length: 73\r\n\r\n";
    message += "<!DOCTYPE html><head><title>Forbidden</title></head><body> </body></html>";
    send(_ClientFD,message.c_str(),message.size(),0);
    FD_CLR(_ClientFD,&w);
    FD_SET(_ClientFD,&r);
    done = 1;
    return 0;
}

int Response::check_upload(fd_set &r , fd_set &w)
{
    if(!_server.getUploadPath().empty())
    {
        upload = _server.getUploadPath() + _request.Getrequest().at("Path").substr(last_slash());
        return 1;
    }
    std::string message=(char *)"HTTP/1.1 500 \r\nConnection: close\r\nContent-Length: 85";
    message +="\r\n\r\n<!DOCTYPE html><head><title>Internal Server Error</title></head><body> </body></html>";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
    return 0;
}

int Response::last_slash()
{
    int a = 0;
    int i = 0;
    a = i = _request.Getrequest().at("Path").find("/",0);
    while(i != -1)
    {
        a = i;
        i = _request.Getrequest().at("Path").find("/",i+1);
    }
    return a;
}

void Response::write_body(char *str,fd_set &r , fd_set &w)
{
    lent_chunked +=  _request.GetLent();
    write(fd_upload, str, _request.GetLent());
    if (lent_chunked >= lent_upload)
    {
        std::string message = (char *)"HTTP/1.1 201 Created\r\nLocation: ";
        message +=upload + "\r\nContent-Length: 0\r\n\r\n";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        std::cout<< "is : "<< lent_chunked << std::endl;
        close(fd_upload);
    }
    else
    {
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
    }

}
void Response::handle_upload(fd_set &r , fd_set &w)
{
    if (!post)
    {
        post = 1;
        ok = 1;
        fd_upload = open(upload.c_str(), O_CREAT | O_RDWR, 0644);
    }
    if (_request.check_chunked() == 0)
    {
        lent_upload = strtoull(_request.Getrequest().at("Content-Length").c_str(),NULL, 10);
        if(lent_upload > lent_server)
        {
            std::string message=(char *)"HTTP/1.1 413 \r\nConnection: close\r\nContent-Length: 85";
            message +="\r\n\r\n<!DOCTYPE html><head><title>Payload Too Large</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            close(fd_upload);
        }
        else
            write_body((char *)_request.send_body().c_str(),r ,w);
    }
}

int Response::handler(fd_set &r , fd_set &w)
{
     if (!ok)
        full_path = Path = delete_space((_request.Getrequest().at("Path")));
    if(ok || is_Valide(r,w))
        {if(ok || is_Unauthorize(r,w))
            {if(ok || check_location(r,w))
                {if(ok || handle_redirection(r,w))
                    {if(ok || handle_method(r,w))
                        {if (delete_space(_request.Getrequest().at("Method")) == "POST")
                            {if(post || check_upload(r,w))
                                handle_upload(r,w);
                            }
                         else if(ok || redirect_path(r,w))
                            {if(ok || handle_index() || handle_autoindex(r,w))
                                send_data(r,w);
                            }
                        }
                    }
                }
            } 
        }
    return 1;
}