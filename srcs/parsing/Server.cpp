#include "../../includes/Webserv.hpp"

Server::Server()
{
    _port = 80;
    _root = "";
    _autoIndex = "off";
    _clientMaxBodySize = "1";
    _uploadPath = "./ var/www/uploads";   
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

std::string & Server::getRoot()
{
    return _root;
}

std::string & Server::getAutoIndex()
{
    return _autoIndex;
}

std::string & Server::getClientMaxBodySize()
{
    return _clientMaxBodySize;
}

std::vector<std::string> & Server::getAllowedMethods()
{
    return _allowedMethods;
}

std::vector<Location> & Server::getLocations()
{
    return _locations;
}

std::vector<std::pair<std::string, std::string> > & Server::getRedirections()
{
    return _redirections;
}

std::vector<std::pair<std::string, std::string> > & Server::getErrorPages()
{
    return _error_pages;
}

std::string & Server::getUploadPath()
{
    return _uploadPath;
}

std::vector<std::string>  & Server::getIndex(){
    return _index;
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

void Server::setRoot(std::string root)
{
    _root = root;
}

void Server::setAutoIndex(std::string autoIndex)
{
    _autoIndex = autoIndex;
}

void Server::setClientMaxBodySize(std::string clientMaxBodySize)
{
    _clientMaxBodySize = clientMaxBodySize;
}

void Server::setAllowedMethods(std::vector<std::string> allowedMethods)
{
    _allowedMethods = allowedMethods;
}

void Server::setLocations(std::vector<Location> locations)
{
    _locations = locations;
}

void Server::setRedirections(std::vector<std::pair<std::string, std::string> > redirections)
{
    _redirections = redirections;
}

void Server::setErrorPages(std::vector<std::pair<std::string, std::string> > error_pages)
{
    _error_pages = error_pages;
}

void Server::setUploadPath(std::string uploadPath)
{
    _uploadPath = uploadPath;
}

void Server::setIndex(std::vector<std::string> index){
    _index = index;
}