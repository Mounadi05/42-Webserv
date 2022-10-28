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

    return 0;
}