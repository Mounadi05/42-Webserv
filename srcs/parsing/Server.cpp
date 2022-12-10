#include "../../includes/Webserv.hpp"

Server::Server()
{
     _port = 80;
    en_handle = 0;
    a = 9000;
    init_MimeTypes();
}

Server::~Server()
{
}

Socket & Server::getSocket()
{
    return _socket;
}


size_t & Server::getPort()
{
    return _port;
}

std::vector<std::string> & Server::getServerNames()
{
    return _serverNames;
}

std::vector<Location> & Server::getLocations()
{
    return _locations;
}

std::vector<std::pair<std::string, std::string> > & Server::getErrorPages()
{
    return _error_pages;
}

std::string & Server::getUploadPath()
{
    return _uploadPath;
}

void Server::setSocket(Socket socket)
{
    _socket = socket;
}

void Server::setPort(size_t port)
{
    _port = port;
}

void Server::setServerNames(std::vector<std::string> serverNames)
{
    _serverNames = serverNames;
}

void Server::setLocations(std::vector<Location> locations)
{
    _locations = locations;
}

void Server::setErrorPages(std::vector<std::pair<std::string, std::string> > error_pages)
{
    _error_pages = error_pages;
}

void Server::setUploadPath(std::string uploadPath)
{
    _uploadPath = uploadPath;
}

std::vector<std::string> &Server::getmime_types(void)
{
    return mime_types;
}
void Server::init_MimeTypes(void)
{
    std::ifstream file;
    file.open("srcs/mime.types");
    std::string str;
    while(getline(file,str))
        mime_types.push_back(str);
    file.close();
}

void Server::setCgi(std::pair<std::string,std::string> cgi)
{
    _cgi.push_back(cgi);
}

 