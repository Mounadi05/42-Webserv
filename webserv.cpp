#include "includes/Webserv.hpp"
#include <math.h>
#include <climits>


#define FILE 0
#define DIRECTORY 1

int defineFileType(std::string pathToResource) // stackOverFlow Thank you
{
    struct stat s;
    if (stat(pathToResource.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            // a directory
            return DIRECTORY;
        }
        else if (s.st_mode & S_IFREG)
        {
            // a regular file
            return FILE;
        }
    }
    return -1;
}

// notFinished depending if we should delete a directory or not
int deleteRequest(std::string pathToDelete)
{
    int fileType = defineFileType(pathToDelete);
    if (fileType == FILE)
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
    else if (fileType == DIRECTORY)
    {
        if (std::remove(pathToDelete.c_str()) == 0)
            return 1;
        return -1;
    }
    return -1;
}


// int isForbiddenResource(std::string resource)
// {
//     if (resource == FILE) // resource is a file
//     {
//         // check with access function
//         if (access(resource, R_OK) == -1) // check for read permission it will throw an error eventually if file doesnt exist
//             return 1;
//         return 0;
//     }
//     else if (resource == DIRECTORY) // resource is a directory
//     {

//     }
//     return 1; // should be always forbidden unless permission's are cheked !!?
// }

int isPayloadTooLarge(Server server, Location locationBlock, int contentLengthRequested)
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

int isAllowedMethod(Server server, Location locationBlock, std::string requestedMethod)
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

int	defineLocation(std::vector<Location> location, std::string uriPath)
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


// not sure if this is working
std::string setFullPath(Server server, std::string uriPath)
{
    std::string fullPath;
    std::string rootPath;
    int locationIndex = defineLocation(server.getLocations(), uriPath);

    if (server.getLocations()[locationIndex].getLocationPath().empty() == false)
        rootPath = server.getLocations()[locationIndex].getRoot();
    else
        rootPath = server.getRoot();
    fullPath = rootPath + uriPath.erase(0, server.getLocations()[locationIndex].getLocationPath().size());
    return fullPath;
}

int main(int argc, char **argv)
{
    Config conf;
    IOMultiplexing io;
    if (argc != 1 && argc != 2)
    {
        std::cout << "Usage :  ./webserv [Config file path]" << std::endl;
        return (1);
    }
    else if (argc == 1)
    {
        conf.SetConfigFile("./config.d/default.conf");
        // conf.parse();
        // conf.print_servers();
        // while(1);
        
    }
    else
        conf.SetConfigFile(argv[1]);
    //io.SetupServers(conf);
    conf.parse();
    
    std::vector<Server> servers = conf.getServers();
    std::vector<Location> locations = servers[0].getLocations();
    
    for (size_t j = 0 ; j < locations.size(); j++)
    {
        std::cout << "location_" << j << std::endl;
        std::cout << " " << locations[j].getLocationPath() << std::endl;
    }

    std::string path = setFullPath(servers[0], "/gym-website-template");
    std::cout << path << std::endl;
    return 0;
}