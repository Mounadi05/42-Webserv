#ifndef Config_class
#define Config_class

#include "Server.hpp"
#include <fstream>
#include <vector>

class Config{
    std::vector<Server> _Servers;
    std::string _FilePath;
    std::ifstream _Configfile;
    

    public :
    Config();
    Config(std::string FilePath);
    ~Config();
    void SetConfigFile(std::string Path);
    // Config & operator=(const Config & conf);
    // Config(const Config & conf);
    void parse();
    void handel_location(std::string &line ,Server &server);
    void handel_server_name(std::string &line, Server &server);
    void hadel_listen(std::string &line, Server &server);
    void print_servers();
    std::vector<Server> & getServers();
    std::string getFilePath() const;
    char **_env;
    void setEnv(char **env);
};

bool valide_host(std::string &host);
std::string getNextToken(std::string &line);
bool is_key(char c);
std::string trim(std::string &str);
std::string getBlockName(std::string &line);
bool skip_empty_lines(std::ifstream &pFile);
bool is_empty(std::ifstream &pFile);
void printError(std::string message);
int valide_error_code(std::string error_code);

#endif