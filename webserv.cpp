

#include "includes/Webserv.hpp"
#include <math.h>

int main(int argc, char **argv, char **env)
{
    Config conf;
    IOMultiplexing io;
    signal(SIGPIPE, SIG_IGN);
    if (argc != 1 && argc != 2)
    {
        std::cout << "Usage :  ./webserv [Config file path]" << std::endl;
        return (1);
    }
    else if (argc == 1)
    {
        conf.SetConfigFile("./config.d/default.conf");
        conf.setEnv(env);
        io.SetupServers(conf);
    }
    else
    {
        conf.SetConfigFile(argv[1]);
        conf.setEnv(env);
        io.SetupServers(conf);
    }
    return 0;
}