#include "includes/Webserv.hpp"
#include <math.h>
#include <climits>

#define FILE 0
#define DIRECTORY 1

// int shouldInvokeCGI(std::string uriPath)
// {
//     std::string extention = get_extension(uriPath);

//     if (extention.compare("py") == 0 || extention.compare("php") == 0)
//     {
//         ;
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
    conf.parse();
    io.SetupServers(conf);
    
    std::vector<Server> servers = conf.getServers();
    std::vector<Location> locations = servers[0].getLocations();
    std::vector<std::string> mimes = servers[0].getmime_types();

    
    return 0;
}