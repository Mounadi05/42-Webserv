#include "../../includes/Webserv.hpp"

Response::Response()
{
}

std::string ft_toString(long long n)
{
    std::string str;
    if (n == 0)
        return "0";
    while (n != 0)
    {
        str.insert(str.begin(), n % 10 + '0');
        n /= 10;
    }
    return str;
}

Response::Response(Request request, Server server, int ClientFD)
{
    _request = request;
    _server = server;
    _ClientFD = ClientFD;
    lent = 0;
    ok = 0;
    lent_re = 0;
    size = 0;
    _send = 0;
    done = 0;
    en_handle = 0;
    post = 0;
    fd_upload = 0;
    lent_server = 0;
    fd_error = 0;
    error = 0;
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
    return "text/plain";
}


int comp(const char *str, const char *path)
{
    int i = 0;
    while(str[i] && path[i])
    {
        if (str[i] == path[i])
            i++;
        else
            break;
    }
    if ((!path[i] && !str[i]) || ( !str[i] && path[i] == '/'))
        return 1;
    return 0;
}

int Response::check_location(fd_set &r, fd_set &w)
{
    int flag = 0;
    for (int a = 0; a < (int)_server.getLocations().size(); a++)
    {
        if (comp(_server.getLocations().at(a).getLocationPath().c_str(),Path.c_str()))
        {
            flag = 1;
            locations = _server.getLocations().at(a);
            std::string tmp = Path;
            full_path = locations.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(a).getLocationPath()), _server.getLocations().at(a).getLocationPath().length(), "");
            char str[1025];
            realpath(full_path.c_str(),str);
            full_path = str;
            if (_request.Getrequest().at("Method") == "DELETE")
            {
                std::string tmp = Path;
                Path = locations.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(a).getLocationPath()), _server.getLocations().at(a).getLocationPath().length(), "");
                char res[1024];
                realpath((char *)Path.c_str(), res);
                full_path = res;
            }
            return 1;
        }
    }
    if(!flag)
    {
        for (int a = 0; a < (int)_server.getLocations().size(); a++)
        {
            if (_server.getLocations().at(a).getLocationPath() == "/")
            {
                locations = _server.getLocations().at(a);
                std::string tmp = Path;
                full_path = locations.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(a).getLocationPath()), _server.getLocations().at(a).getLocationPath().length(), "");
                char str[1025];
                realpath(full_path.c_str(),str);
                full_path = str;
                if (_request.Getrequest().at("Method") == "DELETE")
                {
                    std::string tmp = Path;
                    Path = locations.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(0).getLocationPath()), _server.getLocations().at(a).getLocationPath().length(), "");
                    char res[1024];
                    realpath((char *)Path.c_str(), res);
                    full_path = res;
                }
                return 1;
            }
        }
    }
    if (_request.Getrequest().at("Method") == "POST")
    {
        if (access(_request.get_tmp().c_str(), F_OK) != -1)
            remove(_request.get_tmp().c_str());
    }
    if (send_error("404", " Not Found "))
    {
        std::cout << RED << "Response 404 Not Found " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
        std::string message = (char *)"HTTP/1.1 404 \r\nConnection: close\r\nContent-Length: 73";
        message += "\r\n\r\n<!DOCTYPE html><head><title>Not Found</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
    }
    return 0;
}

int Response::is_Valide(fd_set &r, fd_set &w)
{

    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if (Method != "GET" && Method != "POST" && Method != "PUT" && Method != "PATCH" && Method != "DELETE" && Method != "COPY" && Method != "HEAD" && Method != "OPTIONS" && Method != "LINK" && Method != "UNLINK" && Method != "PURGE" && Method != "LOCK" && Method != "UNLOCK" && Method != "PROPFIND" && Method != "VIEW" && Version != "HTTP/1.1" && Version != "HTTP/1.0" && Version != "HTTP/2.0" && Version != "HTTP/3.0")
    {
        if(send_error("400"," Bad Request "))
        {
            std::cout << RED << "Response 400 >Bad Request " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 400 \r\nConnection: close\r\nContent-Length: 75\r\n\r\n";
            message += "<!DOCTYPE html><head><title>Bad Request</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return 0;
        }
    }
    return 1;
}
int Response::handle_redirection(fd_set &r, fd_set &w)
{
    Path = delete_space((_request.Getrequest().at("Path")));
    if (!locations.getRedirection().second.empty())
    {
        std::cout << YELLOW << "Response 302 Found " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
        std::string message = (char *)"HTTP/1.1 302 Found\r\nLocation: ";
        message += locations.getRedirection().second;
        message += "\r\nContent-Length: 0\r\n\r\n";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}

int Response::handle_method(fd_set &r, fd_set &w)
{
    Path = delete_space((_request.Getrequest().at("Path")));
    if ((int)Path.find(locations.getLocationPath()) != -1)
    {
        if (!locations.getClientMaxBodySize().empty())
            lent_server = strtoull(locations.getClientMaxBodySize().c_str(), NULL, 10);
        else
            lent_server = -1;
        for (int i = 0; i < (int)locations.getAllowedMethods().size(); i++)
            if (delete_space(locations.getAllowedMethods().at(i)) == delete_space(_request.Getrequest().at("Method")))
                return 1;
        if (_request.Getrequest().at("Method") == "POST")
        {
            if (access(_request.get_tmp().c_str(), F_OK) != -1)
                remove(_request.get_tmp().c_str());
        }
        if (send_error("405"," Method Not Allowed "))
        {
            std::cout << RED << "Response 405 Method Not Allowed " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 405 \r\nConnection: close\r\nContent-Length: 82\r\n\r\n";
            message += "<!DOCTYPE html><head><title>Method Not Allowed</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
        }
        return 0;
    }
    return 1;
}
int Response::handle_index()
{
    struct stat s;
    stat(full_path.c_str(), &s);
    if ((access((const char *)full_path.c_str(), F_OK) != -1) && !S_ISDIR(s.st_mode))
        return 1;
    for (int i = 0; i < (int)locations.getIndex().size(); i++)
    {
        full_path += "/" +  locations.getIndex().at(i);
        if (access((const char *)full_path.c_str(), F_OK) != -1)
            return 1;
    }
    return 0;
}

int Response::is_Unauthorize(fd_set &r, fd_set &w)
{
    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if ((Method != "GET" && Method != "POST" && Method != "DELETE"))
    {
        if (send_error("501"," Method Not Implemented "))
        {
            std::cout << RED << "Response 501 Method Not Implemented " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 501 \r\nConnection: close\r\nContent-Length: 79\r\n\r\n";
            message += "<!DOCTYPE html><head><title>Not Implemented</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
        }
        return 0;
    }
    if ((Version != "HTTP/1.1" && Version != "HTTP/1.0"))
    {
        if (send_error("505", "HTTP Version Not Supported"))
        {
            std::cout << RED << "Response 505 HTTP Version Not Supported " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 90\r\n\r\n";
            message += "<!DOCTYPE html><head><title>HTTP Version Not Supported</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
        }
        return 0;
    }
    return 1;
}

int Response::redirect_path(fd_set &r, fd_set &w)
{
    struct stat s;
    stat(full_path.c_str(),&s);
    if (((access(full_path.c_str(),F_OK) != -1 && S_ISDIR(s.st_mode))  && Path != "/" && Path[Path.length() - 1] != '/'))
    {
        std::cout << YELLOW << "Response 301 Moved Permanently " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
        std::string message = (char *)"HTTP/1.1 301 Moved Permanently\r\nLocation: ";
        message += Path + "/";
        message += "\r\nContent-Length: 0\r\n\r\n";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}

void Response::send_data(fd_set &r, fd_set &w)
{
    cgi_exec();
    struct stat st;
    if (access((const char *)full_path.c_str(), F_OK) != -1)
    {
        if (!ok)
        {
            stat((const char *)full_path.c_str(), &st);
            size = st.st_size;
            fd = open(full_path.c_str(), O_RDONLY);
            bzero(str, 1025);
            std::string header;
            std::cout << GREEN << "Response 200 OK " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            header = (char *)"HTTP/1.1 200 OK\r\nContent-Length: " + ft_toString(size) + "\r\nContent-type: ";
            header += delete_space(get_type(full_path)) + ((_headers["Set-Cookie"] == "") ? "" : ("\r\nSet-Cookie: " + _headers["Set-Cookie"])) + "\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
            write(_ClientFD, header.c_str(), header.size());
            ok = 10;
        }
        lent = read(fd, str, 1024);
        _send = send(_ClientFD, str, lent, 0);
        lent_re += _send;
        if (_send == -1)
        {
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            close(fd);
        }
        else if (lent_re >= size)
        {
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            close(fd);
            lent_re = 0;
            done = 1;
            ;
        }
    }
}
int Response::check_forbidden(fd_set &r , fd_set &w)
{
    struct stat s;
    stat(full_path.c_str(),&s);
    if ((access(full_path.c_str(),F_OK) != -1 && access(full_path.c_str(),R_OK) == -1))
    {
        if(send_error("403", " Forbidden "))
        {
            std::cout << RED << "Response 403 Forbidden " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 403 \r\nConnection: close\r\nContent-Length: 73";
            message += "\r\n\r\n<!DOCTYPE html><head><title>Forbidden</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return 0;
        }
        return 0;
    }
    return 1;
}
int Response::handle_autoindex(fd_set &r, fd_set &w)
{
    std::ofstream file;
    if (locations.getAutoIndex() == "on")
    {
        std::string tmp = full_path;
        struct stat s;
        stat(full_path.c_str(),&s);
        if ((access((const char *)full_path.c_str(), F_OK) != -1) && !S_ISDIR(s.st_mode))
            return 1;
        if (S_ISDIR(s.st_mode))
        {
            full_path = "www/autoindex/e.html";
            file.open(full_path);
            file.clear();
            file << generate_autoindex(tmp);
            file.close();
            return 1;
        }
    }
    if (send_error("404", " Not Found "))
    {
            std::cout << RED << "Response 404 Not Found " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 404 \r\nConnection: close\r\nContent-Length: 73";
            message += "\r\n\r\n<!DOCTYPE html><head><title>Not Found</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
    }
    return 0;
}
int Response::check_upload(fd_set &r, fd_set &w)
{
    if (!_server.getUploadPath().empty())
    {
        upload = _server.getUploadPath() + _request.Getrequest().at("Path").substr(last_slash());
        return 1;
    }
    std::cout << RED << "Response 500 Internal Server Error " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
    std::string message = (char *)"HTTP/1.1 500 \r\nConnection: close\r\nContent-Length: 85";
    message += "\r\n\r\n<!DOCTYPE html><head><title>Internal Server Error</title></head><body> </body></html>";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
    if (_request.Getrequest().at("Method") == "POST")
    {
        if (access(_request.get_tmp().c_str(), F_OK) != -1)
            remove(_request.get_tmp().c_str());
    }
    return 0;
}

int Response::last_slash()
{
    int a = 0;
    int i = 0;
    a = i = _request.Getrequest().at("Path").find("/", 0);
    while (i != -1)
    {
        a = i;
        i = _request.Getrequest().at("Path").find("/", i + 1);
    }
    return a;
}

void Response::write_body(fd_set &r, fd_set &w)
{
 
    std::cout << GREEN << "Response 201 Created " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
    rename(_request.get_tmp().c_str(), upload.c_str());
    std::string message = (char *)"HTTP/1.1 201 Created\r\nLocation: ";
    message += upload + "\r\nContent-Length: 0\r\n\r\n";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
}
int Response::isToo_large(fd_set &r, fd_set &w)
{
    if (_request.check_chunked() == 0)
    {
        if (_request.GetLent() > lent_server)
        {
            if (_request.Getrequest().at("Method") == "POST")
            {
                if (access(_request.get_tmp().c_str(), F_OK) != -1)
                    remove(_request.get_tmp().c_str());
            }
            if (send_error("413"," Payload Too Large "))
            {
                std::cout << RED << "Response 413 Payload Too Large " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
                std::string message = (char *)"HTTP/1.1 413 \r\nConnection: close\r\nContent-Length: 81";
                message += "\r\n\r\n<!DOCTYPE html><head><title>Payload Too Large</title></head><body> </body></html>";
                send(_ClientFD, message.c_str(), message.size(), 0);
                FD_CLR(_ClientFD, &w);
                FD_SET(_ClientFD, &r);
                done = 1;
            }
            return 0;
        }
    }
    return 1;
}

int Response::check_lent(fd_set &r, fd_set &w)
{
    if (_request.check_chunked() == 0 && _request.Getrequest().at("Content-Length").empty())
    {
        if (_request.Getrequest().at("Method") == "POST")
        {
            if (access(_request.get_tmp().c_str(), F_OK) != -1)
                remove(_request.get_tmp().c_str());
        }
        if (send_error("411"," Length Required "))
        {
            std::cout << RED << "Response 411 Length Required " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
            std::string message = (char *)"HTTP/1.1 411 \r\nConnection: close\r\nContent-Length: 79";
            message += "\r\n\r\n<!DOCTYPE html><head><title>Length Required</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
        }
        return 0;
    }
    return 1;
}

int Response::check_Content(fd_set &r, fd_set &w)
{
    if (access(full_path.c_str(), F_OK) == -1)
    {
        std::cout << RED << "Response 204 No Content " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
        std::string message = (char *)"HTTP/1.1 204 \r\nConnection: close\r\nContent-Length: 74";
        message += "\r\n\r\n<!DOCTYPE html><head><title>No Content</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        if (_request.Getrequest().at("Method") == "POST")
        {
            if (access(_request.get_tmp().c_str(), F_OK) != -1)
                remove(_request.get_tmp().c_str());
        }
        return 0;
    }
    return 1;
}
void check_all(std::string str, int *a)
{
    struct dirent *d;
    struct stat s;
    stat(str.c_str(), &s);
    if (S_ISDIR(s.st_mode) && (access(str.c_str(), W_OK) == -1 || access(str.c_str(), X_OK) == -1))
    {
        *a = 1;
        return;
    }
    str += "/";
    DIR *dir = opendir(str.c_str());
    while (S_ISDIR(s.st_mode) && (d = readdir(dir)) != NULL)
    {
        std::string str1 = str + d->d_name;
        struct stat s1;
        stat(str1.c_str(), &s1);
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."));
        else
        {
            if (S_ISDIR(s1.st_mode))
                check_all((str + d->d_name), a);
            else if (!S_ISDIR(s1.st_mode))
                if (access(str1.c_str(), W_OK) == -1)
                    *a = 1;
        }
    }
    if (dir)
        closedir(dir);
}
int Response::check_permission(fd_set &r, fd_set &w)
{
    int a = 0;
    struct stat s;
    if (S_ISDIR(s.st_mode) && (access(full_path.c_str(), W_OK) == -1 || access(full_path.c_str(), X_OK)))
        a = 1;
    else if (!S_ISDIR(s.st_mode) && access(full_path.c_str(), W_OK) == -1)
        a = 1;
    else
        check_all(full_path, &a);
    if (a)
    {
        std::cout << RED << "Response 403 Forbidden " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
        std::string message = (char *)"HTTP/1.1 403 \r\nConnection: close\r\nContent-Length: 73";
        message += "\r\n\r\n<!DOCTYPE html><head><title>Forbidden</title></head><body> </body></html>";
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
    stat(str.c_str(), &s);
    str += "/";
    DIR *dir = opendir(str.c_str());
    while (S_ISDIR(s.st_mode) && (d = readdir(dir)) != NULL)
    {
        std::string str1 = str + d->d_name;
        struct stat s1;
        stat(str1.c_str(), &s1);
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."));
        else
        {
            if (S_ISDIR(s1.st_mode))
                delete_file(str + d->d_name);
            else if (!S_ISDIR(s1.st_mode))
                unlink(((const char *)str1.c_str()));
        }
    }
    if (dir)
        closedir(dir);
    remove(str.c_str());
}

void Response::handler_delete(fd_set &r, fd_set &w)
{
    struct stat s;
    stat(full_path.c_str(), &s);
    if (!S_ISDIR(s.st_mode))
        unlink(((const char *)full_path.c_str()));
    else
        delete_file(full_path);
    std::cout << GREEN << "Response 200 OK " << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
    std::string message = (char *)"HTTP/1.1 200\r\nConnection: close\r\nContent-Length: 70";
    message += "\r\n\r\n<!DOCTYPE html><head><title>200 OK</title></head><body> </body></html>";
    send(_ClientFD, message.c_str(), message.size(), 0);
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    done = 1;
}

std::string to_upper(std::string str)
{
    std::string str1;
    for (size_t i = 0; i < str.size(); i++)
        str1 += toupper(str[i]);
    return str1;
}

std::string stringtrim(std::string str)
{
    std::string str1;
    for (size_t i = 0; i < str.size(); i++)
        if (str[i] != ' ')
            str1 += str[i];
    return str1;
}

void Response::load_env(char **env)
{
    std::string str;
    _env.clear();
    for (size_t i = 0; env[i]; i++)
        _env.push_back(env[i]);
}

int Response::send_error(std::string error,std::string m)
{
    for (int i = 0; i < (int)_server.getErrorPages().size(); i++)
    {
        if (_server.getErrorPages().at(i).first == error)
        {
            if (access(_server.getErrorPages().at(i).second.c_str(), F_OK) != -1)
            {
                std::string messge;
                std::cout << RED << "Response " << _server.getErrorPages().at(i).first << m << trim(_request.Getrequest()["Path"]) << " " << trim(_request.Getrequest()["Version"]) << RESET << std::endl;
                char str[10002];
                struct stat st;
                stat(_server.getErrorPages().at(i).second.c_str(), &st);
                fd_error = open(_server.getErrorPages().at(i).second.c_str(), O_RDONLY);
                std::string message = (char *)"HTTP/1.1 " + error + "\r\nConnection: close\r\nContent-Length: " + ft_toString(st.st_size);
                message += "\r\n\r\n";
                send(_ClientFD, message.c_str(), message.size(), 0);
                int len = read(fd_error, str, 10000);
                send(_ClientFD, str, len, 0);
                done = 1;
                return 0;
            }
        }
    }
    return 1;
}

char **create_env(std::vector<std::string> _env)
{
    char **env = new char *[_env.size() + 1];
    for (size_t i = 0; i < _env.size(); i++)
        env[i] = strdup(_env[i].c_str());
    env[_env.size()] = NULL;
    return env;
}

void Response::cgi_exec()
{
    for (size_t i = 0; i < _server._cgi.size(); i++)
    {
        if ((int)_server._cgi[i].second.find(get_extension(full_path)) != -1)
        {
            load_env(_server._env);
            for (std::map<std::string, std::string>::iterator it = _request.Getrequest().begin(); it != _request.Getrequest().end(); it++)
                _env.push_back("HTTP_" + to_upper(it->first) + "=" + stringtrim(it->second));
            char **env = create_env(_env);
            char *argv[] = {
                (char *)_server._cgi[i].first.c_str(),
                (char *)full_path.c_str(),
                NULL};
            unlink("/tmp/cgi_out");
            unlink("/tmp/cgi_out.html");
            pid_t pid = fork();
            int fd;
            if (pid == 0)
            {
                fd = open("/tmp/cgi_out", O_CREAT | O_RDWR, 0666);
                dup2(fd, 1);
                execve(*argv, argv, env);
                close(fd);
            }
            else
                waitpid(pid, 0, 0);
            std::ifstream file("/tmp/cgi_out");
            if (get_extension(full_path) == "php")
            {
                if (file.is_open())
                {
                    std::string line;
                    while (getline(file, line))
                    {
                        std::string key = line.substr(0, line.find(":"));
                        std::string value = line.substr(line.find(":") + 1, line.length());
                        _headers.insert(std::pair<std::string, std::string>(key, value));
                        if (line.length() == 1 && (int)line.find("\r") != -1)
                            break;
                    }
                }
                if (file.eof())
                    file.close();
            }
            std::ofstream myfile("/tmp/cgi_out.html");
            if (myfile.is_open())
            {
                if (!file.is_open())
                    file.open("/tmp/cgi_out");
                std::string line;
                while (getline(file, line))
                {
                    if (line.length() == 1 && (int)line.find("\r") != -1)
                        break;
                    myfile << line;
                }
                myfile.close();
                file.close();
            }
            for (size_t i = 0; env[i]; i++)
                free(env[i]);
            free(env);

            full_path = "/tmp/cgi_out.html";
        }
    }
}

int Response::handler(fd_set &r, fd_set &w)
{
    if (!ok)
        full_path = Path = delete_space((_request.Getrequest().at("Path")));
    if (ok || is_Valide(r, w))
    {
        if (ok || is_Unauthorize(r, w))
        {
            if (ok || check_location(r, w))
            {
                if (ok || handle_redirection(r, w))
                {
                    if (ok || handle_method(r, w))
                    {
                        if (delete_space(_request.Getrequest().at("Method")) == "POST")
                        {
                            if (check_upload(r, w))
                                if (check_lent(r, w))
                                    if (isToo_large(r, w))
                                        write_body(r, w);
                        }
                        else if (delete_space(_request.Getrequest().at("Method")) == "DELETE")
                        {
                            if (check_Content(r, w))
                                if (check_permission(r, w))
                                    handler_delete(r, w);
                        }
                        else if (ok || redirect_path(r, w))
                        {
                            if (ok || check_forbidden(r,w))
                                if(ok || handle_index() ||  handle_autoindex(r, w))
                                    send_data(r, w);
                        }
                    }
                }
            }
        }
    }
    return 1;
}