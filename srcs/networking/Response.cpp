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

std::string Response::get_referer(fd_set &r, fd_set &w)
{
    (void)r;
    (void)w;
    std::string referer = _request.Getrequest().at("Referer");
    return (referer.substr(referer.find_last_of("/")));
    // return ("");
}

Location &Response::define_location_i(int &i_refere, fd_set &r, fd_set &w)
{
    (void)r;
    (void)w;
    (void)i_refere;
    std::string request_path;

    request_path = delete_space(_request.Getrequest().at("Path"));
    // /location/index.html
    // /location
    // substring location from request_path
    int i = 0;
    for (std::vector<Location>::iterator it = _server->getLocations().begin(); it != _server->getLocations().end(); it++, i++)
    {
        if (request_path == it->getLocationPath())
        {
            _server->en_handle = i;
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

// int Response::handle_location(void)
// {
//     Path = delete_space((_request.Getrequest().at("Path")));
//     for (int a = 0; a < (int)_server->getLocations().size(); a++)
//     {
//         if (Path == _server->getLocations().at(a).getLocationPath())
//         {
//             _server->root = _server->getLocations().at(a).getRoot();
//             _server->en_handle = 1;
//             index = _server->getLocations().at(a).getIndex();
//             return 1;
//         }
//     }
//     return 0;
// }

// int Response::handle_index(void)
// {
//     for (int i = 0; i < (int)index.size(); i++)
//     {
//         Path = _server->root + "/" + index.at(i);
//         if (access((const char *)Path.c_str(), F_OK) != -1)
//         {
//             _server->en_handle = 2;
//             return 1;
//         }
//     }
//     return 0;
// }

void Response::send_data(fd_set &r, fd_set &w)
{
    struct stat st;
    Location _location;
    try
    {
        std::cout << "Try" << std::endl;
        _location = define_location_i(_refere, r, w);
        std::cout << "location : " << _location.getLocationPath() << std::endl;
        Path = _location.getRoot() + delete_space((_request.Getrequest().at("Path")));
        std::string index = check_index(_location);
        if (index != "")
            Path = index;
    }
    catch (const std::exception &e)
    {
        std::cout << _server->en_handle << std::endl;
        std::cout << "Catch" << std::endl;
        Path = _server->getLocations().at(_server->en_handle).getRoot() + delete_space((_request.Getrequest().at("Path")));
    }
    std::cout << "Path : " << Path << std::endl;

    if (is_Valide(r, w))
    {
        if (is_Unauthorize(r, w))
        {
            // if (_server->en_handle || handle_location())
            // {
            // if (_server->en_handle >= 2 || handle_index())
            // {
            // if (_server->en_handle == 3)
            // {
            //     Path = _server->root + Path;
            // }
            if (access((const char *)Path.c_str(), F_OK) != -1)
            {
                // _server->en_handle = 3;
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
    else
    {
        //response 404
        std::string header;
        header = (char *)"HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-type: text/html\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
        write(_ClientFD, header.c_str(), header.size());
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
    }
    //     }
    // }
}