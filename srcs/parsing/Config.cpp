#include "../../includes/Webserv.hpp"

void Config::setEnv(char **env){
    _env = env;
}

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
    this->_Configfile.open(_FilePath.c_str());

    if (_Configfile.is_open())
    {
        if (is_empty(_Configfile))
            printError("Empty config file");
    }
    else
        printError("Unable to open " + Path);
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
                printError("config File Error");
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
                server._env = this->_env;
                _Servers.push_back(server);
                n_server++;
            }
            else if (token == "listen")
                hadel_listen(line, _Servers[n_server - 1]);
            else if (token == "server_name")
                handel_server_name(line, _Servers[n_server - 1]);
            else if (token == "cgi")
            {
                std::string cgi_path = getNextToken(line);
                std::string cgi_extension = getNextToken(line);
                if (cgi_path == "" || cgi_extension == "")
                    printError("config File Error");
                if (cgi_extension[0] != '.')
                    printError("Bad cgi extension");
                _Servers[n_server - 1].setCgi(std::pair<std::string,std::string>(cgi_path, cgi_extension));
            }
            // else if (token == "root")
            // {
            //     std::string root = getNextToken(line);
            //     if (root == "")
            //         printError("root without value");
            //     _Servers[n_server - 1].setRoot(root);
            // }
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
            // else if (token == "index")
            // {
            //     std::vector<std::string> indexes;
            //     std::string index = getNextToken(line);
            //     if (index == "")
            //         printError("index without value");
            //     while (index != "")
            //     {
            //         indexes.push_back(index);
            //         index = getNextToken(line);
            //     }
            //     _Servers[n_server - 1].setIndex(indexes);
            // }
            // else if (token == "client_max_body_size")
            // {
            //     std::string client_max_body_size = getNextToken(line);
            //     if (client_max_body_size == "")
            //         printError("client_max_body_size without value");
            //     _Servers[n_server - 1].setClientMaxBodySize(client_max_body_size);
            // }
            else if (token == "location")
                handel_location(line, _Servers[n_server - 1]);
            // else if (token == "autoindex")
            // {
            //     std::string autoindex = getNextToken(line);
            //     if (autoindex == "")
            //         printError("autoindex must be on or off");
            //     _Servers[n_server - 1].setAutoIndex(autoindex);
            // }
            else if (token == "upload_path")
            {
                std::string upload_store = getNextToken(line);
                if (upload_store == "")
                    printError("upload_path without path");
                _Servers[n_server - 1].setUploadPath(upload_store);
            }
            // else if (token == "allow_methods")
            // {
            //     std::vector<std::string> allow_methods;

            //     std::string allow_method = getNextToken(line);

            //     if (allow_method == "")
            //         printError("allow_methods without methods");
            //     while (allow_method != "")
            //     {
            //         allow_methods.push_back(allow_method);
            //         allow_method = getNextToken(line);
            //     }
            //     _Servers[n_server - 1].setAllowedMethods(allow_methods);
            // }
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
        if (buf[i] < '0' || buf[i] > '9')
            return 0;
    return (1);
}

int valide_error_code(std::string error_code)
{
    if (is_number(error_code))
    {
        if (stoi(error_code) != 404 && stoi(error_code) != 403 && stoi(error_code) != 405)
            printError("not valid code");
    }
    else
        printError("not valid code");
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

bool valid_location_root(std::string path)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        if (path[path.length() - 1] == '/')
            return 1;
    }
    return 0;
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
                if (method == "")
                    printError("allow_methods is empty");
                while (method != "")
                {
                    if (!check_method(method))
                        printError("bad methos as argument");
                    methods.push_back(method);
                    method = getNextToken(line);
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
                if (valid_location_root(root))
                    location.setRoot(root);
                else
                    printError("root is not a valid path");
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
            else if (token == "return")
            {
                std::string code = getNextToken(line);
                if (code == "")
                    printError("return code is empty");
                else
                {
                    if (code == "302")
                    {
                        std::string url = getNextToken(line);
                        if (url == "")
                            printError("return url is empty");
                        else
                            location.setRedirection(std::pair<std::string,std::string>(code, url));
                    }
                    else
                        printError("return code is not valid");
                }
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

bool compare_location(Location &a,  Location &b)
{
    return (a.getLocationPath().length()) < (b.getLocationPath().length());
}

std::vector<Server> &Config::getServers()
{
    for (std::vector<Server>::iterator it = _Servers.begin(); it != _Servers.end(); it++)
    {
        std::sort((it)->getLocations().begin(), (it)->getLocations().end(), compare_location);
    }
    return _Servers;
}