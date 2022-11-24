#include "../../includes/Webserv.hpp"

Server::Server()
{
    std::cout << "GHHhelldfljgdsfg" << std::endl;
    _port = 80;
    _root = "";
    _autoIndex = "off";
    _clientMaxBodySize = "1";
    _uploadPath = "./var/www/uploads";
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
    _cgi = cgi;
}

std::pair<std::string,std::string> &Server::getCgi(void)
{
    return _cgi;
}