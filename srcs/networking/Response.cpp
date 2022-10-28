#include "../../includes/Webserv.hpp"

Response::Response()
{
}

Response::Response(Request request, Server server, int ClientFD)
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
    return path;
}
int flag  = 0;
int Response::handler(fd_set &r, fd_set &w)
{
    std::string path = delete_space((_request.Getrequest().at("Path")));
    std::cout << path << std::endl;
    

    // before modifiying this process lets talk
    if (is_Valide(r, w) == 0)
        return -1;
    if (is_unsupportedVersion(r, w) == 0)
        return -1;
    
    // bad request was cheked
    // supported http version was cheked

    // if need to implement URI restricted characters or if the uri is too long it goes here
        // if done
    
    // better to separate resource requested from query parameters
        // substr pathtosearch from 0 to find('?')
    std::string pathtosearch;
    std::string querys;
    if (path.find("?") != std::string::npos)
    {
        pathtosearch = path.substr(0, path.find("?"));
        querys = path.erase(0, path.find("?") + 1);
    }
    else
        pathtosearch = path;

    // lets define the location block who will handle the resource.
    int locationIndex = defineLocation(_server.getLocations(), pathtosearch);
    
    // lets define the full path depending on the directive root
    std::string fullPath = setFullPath(_server, pathtosearch, locationIndex);
    
    // lets define the method if allowed depending on the directive allow_methods
    if (isAllowedMethod(_server, _server.getLocations()[locationIndex], _request.Getrequest().at("Method")) == 0)
    {
        std::string message = (char *)"HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 82\r\n\r\n<!DOCTYPE html><head><title>Method Not Allowed</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return -1;
    }
    if (isPayloadTooLarge(_server, _server.getLocations()[locationIndex], std::stoi(_request.Getrequest().at("Content-Length"))) == 0)
    {
        std::string message = (char *)"HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 81\r\n\r\n<!DOCTYPE html><head><title>Payload Too Large</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return -1;
    }

        // not finished yet need to imlement redirection directive in config file
    // if (shouldRedirectUrl(pathtosearch) == 1)
    // {
    //     std::cout << "your URL should redirect to : " << _server.getLocations()[locationIndex].getRedirection().second << std::endl;

    //     std::string newPath = _server.getLocations()[locationIndex].getRedirection().second;

    // }



    // lets define access permission to the resource if forbidden or not    
    
    // lets define tthe type of the resource

    // lets define if autoindex is on or off

    // lets define if we should redirect the resource
    

    // POST
    // if (_request.Getrequest().at("Method").compare("POST") == 0)
    // {
    //     // if post request is successfull 200 OK should be returned
    //     if (success) // this is just an alias for the checked im still trying to figure out
    //     {
    //         std::string message = (char *)"HTTP/1.1 200 \r\nConnection: close\r\nContent-Length: 86\r\n\r\n<!DOCTYPE html><head><title>OK</title></head><body>Successfully Uploaded</body></html>";
    //         send(_ClientFD, message.c_str(), message.size(), 0);
    //         FD_CLR(_ClientFD, &w);
    //         FD_SET(_ClientFD, &r);
    //         done = 1;
    //     }
    //     // else its an internal server error ??
    //     else
    //     {
    //         std::string message = (char *)"HTTP/1.1 500 \r\nConnection: close\r\nContent-Length: 85\r\n\r\n<!DOCTYPE html><head><title>Internal Server Error</title></head><body> </body></html>";
    //         send(_ClientFD, message.c_str(), message.size(), 0);
    //         FD_CLR(_ClientFD, &w);
    //         FD_SET(_ClientFD, &r);
    //         done = 1;
    //         return -1;
    //     }
    // }
    
    // DELETE
    // we need to verify the pathtosearch give to deletRequestFunction it should be correct
    if (_request.Getrequest().at("Method").compare("DELETE") == 0)
    {
        if (deleteRequest(pathtosearch) == 1) // when it succed to delete the resource it returns 1
        {
            std::string message = (char *)"HTTP/1.1 204 \r\nConnection: close\r\nContent-Length: 74\r\n\r\n<!DOCTYPE html><head><title>No Content</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
        }
        else
        {
            std::string message = (char *)"HTTP/1.1 404 \r\nConnection: close\r\nContent-Length: 73\r\n\r\n<!DOCTYPE html><head><title>Not Found</title></head><body> </body></html>";
            send(_ClientFD, message.c_str(), message.size(), 0);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return -1;
        }
    }
    return 1;
}

int	Response::defineLocation(std::vector<Location> location, std::string uriPath)
{
	std::string locationMatch;
	int			indexMatch = -1;

	for (size_t i = 0; i < location.size(); i++)
	{
		locationMatch = uriPath;
		if (location[i].getLocationPath() == uriPath)
			return i;
		while (locationMatch.length() != 0)
		{
			if (locationMatch.find_last_of("/") == locationMatch.npos)
				break;
			if (locationMatch.find_last_of("/") + 1 != locationMatch.size())
				locationMatch.erase(locationMatch.find_last_of("/") + 1);
			else
				locationMatch.erase(locationMatch.find_last_of("/"));
			if (locationMatch == location[i].getLocationPath())
				break;
		}
        if (locationMatch.length() != 0)
            indexMatch = i;
    }
	return indexMatch;
}

std::string Response::setFullPath(Server server, std::string uriPath, int locationIndex)
{
    std::string fullPath;
    std::string rootPath;

    if (server.getLocations()[locationIndex].getLocationPath().empty() == false)
        rootPath = server.getLocations()[locationIndex].getRoot();
    else
        rootPath = server.getRoot();
    fullPath = rootPath + uriPath.erase(0, server.getLocations()[locationIndex].getLocationPath().size());
    return fullPath;
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

int Response::is_unsupportedVersion(fd_set &r, fd_set &w)
{
    std::string Version = _request.Getrequest().at("Version");
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

int Response::isAllowedMethod(Server server, Location locationBlock, std::string requestedMethod)
{
    std::vector<std::string> blockMethods = locationBlock.getAllowedMethods();
    std::vector<std::string> serverMethods = server.getAllowedMethods();

    if (blockMethods.size() != 0)
    {
        for (size_t i = 0; i < blockMethods.size(); i++)
        {
            if (blockMethods[i] == requestedMethod)
                return 1;
        }
        return 0;
    }
    else
    {
        if (serverMethods.size() != 0)
        {
            for (size_t i = 0; i < serverMethods.size(); i++)
            {
                if (serverMethods[i] == requestedMethod)
                    return 1;
            }
            return 0;
        }
    }
    return 0;
}

int Response::isPayloadTooLarge(Server server, Location locationBlock, int contentLengthRequested)
{
    std::string blockMaxBodySize = locationBlock.getClientMaxBodySize();
    std::string serverMaxBodySize = server.getClientMaxBodySize();
    if (blockMaxBodySize.length() != 0)
    {
        if ( contentLengthRequested > std::stoi(blockMaxBodySize))
            return 1;
        else
            return 0;
    }
    else
    {
        if (serverMaxBodySize.length() != 0)
        {
            if ( contentLengthRequested > std::stoi(server.getClientMaxBodySize()))
                return 1;
            else
                return 0;
        }
    }
    return 0;
}

int Response::defineFileType(std::string pathToResource) // stackOverFlow Thank you
{
    struct stat s;
    if (stat(pathToResource.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            // a directory
            return 1; 
        }
        else if (s.st_mode & S_IFREG)
        {
            // a regular file
            return 0;
        }
    }
    return -1;
}

int                     Response::deleteDir(std::string pathToDir)
{
    DIR                 *chosenDirToDelete;
    struct dirent       *direntElements;
    struct stat         stat;

    if ((chosenDirToDelete = opendir(pathToDir.c_str())) == NULL) {
        std::cout << "error oppening directory" << std::endl;
        return -1;
    }
    while ((direntElements = readdir(chosenDirToDelete)))
    {
        if (!strcmp(direntElements->d_name, ".") ||
        !strcmp(direntElements->d_name, ".."))
             continue;
        std::string new_path(pathToDir);
        new_path.push_back('/');
        new_path.insert(new_path.size(), direntElements->d_name);
        if (!lstat(new_path.c_str(), &stat)) {
            if (S_ISREG(stat.st_mode))
            {
                if (unlink(new_path.c_str()) == -1) {
                    std::cout << "error unlinking a file" << std::endl;
                    return -1;
                }

            }
            else if (S_ISDIR(stat.st_mode)) {
                if (deleteDir(new_path.c_str()) == -1)
                {
                    std::cout << "error thrown by deleteDir going recursively" << std::endl;
                    return -1;
                }
            }
        }
    }
    if (closedir(chosenDirToDelete) == -1) {
        std::cout << "error closing directory" << std::endl;
        return -1;
    }
    if (rmdir(pathToDir.c_str()) == -1) {
        std::cout << "error removing empty directory" << std::endl;
        return -1;
    }
    return (0);
}

int Response::deleteRequest(std::string pathToDelete)
{
    int fileType = defineFileType(pathToDelete);
    if (fileType == 0) // 0 for a file type
    {
        if (std::remove(pathToDelete.c_str()) == 0)
        {
            std::cout << "File Successfully deleted" << std::endl;
            return 1;
        }
        else
        {
            std::cout << "File Not deleted" << std::endl;
            return -1;
        }
    }
    else if (fileType == 1) // 1 for a directory type
    {
        if (deleteDir(pathToDelete) == 0)
        {
             std::cout << "Directory Successfully deleted" << std::endl;
             return 1;
        }
        else 
        {
             std::cout << "Directory not deleted" << std::endl;
             return -1;
        }
    }
    //should handle this case properly
    std::cout << "path not found" << std::endl;
    return -1;
}

// not yet finished , should define if ressource is accessible if it is a directory
int Response::isForbiddenResource(std::string resource)
{
    int fileType = defineFileType(resource);
    if (fileType == 0) // resource is a file
    {
        // check with access function
        if (access(resource.c_str(), R_OK) == -1) // check for read permission it will throw an error eventually if file doesnt exist
            return 1;
        return 0;
    }
    else if (fileType == 1 && _request.Getrequest().at("Method").compare("GET") == 0) // resource is a directory
    {
        ;
    }
    return 0; // should be always forbidden unless permission's are cheked !!? or not
}

int Response::shouldListContent(Server server, int locationIndex)
{
    if (server.getLocations()[locationIndex].getAutoIndex().empty() == false)
    {
        if (server.getLocations()[locationIndex].getAutoIndex().compare("on") == 0)
            return 1;
        return 0;
    }
    else if (server.getAutoIndex().empty() == false)
    {
        if (server.getAutoIndex().compare("on") == 0) // depends on how it was written in the config file capitalize or not
            return 1;
        return 0;
    }
    return 0;
}

int Response::shouldRedirectUrl(Location locationBlock, std::string pathtosearch)
{
    if(locationBlock.getRedirection().first.size() != 0)
    {
        if (locationBlock.getRedirection().first.compare(pathtosearch) == 0)
            return 1;
        return 0;
    }
    return 0;
}


