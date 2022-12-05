#include "../../includes/Webserv.hpp"
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
int &Request::GetLent(void)
{
    return body_length;  
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
    full += a;
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

void Request::delete_space(std::string &str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(str[i], 1);
}

void Request::open_file()
{
    std::string tmp = request.at("Content-Type").substr(request.at("Content-Type").find("/", 0) + 1, request.at("Content-Type").size());
    std::string path1 = request.at("Path").substr(request.at("Path").find("/", 0) + 1, request.at("Path").size());
    delete_space(tmp);
    std::string path = (char *)"www/upload/";
    delete_space(path1);
    path += path1 + "." + tmp;
    fd = open(path.c_str(), O_CREAT | O_RDWR, 0644);
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
    }
    return *this;
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
    // if (finished && request.at("Method") == "POST")
    // {
    //     body_length = _length;
    //     std::string tmp (str,body_length);
    //     body = tmp;
    //     std::cout << "****** > " << std::endl;
    //     write(1,body.c_str(),_length);
    //     std::cout << "****** > " << std::endl;
    //     if (full >= size)
    //         ok = 1;
    // }
    if (!finished)
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
                    chunked = 1;
                i = value.find(":", 0);
                if ((int)value.find("Host") != -1)
                    request.at("Host") = value.substr(i + 1, value.size() - i);
                else
                    request.insert(std::pair<std::string, std::string>(value.substr(0, i), value.substr(i + 1, value.size() - i)));
                value.clear();
                index = hold + 2;            } while (buffer.substr(buffer.find(delemiter, index - 2), buffer.size()) != last);
        }
        finished = 1;
        ok = 1;
        if (!connection)
            request.insert(std::pair<std::string, std::string>("Connection","close"));
        handel_host_port();
        if (request.at("Method") == "POST")
        {size = strtoull(request.at("Content-Length").c_str(),NULL, 10);
            ok = 0;};
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

int Request::get_ok(void)
{
    return ok;
}