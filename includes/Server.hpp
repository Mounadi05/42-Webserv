#ifndef server_class
#define server_class

#include "./Location.hpp"
#include "./Socket.hpp"

class Server
{
private:
    Socket _socket;
    size_t _port;
    std::vector<std::string> _serverNames;
    std::vector<Location> _locations;
    std::vector<std::pair<std::string, std::string> > _error_pages;
    std::string _uploadPath;
    std::vector<std::string> mime_types;
public:
    Server();
    ~Server();
    std::string root;
    int en_handle;
    Socket &getSocket();
    size_t &getPort();
    std::vector<std::string> &getServerNames();
    std::vector<Location> &getLocations();
    std::vector<std::pair<std::string, std::string> > &getErrorPages();
    std::string &getUploadPath();
    std::vector<std::string> &getmime_types(void);
    std::vector<std::pair<std::string,std::string> > _cgi;
    char **_env;
    int a;



    void setSocket(Socket socket);
    void setPort(size_t port);
    void setServerNames(std::vector<std::string> serverNames);
    void setRoot(std::string root);
    void setAutoIndex(std::string autoIndex);
    void setClientMaxBodySize(std::string clientMaxBodySize);
    void setAllowedMethods(std::vector<std::string> allowedMethods);
    void setLocations(std::vector<Location> locations);
    void setRedirections(std::vector<std::pair<std::string, std::string> > redirections);
    void setErrorPages(std::vector<std::pair<std::string, std::string> > error_pages);
    void setUploadPath(std::string uploadPath);
    void setIndex(std::vector<std::string> index);
    void init_MimeTypes(void);
    void setCgi(std::pair<std::string,std::string> cgi);
};

#endif