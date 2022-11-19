#include "../../includes/Webserv.hpp"

Config::Config()
{
}

Config::~Config()
{
    _Configfile.close();
}

void Config::SetConfigFile(std::string Path)
{
    this->_FilePath = Path;
    this->_Configfile.open(_FilePath);

    if (_Configfile.is_open())
    {
        if (is_empty(_Configfile))
            printError("Empty config file");
    }
    else
        printError("Unable to open " + Path);
}

void Config::print_servers()
{
    for (size_t i = 0; i < _Servers.size(); i++)
    {
        std::cout << "Server: " << i << std::endl;
        // std::cout << "Host: " << _Servers[i].getHost() << std::endl;
        std::cout << "Port: " << _Servers[i].getPort() << std::endl;
        std::cout << "Server names: ";
        for (size_t j = 0; j < _Servers[i].getServerNames().size(); j++)
            std::cout << _Servers[i].getServerNames()[j] << " ";
        std::cout << std::endl;
        std::cout << "Root: " << _Servers[i].getRoot() << std::endl;
        std::cout << "Autoindex: " << _Servers[i].getAutoIndex() << std::endl;
        std::cout << "Allowed methods: ";
        for (size_t j = 0; j < _Servers[i].getAllowedMethods().size(); j++)
            std::cout << _Servers[i].getAllowedMethods()[j] << " ";
        std::cout << std::endl;
        std::cout << "Client max body size: " << _Servers[i].getClientMaxBodySize() << std::endl;
        std::cout << "Upload path: " << _Servers[i].getUploadPath() << std::endl;
        std::cout << "Error pages: ";
        for (size_t j = 0; j < _Servers[i].getErrorPages().size(); j++)
            std::cout << _Servers[i].getErrorPages()[j].first << " " << _Servers[i].getErrorPages()[j].second;
        std::cout << std::endl;
        std::cout << "Indexes: ";
        for (size_t j = 0; j < _Servers[i].getIndex().size(); j++)
            std::cout << _Servers[i].getIndex()[j] << " ";
        std::cout << std::endl;
        std::cout << "Locations: " << std::endl;
        for (size_t j = 0; j < _Servers[i].getLocations().size(); j++)
        {
            std::cout << "===========================" << std::endl;
            std::cout << "Location path: " << _Servers[i].getLocations()[j].getLocationPath() << std::endl;
            std::cout << "Root: " << _Servers[i].getLocations()[j].getRoot() << std::endl;
            std::cout << "Autoindex: " << _Servers[i].getLocations()[j].getAutoIndex() << std::endl;
            std::cout << "Allowed methods: ";
            for (size_t k = 0; k < _Servers[i].getLocations()[j].getAllowedMethods().size(); k++)
                std::cout << _Servers[i].getLocations()[j].getAllowedMethods()[k] << " ";
            std::cout << std::endl;
            std::cout << "Client max body size: " << _Servers[i].getLocations()[j].getClientMaxBodySize() << std::endl;
            std::cout << "Upload path: " << _Servers[i].getLocations()[j].getUploadPath() << std::endl;
            std::cout << "Error pages: ";
            for (size_t k = 0; k < _Servers[i].getLocations()[j].getErrorPages().size(); k++)
                std::cout << _Servers[i].getLocations()[j].getErrorPages()[k].first << " " << _Servers[i].getLocations()[j].getErrorPages()[k].second;
            std::cout << std::endl;
            std::cout << "Indexes: ";
            for (size_t k = 0; k < _Servers[i].getLocations()[j].getIndex().size(); k++)
                std::cout << _Servers[i].getLocations()[j].getIndex()[k] << " ";
            std::cout << std::endl;
        }
        std::cout << "---------------------------------" << std::endl;
    }
}

void Config::parse()
{
    std::vector<std::string> curly_phrases;
    int n_server = 0;
    std::string line;

    int start = 0;
    while (skip_empty_lines(_Configfile) && getline(_Configfile, line))
    {
        std::string token;
        while ((token = getNextToken(line)) != "")
        {
            skip_empty_lines(_Configfile);
            if (!start && token != "server")
                printError("Bad config File");
            else
                start = 1;
            if (token == "{")
                curly_phrases.push_back(token);
            else if (token == "}")
            {
                if (curly_phrases.size() != 0)
                    curly_phrases.pop_back();
                else
                    printError("missing opning server block");
            }
            else if (token == "server")
            {
                Server server;
                _Servers.push_back(server);
                n_server++;
            }
            else if (token == "listen")
                hadel_listen(line, _Servers[n_server - 1]);
            else if (token == "server_name")
                handel_server_name(line, _Servers[n_server - 1]);
            else if (token == "root")
            {
                std::string root = getNextToken(line);
                if (root == "")
                    printError("root without value");
                _Servers[n_server - 1].setRoot(root);
            }
            else if (token == "error_page")
            {
                std::vector<std::pair<std::string, std::string> > error_pages;
                std::string error_code = getNextToken(line);
                std::string error_path = getNextToken(line);
                if (error_code == "" || error_path == "")
                    printError("error_page must have 2 arguments");
                while (error_code != "" && error_path != "")
                {
                    error_pages.push_back(std::make_pair(error_code, error_path));
                    if (valide_error_code(error_code))
                        error_code = getNextToken(line);
                    else
                        printError("");
                    error_path = getNextToken(line);
                }
                _Servers[n_server - 1].setErrorPages(error_pages);
            }
            else if (token == "index")
            {
                std::vector<std::string> indexes;
                std::string index = getNextToken(line);
                if (index == "")
                    printError("index without value");
                while (index != "")
                {
                    indexes.push_back(index);
                    index = getNextToken(line);
                }
                _Servers[n_server - 1].setIndex(indexes);
            }
            else if (token == "client_max_body_size")
            {
                std::string client_max_body_size = getNextToken(line);
                if (client_max_body_size == "")
                    printError("client_max_body_size without value");
                _Servers[n_server - 1].setClientMaxBodySize(client_max_body_size);
            }
            else if (token == "location")
                handel_location(line, _Servers[n_server - 1]);
            else if (token == "autoindex")
            {
                std::string autoindex = getNextToken(line);
                if (autoindex == "")
                    printError("autoindex must be on or off");
                _Servers[n_server - 1].setAutoIndex(autoindex);
            }
            else if (token == "upload_path")
            {
                std::string upload_store = getNextToken(line);
                if (upload_store == "")
                    printError("upload_path without path");
                _Servers[n_server - 1].setUploadPath(upload_store);
            }
            else if (token == "allow_methods")
            {
                std::vector<std::string> allow_methods;

                std::string allow_method = getNextToken(line);

                if (allow_method == "")
                    printError("allow_methods without methods");
                while (allow_method != "")
                {
                    allow_methods.push_back(allow_method);
                    allow_method = getNextToken(line);
                }
                _Servers[n_server - 1].setAllowedMethods(allow_methods);
            }
            else
                printError("unknown token " + token);
        }
    }
}

int check_method(std::string method)
{
    std::string tmp;
    for (u_int x = 0; x < method.length(); x++)
        tmp += toupper(method[x]);
    return (tmp == "POST" || tmp == "DELETE" || tmp == "GET");
}

void Config::hadel_listen(std::string &line, Server &server)
{
    std::string port = getNextToken(line);
    if (port == "")
        printError("listen directive must have a port");
    try
    {
        if (stoi(port) >= 0 && stoi(port) <= 65536)
            server.setPort(stoi(port));
        else
            printError("port must be between 0 and 65536");
    }
    catch (...)
    {
        printError("port must be a number");
    }
}

int is_number(std::string buf)
{
    for (size_t i = 0; i < buf.length(); i++)
        if (!isnumber(buf[i]))
            return 0;
    return (1);
}

int valide_error_code(std::string error_code)
{
    // int number = 0;
    if (is_number(error_code))
    {
        if (stoi(error_code) < 400)
            printError("not valid error code");
    }
    else
        printError("not valid error code");
    return 1;
}

void Config::handel_server_name(std::string &line, Server &server)
{
    std::vector<std::string> server_names;
    std::string server_name = getNextToken(line);
    if (server_name == "")
        printError("server_name is empty");
    while (server_name != "")
    {
        server_names.push_back(server_name);
        server_name = getNextToken(line);
    }
    server.setServerNames(server_names);
}

std::string Config::getFilePath() const
{
    return this->_FilePath;
}

void Config::handel_location(std::string &line, Server &server)
{
    Location location;
    std::vector<std::string> curly_phrases;
    std::string token = "";
    std::string token_path = getNextToken(line);
    if (token_path == "")
        printError("location path is empty");
    location.setLocationPath(token_path);

    // int start = 0;
    while (skip_empty_lines(_Configfile) && getline(_Configfile, line))
    {
        while ((token = getNextToken(line)) != "")
        {
            if (token == "{")
            {
                curly_phrases.push_back(token);
                continue;
            }
            else if (token == "}")
            {
                if (curly_phrases.size() == 0)
                    printError("missing opning location block");
                if (curly_phrases.size())
                    curly_phrases.pop_back();
                if (curly_phrases.size() == 0)
                {
                    server.getLocations().push_back(location);
                    return;
                }
            }
            else if (token == "allow_methods")
            {
                std::vector<std::string> methods;
                std::string method = getNextToken(line);
                // std::cout << check_method(method) << ":" << method << std::endl;
                if (method == "")
                    printError("allow_methods is empty");
                while (method != "")
                {
                    if (!check_method(method))
                        printError("bad methos as argument");
                    methods.push_back(method);
                    method = getNextToken(line);
                    // std::cout << check_method(method) << ":" << method << std::endl;
                }
                location.setAllowedMethods(methods);
            }
            else if (token == "client_max_body_size")
            {
                std::string size = getNextToken(line);
                if (size == "")
                    printError("client_max_body_size is empty");
                if (is_number(size))
                    location.setClientMaxBodySize(size);
                else
                    printError("client_max_body_size must be a number");
            }
            else if (token == "root")
            {
                std::string root = getNextToken(line);
                if (root == "")
                    printError("root is empty");
                location.setRoot(root);
            }
            else if (token == "autoindex")
            {
                std::string autoindex = getNextToken(line);
                if (autoindex == "")
                    printError("autoindex is empty");
                location.setAutoIndex(autoindex);
            }
            else if (token == "index")
            {
                std::vector<std::string> indexes;
                std::string index = getNextToken(line);
                if (index == "")
                    printError("index is empty");
                while (index != "")
                {
                    indexes.push_back(index);
                    index = getNextToken(line);
                }
                location.setIndex(indexes);
            }
            else
                printError("Config File Error");
        }
    }
}

bool is_empty(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void printError(std::string message)
{
    std::cerr << "Error : " << message << std::endl;
    exit(1);
}

bool skip_empty_lines(std::ifstream &pFile)
{
    while (pFile.peek() == '\n' || pFile.peek() == '\t' || pFile.peek() == ' ')
        pFile.get();
    return true;
}

bool is_key(char c)
{
    return c == '_' || c == ':' || c == '/' || c == '\\' || c == '.' || c == '-' || isalnum(c);
}

std::string getNextToken(std::string &line)
{
    std::string token = "";
    int i = 0;

    while (line[i] != '\0')
    {
        if (line[i] == ' ' || line[i] == '\t')
        {
            i++;
            continue;
        }
        if (line[i] == '{')
        {
            token += line[i];
            i++;
            break;
        }
        if (line[i] == '}')
        {
            token += line[i];
            i++;
            break;
        }
        if (is_key(line[i]))
        {
            while (is_key(line[i]))
            {
                token += line[i];
                i++;
            }
            break;
        }
        i++;
    }
    line = line.substr(i, line.length() - i);
    return token;
}

std::vector<Server> &Config::getServers()
{
    return _Servers;
}