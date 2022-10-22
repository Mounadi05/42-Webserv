#include "../../includes/Webserv.hpp"

Response::Response()
{

}

Response::Response(Request  request,Server  server, int ClientFD)
{
    _request = request;
    _server = server;
    _ClientFD = ClientFD;
    str = new char[1025];
    lent  = 0;
    finish = 0;
    lent_re = 0;
    size = 0;
    _send = 0;
    done = 0;
}

Response::~Response()
{

}

int Response::getClientFD() const{
    return _ClientFD;
}

Request & Response::getRequest()
{
    return _request;
}

Server & Response::getServer()
{
    return _server;
}
int &Response::get_done(void)
{
    return done;
}
std::string Response::get_extension(std::string str)
{
    std::string tmp ;
    int i = str.size();
    while(i != 0 && i--)
    {
            if (str[i] != '.')
                tmp.insert(tmp.begin(),str[i]);
            else
                return tmp;
    }
    return tmp;
}
std::string Response::delete_space(std::string str)
{   
    for(int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i,1);
    return str;
}
std::string Response::get_type(std::string path)
{
    std::string tmp = get_extension(path);
    std::string str;
    std::ifstream file;
    file.open("www/mime.types") ;
    while(getline(file,str))
    {
        if ((int)str.find(tmp,0) != -1 )
        {
            str = str.substr(0,str.find("|",0));
            return str;
        }
        str.clear();
    }
    return path;
}
int Response::handler(fd_set &r , fd_set &w)
{
    send_data(r,w);
    return 1;
}
