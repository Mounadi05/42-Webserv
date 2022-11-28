#include "../../includes/Webserv.hpp"

Response::Response()
{
}

Response::Response(Request request, Server *server, int ClientFD)
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
    _refere = 0;
    _rederict = 0;
    // index = -1;
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

Server *Response::getServer()
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
    for (int i = 0; i < (int)_server->getmime_types().size(); i++)
        if ((int)_server->getmime_types()[i].find(tmp, 0) != -1)
            return _server->getmime_types()[i].substr(0, _server->getmime_types()[i].find("|", 0));
    return path;
}

int Response::handler(fd_set &r, fd_set &w)
{
    send_data(r, w);
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

// int Response::handle_location(void)
// {
//     Path = delete_space((_request.Getrequest().at("Path")));
//     for (int a = 0; a < (int)_server.getLocations().size(); a++)
//     {
//         if (Path == _server.getLocations().at(a).getLocationPath())
//         {
//             _server.root = _server.getLocations().at(a).getRoot();
//             _server.en_handle = 1;
//             index = _server.getLocations().at(a).getIndex();
//             return 1;
//         }
//     }
//     return 0;
// }

// int Response::handle_index(void)
// {
//     for (int i = 0; i < (int)index.size(); i++)
//     {
//         Path = _server.root + "/" + index.at(i);
//         if (access((const char *)Path.c_str(), F_OK) != -1)
//         {
//             _server.en_handle = 2;
//             return 1;
//         }
//     }
//     return 0;
// }

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

// std::string Response::getLocationRefere()
// {
//     std::string result = _request.Getrequest().at("Referer");
//     std::cout << "result: " << result << std::endl;
//     if (result != "")
//         return (result.substr(result.find_last_of("/")));
//     return "";
// }

Location &Response::define_location(std::string location_path)
{
    for (std::vector<Location>::iterator it = _server->getLocations().begin(); it != _server->getLocations().end(); it++)
    {
        if (location_path.find(it->getLocationPath()) != std::string::npos)
        {
            // _server->en_handle = 0;
            return *it;
        }
    }

    throw std::runtime_error("Location not found");
    return (_server->getLocations().at(0));
}

std::string Response::check_index(Location &_location)
{
    for (size_t i = 0; i < _location.getIndex().size(); i++)
    {
        std::string file =
            ((_location.getRoot()[_location.getRoot().length() - 1] != '\\') ? _location.getRoot() + "/" : _location.getRoot()) + _location.getIndex().at(i);
        if (access(file.c_str(), F_OK) != -1)
            return (file);
    }
    return ("");
}

std::string parse_location(std::string path)
{
    std::cout << "path: " << path << std::endl;
    std::string result = path.substr(0, path.find_first_of("/", 1));
    std::cout << "result: " << result << std::endl;
    return (result);
}

std::string Response::grepLocation(std::string path, std::vector<Location> locations)
{
    std::cout << "path: " << path << std::endl;
    std::string result = "";
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (path.find(it->getLocationPath()) != std::string::npos)
        {
            if (path[it->getLocationPath().length()] && path[it->getLocationPath().length()] == '/')
                result = path.substr(0, it->getLocationPath().length() + 1);
            else
                result = path.substr(0, it->getLocationPath().length());
            std::cout << "result: " << result << std::endl;
            break;
        }
    }
    if (result == "")
        throw std::runtime_error("Location not found");
    if (result[result.length() - 1] != '/')
    {
        _rederict = 1;
        return (result);
        std::cout << "redirect to ----> " + result + "/" << std::endl;
    }
    if (result == path)
        _rederict = 2;
    return (result);
}

std::string Response::location_handler()
{
    Location _location;
    std::string location_path = grepLocation(delete_space((_request.Getrequest().at("Path")))
        , _server->getLocations());
    // std::cout << "location_path: " << location_path << std::endl;
    _location = define_location(location_path);
    if (_rederict == 1)
        return (location_path);
    else if (_rederict == 2)
        return (check_index(_location));
    else if (_rederict == 0)
        return (((_location.getRoot()[_location.getRoot().length() - 1] != '\\') ? _location.getRoot() + "/" : _location.getRoot()) 
        + delete_space((_request.Getrequest().at("Path"))).substr(location_path.length()));
    return ("zabi");
}

void Response::send_data(fd_set &r, fd_set &w)
{
    struct stat st;
    std::string re = location_handler();
    std::cout << "_rederict : " << _rederict << std::endl;
    if (_rederict == 1)
    {
        std::cout << "redirect to ----> " + re << std::endl;
        std::string message = (char *)"HTTP/1.1 301 Moved Permanently\r\nLocation: ";
        message += _request.Getrequest().at("Path") + "/";
        message += "\r\nContent-Length: 0\r\n\r\n";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return;
    }
    else
        Path = re;
    std::cout << "Path: " << Path << std::endl;
    if (is_Valide(r, w))
    {
        if (is_Unauthorize(r, w))
        {
            if (access((const char *)Path.c_str(), F_OK) != -1)
            {
                if (!finish)
                {
                    stat((const char *)Path.c_str(), &st);
                    size = st.st_size;
                    fd = open(Path.c_str(), O_RDONLY);
                    bzero(str, 1025);
                    std::string header;
                    header = (char *)"HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + delete_space(get_type(Path)) + "\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
                    write(_ClientFD, header.c_str(), header.size());
                    finish = 10;
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
                }
            }
        }
    }
}