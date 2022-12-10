#include "../../includes/Webserv.hpp"
#include <math.h>
Request::Request()
{
    _length = 0;
    header = 0;
    first_line = 0;
    done = 0;
    body_length = 0;
    fd = 0;
    finished = 0;
    size = 0;
    send = 0;
    connection = 0;
    chunked = 0;
    full = 0;
    ok = 0;
    lent_chunked = 0;
    skip = 0;
}
Request::~Request()
{
}
std::map<std::string, std::string> &Request::Getrequest(void)
{
    return request;
}
std::string Request::send_body(void)
{
    return body;
}
u_int64_t &Request::GetLent(void)
{
    return full;  
}
int &Request::Getheader(void)
{
    return header;
}

int &Request::Getfirst_line(void)
{
    return first_line;
}
int &Request::getLength(void)
{
    return _length;
}
void Request::setLength(int length)
{
    _length = length;
}
void Request::init_map()
{
    request.insert(std::pair<std::string, std::string>("Version", ""));
    request.insert(std::pair<std::string, std::string>("Path", ""));
    request.insert(std::pair<std::string, std::string>("Method", ""));
    request.insert(std::pair<std::string, std::string>("Host", ""));
    request.insert(std::pair<std::string, std::string>("Content-Length", ""));
}

int &Request::get_send(void)
{
        return send;
}
// int &Request::get_size(void)
// {
//         return size;
// }
int &Request::check_chunked(void)
{
    return chunked;
}
void Request::get_body(char *str)
{
    std::string tmp = str;
    int i = tmp.find("\r\n\r\n", 0) + 4;
    int a = 0;
    while (i < _length)
    {
        body.push_back(str[i++]);
        a++;
    }
    body_length = a;
}

void Request::handel_host_port(void)
{   
    request.insert(std::pair<std::string, std::string>("Port",""));
    std::string tmp = request.at("Host");
    int index = tmp.find(":",0);
    if(index != -1)
    {
        request.at("Host") = tmp.substr(0,index);
        request.at("Port") = tmp.substr(index+1,4);
    }
}


std::string Request::get_tmp()
{
    return path_tmp;
}
std::string delete_space(std::string str)
{   
    for(int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i,1);
    return str;
}
int last_slash(std::string tmp)
{
    int a = 0;
    int i = 0;
    a = i = tmp.find("/",0);
    while(i != -1)
    {
        a = i;
        i = tmp.find("/",i+1);
    }
    return a;
}
void Request::open_file()
{
    path_tmp = "tmp"+ delete_space(request.at("Path").substr(last_slash(request.at("Path"))));
    fd = open(path_tmp.c_str(),O_CREAT|O_RDWR,0777);
}

int &Request::getFinished()
{
    return finished;
}

Request::Request(const Request &req)
{
    *this = req;
}

Request &Request::operator=(const Request &req)
{
    if (this != &req)
    {
        this->body = req.body;
        this->body_length = req.body_length;
        this->fd = req.fd;
        this->header = req.header;
        this->_length = req._length;
        this->finished = req.finished;
        this->request = req.request;
        this->first_line = req.first_line;
        this->done = req.done;
        this->connection = req.connection;
        this->chunked = req.chunked;
        this->full = req.full;
        this->send = req.send;
        this->size = req.size;
        this->ok = req.ok;
        this->path_tmp = req.path_tmp;
        this->lent_chunked = req.lent_chunked;
        this->skip =  req.skip;
    }
    return *this;
}

void Request::write_body()
{
    int a = write(fd,body.c_str(),body.length());
    full += a;
    if (full >= size)
    {
        close(fd);
        finished = 1;
        ok = 0;
    }
}
int hex_dec(char *str)
{
     int i = 0, val, len;
    int n = 0;
     len = strlen(str) - 1;
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
            val = str[i] - 48;
        else if (str[i] >= 'a' && str[i] <= 'f') 
            val = str[i] - 97 + 10;
        else if (str[i] >= 'A'&& str[i] <= 'F') 
            val = str[i] - 65 + 10;
        n += val * pow(16, len);
        len--;
        i++;
    }
    return n;
}
int Request::get_Lchuncked(std::string str)
{
    std::string tmp;
    for(int i = 0 ; i < (int)str.length();i++)
    {
        if (str[i] == '\r' && str[i+1] == '\n')
        {
            i +=2;
            skip = 2;
            while(i < (int)str.length() && str[i] != '\r' && str[i+1] != '\n')
            {
                tmp.push_back(str[i++]);
                skip++;
            }
            if(!tmp.empty())
            {
                skip +=2; 
                return hex_dec((char *)tmp.c_str());
            }
            else
                skip = 0;
        }
    }
    return 0;
}

void Request::transfer_chunked()
{
    char u;
    std::string tmp;
    int a = 0;
    (void) a;
    if (lent_chunked > (int)body.length())
    {
        write(fd,body.c_str(),body.length());
        lent_chunked -= (int)body.length();
    }
    else
    {
        if (lent_chunked == 0)
        {
            lent_chunked = get_Lchuncked(body);
            if(lent_chunked == 0)
            {
                close(fd);
                finished = 1;
                ok = 0;
            }
            else
                body.erase(0,skip);
        }
        for(int i = 0 ; !finished && i < (int)body.length() && lent_chunked;i++)
        {
            if (lent_chunked)
            {
                u = body[i];
                write(fd,&u,1);
            }
            lent_chunked--;
            if (lent_chunked == 0)
            {

                lent_chunked = get_Lchuncked(body.substr(i));
                a = body.length() - i;
                if (lent_chunked == 0)
                {
                    finished = 1;
                    close(fd);
                    ok = 0;
                }
                i += skip;
            }
        }
    }
}
void Request::handle_request(char *str)
{   
     int index = 0;
    std::string delemiter = "\r\n";
    std::string last = "\r\n\r\n";
    std::string value;
    std::string check = str;
    int hold = 0;
    int i = 0;
    if (ok)
    {
        body_length = _length;
        std::string tmp (str,body_length);
        body = tmp;
        if(chunked)
            transfer_chunked();
        else
            write_body();
    }
    if (!finished && !ok)
    {
        init_map();
        check_request(str);
        if (first_line)
        {
            index = buffer.find(delemiter, index) + 2;
            do
            {
                hold = buffer.find(delemiter, index);
                value = buffer.substr(index, hold - index);
                if ((int)value.find("Connection",0) != -1)
                    connection = 1;
                if ((int)value.find("chunked",0) != -1)
                {
                     chunked = 1;
                }
                i = value.find(":", 0);
                if ((int)value.find("Content-Length") != -1)
                    request.at("Content-Length") = value.substr(i + 1, value.size() - i);
                if ((int)value.find("Host") != -1)
                    request.at("Host") = value.substr(i + 1, value.size() - i);
                else
                    request.insert(std::pair<std::string, std::string>(value.substr(0, i), value.substr(i + 1, value.size() - i)));
                value.clear();
                index = hold + 2;            } while (buffer.substr(buffer.find(delemiter, index - 2), buffer.size()) != last);
        }
        finished = 1;
        if (!connection)
            request.insert(std::pair<std::string, std::string>("Connection","close"));
        handel_host_port();
        if (request.at("Method") == "POST" && (!request.at("Content-Length").empty() || chunked  == 1))
        {
            open_file();
            size = strtoull(request.at("Content-Length").c_str(),NULL, 10);
            ok = 1;
            finished = 0;
            if (chunked)
            {
                body = delemiter + body;
                 transfer_chunked();
             }
            else
                write_body();
        }
     }
  }

void Request::check_request(char *tmp)
{
    std::string check = tmp;
    if ((int)check.find("\r\n\r\n", 0) != -1)
    {
        header = 1;
        buffer = get_header(check);
        valid_request(buffer);
        get_body(tmp);
        first_line = 1;
    }
}

void Request::valid_request(std::string str)
{
    int index = 0;
    int delemiter = 0;
    std::string tmp;
    delemiter = str.find(" ", index);
    tmp = str.substr(index, delemiter);
    request.at("Method")= tmp;
    index = delemiter;
    delemiter = str.find(" ", index + 1);
    tmp = str.substr(index, delemiter - index);
    request.at("Path")= tmp;
    index = delemiter + 1;
    delemiter = str.find("\r\n", index + 1);
    tmp = str.substr(index, delemiter - index);
    request.at("Version")= tmp;
    if (!(request.at("Method") == "GET" || request.at("Method") == "POST" || request.at("Method") == "DELETE") && (request.at("Version") == "HTTP/1.1" || request.at("Version") == "HTTP/1.0"))
        first_line = 1;
}

std::string Request::get_header(std::string str)
{
    std::string tmp;
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '\r' && str[i + 1] == '\n' && str[i + 2] == '\r' && str[i + 3] == '\n')
        {
            tmp += "\r\n\r\n";
            return tmp;
        }
        tmp.push_back(str[i]);
    }
    return str;
}
int Request::get_ok()
{
    return ok;
}