#ifndef Response_class
#define Response_class

#include "Request.hpp"
#include "Server.hpp"
#include <sys/stat.h>
class Response
{
    private:
        int _ClientFD;
        Request _request;
        Server *_server;
        char *str;
        int lent;
        int finish;
        int lent_re;
        int fd;
        int size;
        int _send;
        int done;
        std::string Path;
        std::vector<std::string> index;
    public:
        int _rederict;
        int _refere;
        Response();
        Response(Request request,Server  *server, int ClientFD);
        ~Response();
        int getClientFD() const;
        Request & getRequest();
        Server * getServer();
        int &get_done(void);
        std::string get_extension(std::string str);
        std::string get_type(std::string path);
        std::string delete_space(std::string str);
        int handler(fd_set &r , fd_set &w);
        int is_Valide(fd_set &r , fd_set &w);
        int is_Unauthorize(fd_set &r , fd_set &w);
        int handle_location(void);
        int handle_index(void);
        void send_data(fd_set &r , fd_set &w);
        Location & define_location(std::string location_path);
        std::string check_index(Location & _location);
        std::string getLocationRefere();
        std::string location_handler();
        std::string grepLocation(std::string path, std::vector<Location> locations);
};

#endif
