#include "../../includes/Webserv.hpp"

Location::Location()
{
}

Location::~Location()
{
}

std::pair<std::string, std::string> &Location::getRedirection()
{
    return _redirection;
}

std::string &Location::getLocationNumber()
{
    return _locationNumber;
}

std::string &Location::getLocationPath()
{
    return _locationPath;
}

std::vector<std::string> &Location::getAllowedMethods()
{
    return _allowedMethods;
}

std::string &Location::getRoot()
{
    return _root;
}

std::string &Location::getClientMaxBodySize()
{
    return _clientMaxBodySize;
}

std::vector<std::string> &Location::getIndex()
{
    return _index;
}

std::string &Location::getAutoIndex()
{
    return _autoIndex;
}

std::vector<std::pair<std::string, std::string> > &Location::getErrorPages()
{
    return _error_pages;
}

std::string &Location::getUploadPath()
{
    return _uploadPath;
}

// seters
void Location::setRedirection(std::string pathToRedirect, std::string redirectToPath)
{
    _redirection.first = pathToRedirect;
    _redirection.second = redirectToPath;
}

void Location::setLocationNumber(std::string locationNumber)
{
    _locationNumber = locationNumber;
}

void Location::setLocationPath(std::string locationPath)
{
    _locationPath = locationPath;
}

void Location::setAllowedMethods(std::vector<std::string> allowedMethods)
{
    _allowedMethods = allowedMethods;
}

void Location::setRoot(std::string root)
{
    _root = root;
}

void Location::setClientMaxBodySize(std::string clientMaxBodySize)
{
    _clientMaxBodySize = clientMaxBodySize;
}

void Location::setIndex(std::vector<std::string> index)
{
    _index = index;
}

void Location::setAutoIndex(std::string autoIndex)
{
    _autoIndex = autoIndex;
}

void Location::setErrorPages(std::vector<std::pair<std::string, std::string> > error_pages)
{
    _error_pages = error_pages;
}

void Location::setUploadPath(std::string uploadPath)
{
    _uploadPath = uploadPath;
}
