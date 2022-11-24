#ifndef server_class
#define server_class

#include "./Location.hpp"
#include "./Socket.hpp"

class Server
{
private:
    Socket _socket;
    // std::string                                         _host;
    size_t _port;
    std::vector<std::string> _serverNames;
    std::string _root;
    std::string _autoIndex;
    std::string _clientMaxBodySize;
    std::vector<std::string> _allowedMethods;
    std::vector<Location> _locations;
    std::vector<std::pair<std::string, std::string> > _redirections;
    std::vector<std::pair<std::string, std::string> > _error_pages;
    std::string _uploadPath;
    std::vector<std::string> _index;
    std::vector<std::string> mime_types;
    

public:
    Server();
    ~Server();
    std::string root;
    int en_handle;
    Socket &getSocket();
    // std::string                                         & getHost();
    size_t &getPort();
    std::vector<std::string> &getServerNames();
    std::string &getRoot();
    std::string &getAutoIndex();
    std::string &getClientMaxBodySize();
    std::vector<std::string> &getAllowedMethods();
    std::vector<Location> &getLocations();
    std::vector<std::pair<std::string, std::string> > &getRedirections();
    std::vector<std::pair<std::string, std::string> > &getErrorPages();
    std::string &getUploadPath();
    std::vector<std::string> &getIndex();
    std::vector<std::string> &getmime_types(void);
    int a;



    void setSocket(Socket socket);
    // void                                                setHost(std::string host);
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
};

#endif