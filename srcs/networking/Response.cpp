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

std::string Response::extractQueryParams(std::string path)
{
    std::string pathtosearch; 
    if (path.find("?") != std::string::npos)
    {
        pathtosearch = path.substr(0, path.find("?"));
        this->queryParams = path.erase(0, path.find("?") + 1);
    }
    else
    {
        this->queryParams = "";
        pathtosearch = path;
    }
    return pathtosearch;
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
    (void)r;
    (void)w;  
    std::string path = delete_space((_request.Getrequest().at("Path")));
    std::cout << path << std::endl;

    std::string pathtosearch = extractQueryParams(path);
    // lets define the location block who will handle the resource.
    int locationIndex = defineLocation(_server.getLocations(), pathtosearch);
        // if no location block is found i will be handle by  the  location /
      

    // before modifiying this process lets talk
    if (isBadRequest() == 0)
    {
        this->_statusCode = 400;
        craftErrorPage("Bad Request", 400);
        return (sendResponse());
    }
    if (is_notImplemented() == 0)
    {
        this->_statusCode = 501;
        craftErrorPage("Method Not Implemented", 501);
        return (sendResponse());
    }
    if (is_unsupportedVersion() == 0)
    {
        this->_statusCode = 505;
        craftErrorPage("HTTP version unsupported", 505);
        return(sendResponse());
    } 
    std::string fullPath = setFullPath(_server, pathtosearch, locationIndex);

    if (defineFileType(fullPath) == 1)
    {
        if (fullPath[fullPath.size() - 1] != '/')
            fullPath.append("/");
    }
    std::cout << "full path constructed : " << fullPath <<std::endl;
    if (isAllowedMethod(_server, _server.getLocations()[locationIndex], _request.Getrequest().at("Method")) == 0)
    {
        this->_statusCode = 405;
        craftErrorPage("Method Not Allowed", 405);
        return(sendResponse());
    }
    
    if (shouldRedirectUrl(_server.getLocations()[locationIndex], pathtosearch) == 1)
    {
        std::cout << "your URL should redirect to : " << _server.getLocations()[locationIndex].getRedirection().second << std::endl;

        std::string newPath = _server.getLocations()[locationIndex].getRedirection().second;
        this->_statusCode = 301;
    }
    int status;
    if ((status = isForbiddenResource(path, locationIndex)) >= 0)
    {
        if (status == 0)
        {
            int found = 0;
            // i should append index
            for (size_t i = 0; i < _server.getLocations()[locationIndex].getIndex().size(); i++)
            {
                if (access(path.c_str(), R_OK) == 0)
                {
                    path.append(_server.getLocations()[locationIndex].getIndex()[i]);
                    found = 1;
                    break;
                }
            }
            if (found == 0)
            {
                this->_statusCode = 404;
                craftErrorPage("Not Found", 404);
                return(sendResponse());
            }
        }
        if (status == 1)
        {
            this->_statusCode = 403;
            craftErrorPage("Forbidden", 403);
            return(sendResponse());
        }
        if (status == 2)
        {
            // i should list content of DIR
            ;
        }
    }
    

    // if (isPayloadTooLarge(_server, _server.getLocations()[locationIndex], we need the value of the content length) == 0)
    // {
    //     this->_statusCode = 413;
    //     craftErrorPage("Payload Too Large", 413);
    //     return(sendResponse());
    // }

    // lets define access permission to the resource if forbidden or not    
    
    // lets define if autoindex is on or off

    // lets define if we should redirect the resource
    

    //POST
    // if (_request.Getrequest().at("Method").compare("POST") == 0)
    // {
    //     // if post request is successfull 200 OK should be returned
    //     if (success) // this is just an alias for the checked im still trying to figure out
    //     {
    //         this->_statusCode = 200;
    //         craftResponse("./responsePages/ok.html", "OK", 200);
    //         return(sendResponse());
    //     }    
    //     else
    //     {
    //         this->_statusCode = 500;  
    //         craftErrorPage("Internal Server Error", 500);
    //         return(sendResponse());   
    //     }
    // }
    
    // DELETE
    // we need to verify the pathtosearch give to deletRequestFunction it should be correct
    // if (_request.Getrequest().at("Method").compare("DELETE") == 0)
    // {
    //     if (deleteRequest(pathtosearch) == 1) // when it succed to delete the resource it returns 1
    //     {
    //         this->_statusCode = 204;
    //         craftResponse("./responsePages/noContent.html", "No Content", 204);
    //         return(sendResponse());
    //     }
    //     else
    //     {
    //         this->_statusCode = 404;
    //         craftErrorPage("Not Found", 404);
    //         return(sendResponse());
    //     }
    // }
    std::cout << "i am hereeeeee" << std::endl;
    return 1;
}

int Response::sendResponse()
{
    return 0;
}
// std::string Response::defineMimeType(std::vector<std::string> mimeTypes, std::string path)
// {
//     for (size_t i = 0; i < mimeTypes.size())
// }

void Response::craftResponse(std::string path, std::string msg, size_t statusCode, bool isError)
{
    (void)isError;
    
    std::fstream _responseFile;
    std::vector<std::string> mimeTypes = _server.getmime_types();
    //std::string type = defineMimeType(mimeTypes, path);
    int sizeOfFile = (int)getSizeOfFile(path);
    
    _responseFile.open("./tmp/responseFile.txt", std::ios::out);
 
    // set first line
    _responseFile << "HTTP/1.1 ";
    _responseFile << statusCode;
    _responseFile << " " + msg;
    _responseFile << "\r\n";
    
    //set content type
    _responseFile << "Content-Type: ";
    _responseFile << "text/html"; // should be the return of defineMimeType()
    _responseFile << "\r\n";
    
    // set content length
    _responseFile << "Content-Length: ";
    _responseFile << sizeOfFile;
    _responseFile << "\r\n";
    
    // set body
    std::string     line;
    std::ifstream   readFromFile(path);

    if (!readFromFile.is_open())
            return ;
    _responseFile << "\r\n";

    while (std::getline(readFromFile, line))
    {
        _responseFile << line;
        if (!readFromFile.eof())
            _responseFile << "\n";
    }

    // close Openned Files
    if (readFromFile.is_open())
        readFromFile.close();
    if (_responseFile.is_open())
        _responseFile.close();
}

void Response::craftErrorPage(std::string errorMsg, size_t statusCode)
{
    std::string errorPagePath;
    std::string errorPageMsg = errorMsg;
    
    if (statusCode == 400)
        errorPagePath = "./errorPages/badRequest.html";
    else if (statusCode == 501)
        errorPagePath = "./errorPages/notImplemented.html";
    else if (statusCode == 505)
        errorPagePath = "./errorPages/unsupportedVersion.html";
    else if (statusCode == 405)
        errorPagePath = "./errorPages/notAllowed.html";
    else if (statusCode == 403)
        errorPagePath = "./errorPages/forbidden.html";
    else if (statusCode == 413)
        errorPagePath = "./errorPages/payloadLarge.html";
    else if (statusCode == 500)
        errorPagePath = "./errorPages/internalServer.html";
    else if (statusCode == 404)
        errorPagePath = "./errorPages/notFound.html";
    
    craftResponse(errorPagePath, errorPageMsg, statusCode, true);
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

int Response::isBadRequest()
{

    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if (Method != "GET" && Method != "POST" && Method != "PUT" && Method != "PATCH" && Method != "DELETE" && Method != "COPY" && Method != "HEAD" && Method != "OPTIONS" && Method != "LINK" && Method != "UNLINK" && Method != "PURGE" && Method != "LOCK" && Method != "UNLOCK" && Method != "PROPFIND" && Method != "VIEW" && Version != "HTTP/1.1" && Version != "HTTP/1.0" && Version != "HTTP/2.0" && Version != "HTTP/3.0")
        return 0;
    return 1;
}

int Response::is_notImplemented()
{
    std::string Method = _request.Getrequest().at("Method");
    if ((Method != "GET" && Method != "POST" && Method != "DELETE"))
        return 0;
    return 1;
}
int Response::is_unsupportedVersion()
{
    std::string Version = _request.Getrequest().at("Version");
    if ((Version != "HTTP/1.1" && Version != "HTTP/1.0"))
        return 0;
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
int Response::isForbiddenResource(std::string resource, int locationIndex)
{
    Location locationBlock = _server.getLocations()[locationIndex];
    std::vector<std::string> indexes = locationBlock.getIndex();
    int fileType = defineFileType(resource);
    
    if (fileType == 0) // resource is a file
    {
        // check with access function
        if (access(resource.c_str(), R_OK) == -1) // check for read permission it will throw an error eventually if file doesnt exist
            return 1;
        else
            return 0;
    }
    else if (fileType == 1 && _request.Getrequest().at("Method").compare("GET") == 0) // resource is a directory
    {
        if (indexes.size() == 0 && shouldListContent(_server, locationIndex) == 0)
            return 1;
        if (indexes.size() == 0 && shouldListContent(_server, locationIndex) == 1)
        {
            if (access(resource.c_str(), R_OK) == 0) // i should list content here
                return 2;
            else
                return 1; // no access rights return of access is -1
        }
        if (indexes.size() != 0 && shouldListContent(_server, locationIndex) == 1)
        {
           if (access(resource.c_str(), R_OK) == 0) // i should list content
                return 2;
            else
                return 1;
        }
        if (indexes.size() != 0 && shouldListContent(_server, locationIndex) == 0)
        {
            if (access(resource.c_str(), R_OK) == 0) // i should append an index to serve
                return 0;
            else
                return 1;
        }
    }
    return 1; // should be always forbidden unless permission's are cheked !!? or not
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

size_t Response::getSizeOfFile(std::string file) // thank you tutorialpoints again
{
    size_t start;
    size_t end;
    std::ifstream myfile (file);
    
    start = myfile.tellg();
    myfile.seekg (0, std::ios::end);
    end = myfile.tellg();
    myfile.close();
    
    return (end - start);
}

