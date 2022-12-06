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

void Response::write_body(fd_set &r , fd_set &w)
{
    std::cout << "post" << std::endl;
    // char reso[1024];
    // char old_reso[1024];
    // realpath(upload.c_str(), reso);
    // realpath(_request.get_tmp().c_str(), old_reso);
    // std::cout<<"reso :" << reso << std::endl;
    // std::cout<<"old_reso : " << old_reso << std::endl;
    std::cout << "upload is finished" << std::endl;
    rename(_request.get_tmp().c_str(),upload.c_str());
    std::string message = (char *)"HTTP/1.1 201 Created\r\nLocation: ";
    message +=upload + "\r\nContent-Length: 0\r\n\r\n";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
}
int Response::isToo_large(fd_set &r , fd_set &w)
{
    if (_request.check_chunked() == 0)
    {
         if(_request.GetLent() > lent_server)
        {
            std::string message=(char *)"HTTP/1.1 413 \r\nConnection: close\r\nContent-Length: 81";
            message +="\r\n\r\n<!DOCTYPE html><head><title>Payload Too Large</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return 0;
        }
    }
    return 1;
}

int Response::check_lent(fd_set &r , fd_set &w)
{
    if (_request.check_chunked() == 0 && _request.Getrequest().at("Content-Length").empty())
    {
        std::string message=(char *)"HTTP/1.1 411 \r\nConnection: close\r\nContent-Length: 79";
        message +="\r\n\r\n<!DOCTYPE html><head><title>Length Required</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}

int Response::check_Content(fd_set &r , fd_set &w)
{
    std::cout << Path << std::endl;
    Path = "www" + Path;
    std::cout << access(Path.c_str(),F_OK) << std::endl;
    if (access(Path.c_str(),F_OK) == -1)
    {
        std::string message=(char *)"HTTP/1.1 204 \r\nConnection: close\r\nContent-Length: 74";
        message +="\r\n\r\n<!DOCTYPE html><head><title>No Content</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}
void check_all(std::string str,int *a)
{
	struct dirent *d;
	struct stat s;
	stat(str.c_str(),&s);
    if (S_ISDIR(s.st_mode))
    std::cout << "is dir" << std::endl;
    if (S_ISDIR(s.st_mode) && (access(str.c_str(), W_OK) == -1 || access(str.c_str(), X_OK) == -1))
	{
		*a = 1;
		return ;
	}
	str += "/";
	DIR *dir = opendir(str.c_str());
    while(S_ISDIR(s.st_mode) && (d = readdir(dir)) != NULL)
    {
        std::string str1 =str+d->d_name;
        struct stat s1;
        stat(str1.c_str(),&s1);
        if (!strcmp(d->d_name,".") || !strcmp(d->d_name,".."));
        else
        {
            if (S_ISDIR(s1.st_mode))
                check_all((str+d->d_name), a);
            else if (!S_ISDIR(s1.st_mode))
                if (access(str1.c_str(), W_OK) == -1)
                    *a = 1;
        }
    }
    if (dir)
        closedir(dir);
 }
int Response::check_permission(fd_set &r , fd_set &w)
{
    int a = 0;
    struct stat s;
    if (S_ISDIR(s.st_mode) && (access(Path.c_str(), W_OK) == -1  || access(Path.c_str(), X_OK)))
        a = 1;
    else if(!S_ISDIR(s.st_mode) && access(Path.c_str(), W_OK) == -1)
        a = 1;
    else 
        check_all(Path,&a);
    if (a)
    {
        std::string message=(char *)"HTTP/1.1 403 \r\nConnection: close\r\nContent-Length: 73";
        message +="\r\n\r\n<!DOCTYPE html><head><title>Forbidden</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}
void delete_file(std::string str)
{
    struct dirent *d;
	struct stat s;
	stat(str.c_str(),&s);
	str +="/";
	DIR *dir = opendir(str.c_str());
    while(S_ISDIR(s.st_mode) && (d = readdir(dir)) != NULL)
    {
        std::string str1 = str+d->d_name;
        struct stat s1;
        stat(str1.c_str(),&s1);
        if (!strcmp(d->d_name,".") || !strcmp(d->d_name,".."));
        else
        {
            if (S_ISDIR(s1.st_mode))
                delete_file(str+d->d_name);
            else if (!S_ISDIR(s1.st_mode))
            {
                std::string tmp = str1+d->d_name;
                unlink(((const char *)tmp.c_str()));
            }
        }
    }
    if(dir)
        closedir(dir);
    remove(str.c_str());
}
void Response::handler_delete(fd_set &r , fd_set &w)
{
    struct stat s;
  	if (!S_ISDIR(s.st_mode))
        unlink(((const char *)Path.c_str()));
    else
        delete_file(Path);
    std::string message=(char *)"HTTP/1.1 200\r\nConnection: close\r\nContent-Length: 70";
    message +="\r\n\r\n<!DOCTYPE html><head><title>200 OK</title></head><body> </body></html>";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
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
                            {if(check_upload(r,w))
                                {if(check_lent(r,w))
                                    {if(isToo_large(r,w))
                                        write_body(r,w);
                                    }
                                }
                            }
                        else if(delete_space(_request.Getrequest().at("Method")) == "DELETE")
                        {if(check_Content(r,w))
                            {if(check_permission(r,w))
                                    handler_delete(r,w);
                            }
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