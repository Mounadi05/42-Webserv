

#include "includes/Webserv.hpp"
#include <math.h>

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
        io.SetupServers(conf);
    }
    else
        conf.SetConfigFile(argv[1]);
    return 0;
}