#include "includes/Webserv.hpp"
#include <math.h>
#include <climits>

#define FILE 0
#define DIRECTORY 1

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