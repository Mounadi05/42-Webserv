#include "includes/Webserv.hpp"
#include <math.h>
#include <climits>


#define FILE 0
#define DIRECTORY 1



// notFinished depending if we should delete a directory or not
// int deleteRequest(std::string pathToDelete)
// {
//     int fileType = defineFileType(pathToDelete);
//     if (fileType == FILE)
//     {
//         if (std::remove(pathToDelete.c_str()) == 0)
//         {
//             std::cout << "File Successfully deleted" << std::endl;
//             return 1;
//         }
//         else
//         {
//             std::cout << "File Not deleted" << std::endl;
//             return -1;
//         }
//     }
//     else if (fileType == DIRECTORY)
//     {
//         if (std::remove(pathToDelete.c_str()) == 0)
//             return 1;
//         return -1;
//     }
//     return -1;
// }


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

// not completed should find a way to store the path of status code found
// int checkIfshouldUseDefaultErrorPage(Server server, int locationIndex, std::string statusCode)
// {
//     std::vector<std::string, std::string> serverErrorPages = server.getErrorPages();
//     std::vector<std::string, std::string> locationBlockErrorPages = server.getLocations()[locationIndex].getErrorPages(); 
//     if (locationBlockErrorPages.size() != 0)
//     {
//         for (size_t i = 0; i < locationBlockErrorPages.size(); i++)
//         {
//             if (locationBlockErrorPages[i].first.compare(statusCode) == 0)
//             {
//                 // here i should find a way to define the path to the error page
//                 return 1;
//             }
//         }
//         return 0;
//     }
//     else if (serverErrorPages.size() != 0)
//     {
//         for (size_t i = 0; i < serverErrorPages.size() ; i++)
//         {
//             if (serverErrorPages[i].first.compare(statusCode) == 0)
//             {
//                 // here i should find a way to define the path to the error page
//                 return 1;
//             }
//         }
//         return 0;
//     }
//     return 0;
// }

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

    std::string pathtosearch = "/index.php?param1=value1&param2=value2";
    std::string path;
    std::string querys;
    if (pathtosearch.find("?") != std::string::npos)
    {
        path = pathtosearch.substr(0, pathtosearch.find("?"));
        querys = pathtosearch.erase(0, pathtosearch.find("?"));
    }
    else
        path = pathtosearch;
    return 0;
}