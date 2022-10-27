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

int shouldListIndexes(Server server, int locationIndex)
{
    int autoIndexOn;
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

// not completed should find a way to store the path of status code found
int checkIfshouldUseDefaultErrorPage(Server server, int locationIndex, std::string statusCode)
{
    std::vector<std::string, std::string> serverErrorPages = server.getErrorPages();
    std::vector<std::string, std::string> locationBlockErrorPages = server.getLocations()[locationIndex].getErrorPages(); 
    if (locationBlockErrorPages.size() != 0)
    {
        for (size_t i = 0; i < locationBlockErrorPages.size(); i++)
        {
            if (locationBlockErrorPages[i].first.compare(statusCode) == 0)
            {
                // here i should find a way to define the path to the error page
                return 1;
            }
        }
        return 0;
    }
    else if (serverErrorPages.size() != 0)
    {
        for (size_t i = 0; i < serverErrorPages.size() ; i++)
        {
            if (serverErrorPages[i].first.compare(statusCode) == 0)
            {
                // here i should find a way to define the path to the error page
                return 1;
            }
        }
        return 0;
    }
    return 0;
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

    std::string example = "/Ping-Pong";
    std::cout << example[example.size() - 1] << std::endl;
    return 0;
}