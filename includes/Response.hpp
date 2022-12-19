#ifndef Response_class
#define Response_class

#include "Request.hpp"
#include "Server.hpp"
#include <sys/stat.h>
std::string generate_autoindex(std::string path);
class Response
{
    private:
        int _ClientFD;
        Request _request;
        Server _server;
        char str[1025];
        int lent;
        int ok;
        int lent_re;
        int fd;
        int size;
        int _send;
        int done;
        std::string Path;
        std::string full_path;
        int en_handle;
        int post;
        std::string upload;
        int fd_upload;
         u_int64_t lent_server;
         int fd_error;
         int error;
         std::string status_code;
         Location locations;
     public:
        Response();
        Response(Request request,Server  server, int ClientFD);
        ~Response();
        int getClientFD() const;
        Request & getRequest();
        Server & getServer();
        int &get_done(void);
        std::string get_extension(std::string str);
        std::string get_type(std::string path);
        std::string delete_space(std::string str);
        int handler(fd_set &r , fd_set &w);
        int is_Valide(fd_set &r , fd_set &w);
        int is_Unauthorize(fd_set &r , fd_set &w);
        int handle_redirection(fd_set &r , fd_set &w);
        int handle_location(fd_set &r , fd_set &w);
        int handle_index(void);
        int handle_method(fd_set &r ,fd_set &w);
        int check_autoindex(fd_set &r , fd_set &w);
        int check_location(fd_set &r , fd_set &w);
        int redirect_path(fd_set &r , fd_set &w);
        void send_data(fd_set &r , fd_set &w);
        int  handle_autoindex(fd_set &r , fd_set &w);
        int  check_upload(fd_set &r , fd_set &w);
        int check_lent(fd_set &r , fd_set &w);
        int isToo_large(fd_set &r , fd_set &w);
        int last_slash();
        void write_body(fd_set &r , fd_set &w);
        int check_Content(fd_set &r , fd_set &w);
        int check_permission(fd_set &r , fd_set &w);
        void handler_delete(fd_set &r , fd_set &w);
        void cgi_exec();
        std::map<std::string, std::string> _headers;
        std::vector<std::string> _env;
        void load_env(char **env);
        int send_error(std::string error, std::string m);
        int check_forbidden(fd_set &r , fd_set &w);
};

#endif

