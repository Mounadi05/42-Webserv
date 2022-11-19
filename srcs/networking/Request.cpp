#include "../../includes/Webserv.hpp"

Request::Request()
{
    _length = 0;
    header = 0;
    first_line = 0;
    done = 0;
    body = new char[1025];
    bzero(body,1024);
    body_length = 0;
    fd = 0;
    finished = 0;
    size = 0;
    send = 0;
    connection = 0;
   
}
Request::~Request()
{
}
std::map<std::string, std::string> &Request::Getrequest(void)
{
    return request;
}
int &Request::Getheader(void)
{
    return header;
}
// int &Request::Getstatus_code(void)
// {
//     return status_code;
// }

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
}
void Request::valid_request(std::string str)
{
    std::string hhh =  str;
    int index = 0;
    int delemiter = 0;
    std::string tmp;
    delemiter = str.find(" ", index);
    tmp = str.substr(index, delemiter);
    request.insert(std::pair<std::string, std::string>("Method", tmp));
    index = delemiter;
    delemiter = str.find(" ", index + 1);
    tmp = str.substr(index, delemiter - index);
    request.insert(std::pair<std::string, std::string>("Path", tmp));
    index = delemiter + 1;
    delemiter = str.find("\r\n", index + 1);
    tmp = str.substr(index, delemiter - index);
    request.insert(std::pair<std::string, std::string>("Version", tmp));
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
int &Request::get_send(void)
{
        return send;
}
int &Request::get_size(void)
{
        return size;
}
void Request::get_body(char *str)
{
    std::string tmp = str;
    int i = tmp.find("\r\n\r\n", 0) + 4;
    int a = 0;
    while (i < _length)
        body[a++] = str[i++];
    body_length = a;
}

void Request::check_request(char *tmp)
{
    std::string check = tmp;
    if (!first_line)
    {
        if ((int)check.find("\r\n\r\n", 0) != -1)
        {
            header = 1;
            buffer = get_header(check);
            valid_request(buffer);
            get_body(tmp);
            first_line = 1;
        }
        else if ((int)check.find("\r\n", 0) != -1)
        {
            valid_request(check);
            buffer += check;
        }
    }
    else if ((int)check.find("\r\n\r\n", 0) != -1)
    {
        buffer += get_header(check);
        // get_body(buffer.c_str());
        header = 1;
    }
    else
        buffer += check;
}

void Request::write_body(char *str)
{
    send +=  body_length;
    write(fd, str, body_length);
    if (send >= size)
    {
        finished = 1;
        std::cout << "done" << std::endl;
        close(fd);
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

void Request::handle_request(char *str)
{
    int index = 0;
    std::string delemiter = "\r\n";
    std::string last = "\r\n\r\n";
    std::string value;
    int hold = 0;
    int i = 0;
    if (!done && !finished)
    {
        check_request(str);
        if (header && first_line && !finished)
        {
            index = buffer.find(delemiter, index) + 2;
            do
            {
                hold = buffer.find(delemiter, index);
                value = buffer.substr(index, hold - index);
                if ((int)value.find("Connection",0) != -1)
                    connection = 1;
                i = value.find(":", 0);
                request.insert(std::pair<std::string, std::string>(value.substr(0, i), value.substr(i + 1, value.size() - i)));
                value.clear();
                index = hold + 2;
                // std::cout << value << std::endl;
            } while (buffer.substr(buffer.find(delemiter, index - 2), buffer.size()) != last);
            finished = 1;
            if (request.at("Method") == "POST")
            {
                finished = 0;
                done = 1;
                size = atoi(request.at("Content-Length").c_str());
                open_file();
                write_body(body);
            }
        }
        if (!connection)
            request.insert(std::pair<std::string, std::string>("Connection","close"));
    }
    else if (done)
    {
        body_length = _length;
        if (request.at("Method") == "POST")
            write_body(str);
    }
    _path = request.at("Path");
    _method = request.at("Method");
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
        // delete this->body;
        this->body = new char[1025];
        // strlcpy(this->body,req.body,body_length - 1);
        this->body_length = req.body_length;
        this->fd = req.fd;
        this->header = req.header;
        this->_length = req._length;
        this->finished = req.finished;
        this->request = req.request;
        this->first_line = req.first_line;
        this->done = req.done;
        this->connection = req.connection;
        this->_path = req._path;
        this->_method = req._method;
    }
    return *this;
}

std::ostream & operator<<(std::ostream & out ,Request & other){
    out << "========== request begin =============" << std::endl;
    out << "headers : " << std::endl;
    for (std::map<std::string, std::string>::iterator  it = other.Getrequest().begin() ; it != other.Getrequest().end() ; it++)
        out << it->first << ":" << it->second << std::endl;
    if ( other.Getrequest().at("Method")== "POST")
    {
        out << "body : " << std::endl;
        out << std::string(other.body,other.getLength()) << std::endl;
    }
    out << "========== request end =============" << std::endl;
    return out;
}

std::string Request::getPath() const{
    return this->_path;
}
std::string Request::getMethod() const{
    return this->_method;
}