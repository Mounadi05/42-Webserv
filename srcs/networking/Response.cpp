#include "../../includes/Webserv.hpp"

Response::Response()
{
}
std::string uri_decode(std::string &src)
{
  std::string ret;
  char ch;
  int i, ii;
  for (i = 0; i <(int)src.length(); i++)
  {

    if (int(src[i]) == '%')
    {
      sscanf(src.substr(i + 1, 2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      ret += ch;
      i = i + 2;
    }
    else
    {
      ret += src[i];
    }
  }

  return (ret);
}

Response::Response(Request request, Server *server, int ClientFD)
{
    _request = request;
    _request.Getrequest().at("Path") = uri_decode(_request.Getrequest().at("Path"));
    _server = server;
    _ClientFD = ClientFD;
    str = new char[1025];
    lent = 0;
    finish = 0;
    lent_re = 0;
    size = 0;
    _send = 0;
    done = 0;
    // _refere = 0;
    _rederict = 0;
    _location_index = -1;
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

Location &Response::define_location(std::string location_path)
{
    for (std::vector<Location>::iterator it = _server->getLocations().begin(); it != _server->getLocations().end(); it++)
    {
        if (location_path.find(it->getLocationPath()) != std::string::npos)
        {
            _location_index = (it  - _server->getLocations().begin());
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

std::string strtim(std::string str)
{
    int i = 0;
    while (str[i] == ' ')
        i++;
    str = str.substr(i, str.size() - i);
    i = str.size() - 1;
    while (str[i] == ' ')
        i--;
    str = str.substr(0, i + 1);
    return str;
}

std::string Response::grepLocation(std::string path, std::vector<Location> locations)
{
    std::string result = "";
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (path.find(it->getLocationPath()) != std::string::npos)
        {
            if (path[it->getLocationPath().length()] && path[it->getLocationPath().length()] == '/')
                result = path.substr(0, it->getLocationPath().length() + 1);
            else
                result = path.substr(0, it->getLocationPath().length());
            break;
        }
    }
    if (result == "")
        throw std::runtime_error("Location not found");
    if (result[result.length() - 1] != '/')
    {
        _rederict = 1;
        return (result);
    }
    if (result == path)
        _rederict = 2;
    return (result);
}

std::string Response::location_handler()
{
    Location _location;
    std::string location_path = grepLocation(strtim((_request.Getrequest().at("Path"))), _server->getLocations());
    _location = define_location(location_path);
    if (_rederict == 1)
        return (location_path);
    else if (_rederict == 2) // autoinddex on check
        return (check_index(_location));
    else
    {
        return (((_location.getRoot()[_location.getRoot().length() - 1] != '\\') ? _location.getRoot() + "/" : _location.getRoot()) + strtim((_request.Getrequest().at("Path"))).substr(location_path.length()));
    }
}

std::string generate_autoindex(std::string path, std::string r_path);

int Response::isNotAllowedMethod()
{
    if (_location_index != -1)
    {
        Location locationBlock = _server->getLocations()[_location_index];
        if (locationBlock.getAllowedMethods().size() != 0)
        {
            for (size_t i = 0; i < locationBlock.getAllowedMethods().size(); i++)
            {
                if (_request.Getrequest().at("Method").compare(locationBlock.getAllowedMethods()[i]) == 0)
                    return 0;
            }
        }
        return 1;
    }
    return 1;
}

int Response::httpVersionNotSupported()
{
    if (_request.Getrequest().at("Version").compare("HTTP/1.0") != 0 && _request.Getrequest().at("Version").compare("HTTP/1.1") != 0)
        return 1;
    else
        return 0;
}

int Response::isPayloadTooLarge(struct stat *st)
{
    stat((const char *)Path.c_str(), st);
    size = (*st).st_size;
    if (_location_index != -1)
    {
        Location locationBlock = _server->getLocations()[_location_index];
        if (locationBlock.getClientMaxBodySize().size() != 0)
        {
            std::cout << "Sizeof Requested File : " << size << "\n" << "ClientMaxBodySize : " << std::stoi(locationBlock.getClientMaxBodySize()) << std::endl;
            if (size > std::stoi(locationBlock.getClientMaxBodySize()))
                return 1;
        }
        return 0;
    }
    return 0;
}

int Response::shouldListDirectoryContent()
{
    if (_location_index != -1)
    {
        Location locationBlock = _server->getLocations()[_location_index];
        if (locationBlock.getAutoIndex().size() != 0)
        {
            if (locationBlock.getAutoIndex().compare("on") == 0)
                return 1;
        }
    }
    return 0;
}

void Response::send_data(fd_set &r, fd_set &w)
{
    struct stat st;
    try
    {
        std::string re = location_handler();
        if (_rederict == 1)
        {
            std::string message = (char *)"HTTP/1.1 302 Found\r\nLocation: ";
            message += strtim(_request.Getrequest().at("Path")) + "/";
            message += "\r\nContent-Length: 0\r\n\r\n";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return;
        }
        else
            Path = re;
        if (stat(Path.c_str(), &st) == -1)
        {
            std::string message = (char *)"HTTP/1.1 404 \r\nConnection: close\r\nContent-Length: 73\r\n\r\n<!DOCTYPE html><head><title>Not Found</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return;
        }
        if (S_ISDIR(st.st_mode))
        {
            if (Path[Path.length() - 1] != '/')
            {
                std::string message = (char *)"HTTP/1.1 302 Found\r\nLocation: ";
                message += strtim(_request.Getrequest().at("Path")) + "/";
                message += "\r\nContent-Length: 0\r\n\r\n";
                send(_ClientFD, message.c_str(), message.size(), 0);
                FD_CLR(_ClientFD, &w);
                FD_SET(_ClientFD, &r);
                done = 1;
                return;
            }
            std::string resp = generate_autoindex(Path, strtim((_request.Getrequest().at("Path"))));
            std::string message = (char *)"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: ";
            message += std::to_string(resp.size());
            message += "\r\n\r\n";
            message += resp;
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    if (is_Valide(r, w))
    {
        if (is_Unauthorize(r, w))
        {
            if (access((const char *)Path.c_str(), F_OK) != -1)
            {
                if (isNotAllowedMethod() == 1)
                {
                    stat("./error/notAllowed.html", &st);
                    size = st.st_size;
                    fd = open("./error/notAllowed.html", O_RDONLY);
                    bzero(str, 1025);
                    std::string header;
                    header = (char *)"HTTP/1.1 405 NotAllowed\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + get_type("./error/notAllowed.html") + "\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
                    write(_ClientFD, header.c_str(), header.size());
                    finish = 10;
                }
                if (httpVersionNotSupported() == 1)
                {
                    stat("./error/httpVersionNotSupportted.html", &st);
                    size = st.st_size;
                    fd = open("./error/httpVersionNotSupportted.html", O_RDONLY);
                    bzero(str, 1025);
                    std::string header;
                    header = (char *)"HTTP/1.1 505 VersionNotSupportted\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + get_type("./error/httpVersionNotSupportted.html") + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
                    write(_ClientFD, header.c_str(), header.size());
                    finish = 10;
                }
                if (isPayloadTooLarge(&st) == 1)
                {
                    stat("./error/payloadTooLarge.html", &st);
                    size = st.st_size;
                    fd = open("./error/payloadTooLarge.html", O_RDONLY);
                    bzero(str, 1025);
                    std::string header;
                    header = (char *)"HTTP/1.1 413 PayloadTooLarge\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + get_type("./error/payloadTooLarge.html") + "\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\n\r\n";
                    write(_ClientFD, header.c_str(), header.size());
                    finish = 10;
                }
                if (shouldListDirectoryContent() == 1)
                {
                    if (S_ISDIR(st.st_mode))
                    {
                        std::string resp = generate_autoindex(Path.substr(0, Path.find_last_of("/")), strtim((_request.Getrequest().at("Path"))));
                        std::string message = (char *)"HTTP/1.1 200 OK\r\nConnection: " + delete_space(_request.Getrequest().at("Connection")) + "\r\nContent-Length: ";
                        message += std::to_string(resp.size());
                        message += "\r\n\r\n";
                        message += resp;
                        send(_ClientFD, message.c_str(), message.size(), 0);
                        done = 1;
                        finish = 10;
                    }
                }
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