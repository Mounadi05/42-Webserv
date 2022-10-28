#include "includes/Webserv.hpp"
#include <math.h>
#include <climits>

#define FILE 0
#define DIRECTORY 1

std::string get_extension(std::string str)
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

int defineFileType(std::string pathToResource) // stackOverFlow Thank you
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

int shouldInvokeCGI(std::string uriPath)
{
    std::string extention = get_extension(uriPath);

    if (extention.compare("py") == 0 || extention.compare("php") == 0)
    {
        ;
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

    std::pair<std::string, std::string> redirection;

    redirection.first = "pathToRedirect";
    redirection.second = "redirectToPath";


    if (redirection.first.size() != 0)
    {
        if (redirection.first.compare("pathToRedirect") == 0)
            std::cout << "your URL should be redirected to : " << redirection.second << std::endl;
        else
            std::cout << "Nothing Found GTFO" << std::endl;
    }
    return 0;
}