#include "../../includes/Webserv.hpp"

std::string generate_autoindex(std::string path , std::string root);

Response::Response()
{
    _state = INIT;
}

Response::Response(Request request, Server server, int ClientFD)
{
    _request = request;
    _server = server;
    _ClientFD = ClientFD;
    str = new char[1025];
    lent = 0;
    finish = 0;
    lent_re = 0;
    size = 0;
    _send = 0;
    done = 0;
    _state = INIT;
    _lindex = -1;
}

Response::~Response()
{
}

int Response::getClientFD() const
{
    return _ClientFD;
}

Request &Response::getRequest()
{
    return _request;
}

Server &Response::getServer()
{
    return _server;
}

int &Response::get_done(void)
{
    return done;
}

std::string Response::get_extension(std::string str)
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

std::string Response::delete_space(std::string str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i, 1);
    return str;
}

std::string Response::get_type(std::string path)
{
    std::string tmp = get_extension(path);
    for (int i = 0; i < (int)_server.getmime_types().size(); i++)
        if ((int)_server.getmime_types()[i].find(tmp, 0) != -1)
            return _server.getmime_types()[i].substr(0, _server.getmime_types()[i].find("|", 0));
    return path;
}

std::string stringtrim(std::string str)
{
    int i = 0;
    int j = str.size() - 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (str[j] == ' ' || str[j] == '\t')
        j--;
    return str.substr(i, j - i + 1);
}

int Response::defineLocation(std::string path)
{
    for (size_t i = 0; i < _server.getLocations().size(); i++)
    {
        if (path == _server.getLocations()[i].getLocationPath())
        {
            _lindex = i;
            return (1);
        }
    }
    return (0);
}

int Response::checkMethod(void)
{
    std::string method = stringtrim(_request.Getrequest().at("Method"));
    for (size_t i = 0; i < _server.getLocations()[_lindex].getAllowedMethods().size(); i++)
    {
        if (method == _server.getLocations()[_lindex].getAllowedMethods()[i])
            return (1);
    }
    return (0);
}

void send_error(int fd, int code, bool Default = true, std::string path = "")
{
    (void)path;
    std::string msg;
    std::string resp;

    if (Default)
    {
        if (code == 400)
            msg = "Bad Request";
        else if (code == 403)
            msg = "Forbidden";
        else if (code == 404)
            msg = "Not Found";
        else if (code == 405)
            msg = "Method Not Allowed";
        else if (code == 413)
            msg = "Payload Too Large";
        else if (code == 501)
            msg = "Not Implemented";
        else if (code == 505)
            msg = "HTTP Version Not Supported";

        resp = "HTTP/1.1 " + std::to_string(code) + " " + msg + "\r\n";
        resp += "Content-Type: text/html\r\n";
        resp += "Content-Length: " + std::to_string(msg.size()) + "\r\n";
        resp += "Connection: close\r\n";
        resp += "\r\n";
        resp += msg;
        send(fd, resp.c_str(), resp.size(), 0);
    }
    else
    {
        std::ifstream file(path);
        std::string headers;
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
                resp += line;
            file.close();
            headers = "HTTP/1.1 " + std::to_string(code) + " " + msg + "\r\n";
            headers += "Content-Type: text/html\r\n";
            headers += "Content-Length: " + std::to_string(resp.size()) + "\r\n";
            headers += "Connection: close\r\n";
            headers += "\r\n";
            headers += resp;
            send(fd, headers.c_str(), headers.size(), 0);
        }
    }
}

std::string Response::DefineErrorPath(std::string ErrorCode)
{
    for (size_t i = 0; i < _server.getErrorPages().size(); i++)
    {
        if (ErrorCode == _server.getErrorPages()[i].first)
            return (_server.getErrorPages()[i].second);
    }
    return "";
}

int Response::sendResponse(fd_set &r, fd_set &w, std::string filePath)
{
    (void)r;
    (void)w;
    std::string path;

    if (_state == DENIEDMETHOD)
    {
        path = this->DefineErrorPath("405");
        if (path == "")
            send_error(_ClientFD, DENIEDMETHOD);
        else
            send_error(_ClientFD, DENIEDMETHOD, false, path);
    }
    else if (_state == NOTFOUND)
    {
        path = this->DefineErrorPath("404");
        if (path != "" && access(path.c_str(), F_OK) != -1)
            send_error(_ClientFD, NOTFOUND, false, path);
        else
            send_error(_ClientFD, NOTFOUND);
    }
    else if (_state == UNAUTHORIZED)
    {
        path = this->DefineErrorPath("401");
        if (path == "")
            send_error(_ClientFD, UNAUTHORIZED);
        else
            send_error(_ClientFD, UNAUTHORIZED, false, path);
    }
    (void)filePath;
    FD_CLR(_ClientFD, &w);
    FD_SET(_ClientFD, &r);
    return (0);
}

int is_file(std::string path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) == -1)
        return (0);
    if (S_ISREG(buf.st_mode))
        return (1);
    return (0);
}

int is_dir(std::string path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) == -1)
        return (0);
    if (S_ISDIR(buf.st_mode))
        return (1);
    return (0);
}

std::string check_index(std::string path, std::vector<std::string> index)
{
    for (size_t i = 0; i < index.size(); i++)
    {
        if (is_file(path + "/" + index[i]))
            return (path + "/" + index[i]);
    }
    return "";
}

int check_autoindex(std::string value)
{
    if (value == "on")
        return (1);
    return (0);
}

std::string defineType(std::string path)
{
    std::string type;
    std::string ext;
    size_t pos = path.find_last_of(".");
    if (pos != std::string::npos)
        ext = path.substr(pos + 1);
    if (ext == "html")
        type = "text/html";
    else if (ext == "css")
        type = "text/css";
    else if (ext == "js")
        type = "text/javascript";
    else if (ext == "jpg")
        type = "image/jpeg";
    else if (ext == "jpeg")
        type = "image/jpeg";
    else if (ext == "png")
        type = "image/png";
    else if (ext == "gif")
        type = "image/gif";
    else if (ext == "svg")
        type = "image/svg+xml";
    else if (ext == "ico")
        type = "image/x-icon";
    else if (ext == "mp3")
        type = "audio/mpeg";
    else if (ext == "mp4")
        type = "video/mp4";
    else if (ext == "pdf")
        type = "application/pdf";
    else if (ext == "zip")
        type = "application/zip";
    else if (ext == "gz")
        type = "application/gzip";
    else if (ext == "tar")
        type = "application/x-tar";
    else if (ext == "txt")
        type = "text/plain";
    else if (ext == "xml")
        type = "text/xml";
    else if (ext == "json")
        type = "application/json";
    else if (ext == "woff")
        type = "font/woff";
    else if (ext == "woff2")
        type = "font/woff2";
    else if (ext == "eot")
        type = "application/vnd.ms-fontobject";
    else if (ext == "ttf")
        type = "font/ttf";
    else if (ext == "otf")
        type = "font/otf";
    else
        type = "text/plain";
    return type;
}
int Response::handler(fd_set &r, fd_set &w)
{
    (void)r;
    (void)w;
    std::cout << "rquest : " << _request.Getrequest().at("Path") << std::endl;
    std::string path =  stringtrim(_request.Getrequest().at("Path"));
    std::cout << "path : " << path << std::endl;
    if (defineLocation(path))
    {
        std::string file = check_index(_server.getLocations()[_lindex].getRoot(), _server.getLocations()[_lindex].getIndex());
        if (!this->checkMethod())
        {
            this->_state = DENIEDMETHOD;
            sendResponse(r, w);
        }
        if (file == "")
        {
            if (is_dir(_server.getLocations()[_lindex].getRoot() + path))
            {
                if (check_autoindex(_server.getLocations()[_lindex].getAutoIndex()))
                {
                    std::cout << "autoindex" << std::endl;
                }
                std::cout << "autoindex on" << std::endl;
            }
        }
        else
        {
            std::cout << "file found" << std::endl;
            int fd = open(file.c_str(), O_RDONLY);
            if (fd == -1)
                return (0);
            char *buf = (char *)malloc(sizeof(char) * 1000000);
            int ret = read(fd, buf, 1000000);
            if (ret == -1)
                return (0);
            std::string resp = "HTTP/1.1 200 OK\r\n";
            resp += "Content-Type: text/html\r\n";
            resp += "Content-Length: " + std::to_string(ret) + "\r\n";
            resp += "Connection: close\r\n";
            resp += "\r\n";
            resp += buf;
            send(_ClientFD, resp.c_str(), resp.size(), 0);
            free(buf);
            close(fd);
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            done = 1;
            return (1);
        }
    }
    else
    {
        std::cout << "search : " << stringtrim(_server.getRoot() + stringtrim(_request.Getrequest().at("Path"))) << std::endl;
        if (is_file(stringtrim(_server.getRoot()) + stringtrim(_request.Getrequest().at("Path"))))
        {
            std::cout << "DBG : " << _server.getLocations()[0].getRoot() + path << std::endl;
            std::cout << "file found" << std::endl;
            int fd = open((_server.getLocations()[0].getRoot() + path).c_str(), O_RDONLY);
            if (fd == -1)
                return (0);
            char *buf = (char *)malloc(sizeof(char) * 1000000);
            int ret = read(fd, buf, 1000000);
            if (ret == -1)
                return (0);
            std::string resp = "HTTP/1.1 200 OK\r\n";
            // defile type
            resp += "Content-Type: " + defineType(path) + "\r\n";
            resp += "Content-Length: " + std::to_string(ret) + "\r\n";
            resp += "Connection: close\r\n";
            resp += "\r\n";
            resp += buf;
            send(_ClientFD, resp.c_str(), resp.size(), 0);
            free(buf);
            close(fd);
            done = 1;
            FD_CLR(_ClientFD, &w);
            FD_SET(_ClientFD, &r);
            return (1);
        }
        else if (is_dir(_server.getLocations()[0].getRoot() + path))
        {
            if (check_autoindex(_server.getLocations()[0].getAutoIndex()))
            {
                //send response
                std::string resp = generate_autoindex(_server.getLocations()[0].getRoot() + path , path);
                std::string resp2 = "HTTP/1.1 200 OK\r\n";
                resp2 += "Content-Type: text/html\r\n";
                resp2 += "Content-Length: " + std::to_string(resp.size()) + "\r\n";
                resp2 += "Connection: close\r\n";
                resp2 += "\r\n";
                resp2 += resp;
                send(_ClientFD, resp2.c_str(), resp2.size(), 0);
                done = 1;
                FD_CLR(_ClientFD, &w);
                FD_SET(_ClientFD, &r);
            }
        }
        else
        {
            this->_state = NOTFOUND;
            std::cout << "file not found" << std::endl;
            sendResponse(r, w);
            done = 1;
            return (1);
        }
    }
    return (1);
}

int Response::is_Valide(fd_set &r, fd_set &w)
{

    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if (Method != "GET" && Method != "POST" && Method != "PUT" && Method != "PATCH" && Method != "DELETE" && Method != "COPY" && Method != "HEAD" && Method != "OPTIONS" && Method != "LINK" && Method != "UNLINK" && Method != "PURGE" && Method != "LOCK" && Method != "UNLOCK" && Method != "PROPFIND" && Method != "VIEW" && Version != "HTTP/1.1" && Version != "HTTP/1.0" && Version != "HTTP/2.0" && Version != "HTTP/3.0")
    {
        std::string message = (char *)"HTTP/1.1 400 \r\nConnection: close\r\nContent-Length: 75\r\n\r\n<!DOCTYPE html><head><title>Bad Request</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}
int Response::is_Unauthorize(fd_set &r, fd_set &w)
{
    std::string Method = _request.Getrequest().at("Method");
    std::string Version = _request.Getrequest().at("Version");
    if ((Method != "GET" && Method != "POST" && Method != "DELETE"))
    {
        std::string message = (char *)"HTTP/1.1 501 \r\nConnection: close\r\nContent-Length: 79\r\n\r\n<!DOCTYPE html><head><title>Not Implemented</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    if ((Version != "HTTP/1.1" && Version != "HTTP/1.0"))
    {
        std::string message = (char *)"HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 90\r\n\r\n<!DOCTYPE html><head><title>HTTP Version Not Supported</title></head><body> </body></html>";
        send(_ClientFD, message.c_str(), message.size(), 0);
        FD_CLR(_ClientFD, &w);
        FD_SET(_ClientFD, &r);
        done = 1;
        return 0;
    }
    return 1;
}
