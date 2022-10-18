#include "Request.hpp" 
    Request::Request()
    {
        lent = 0;
        header = 0;
        first_line = 0;
        status_code = 0;
    }
    Request::~Request()
    {
    }
    std::map <std::string,std::string> &Request::Getrequest(void)
    {
        return request;
    }
    int &Request::Getheader(void)
    {
        return header;
    }
    int &Request::Getstatus_code(void)
    {
        return status_code;
    }
    int &Request::Getfirst_line(void)
    {
        return first_line;
    }
    void Request::valid_request(std::string str)
    {
        int index = 0;
        int delemiter = 0;
        std::string tmp;
        delemiter = str.find(" ",index);
        tmp = str.substr(index,delemiter);
        request.insert(std::pair<std::string ,std::string>("Methode",tmp));
        index =  delemiter;
        delemiter = str.find(" ",index+1);
        tmp = str.substr(index,delemiter - index);
        request.insert(std::pair<std::string ,std::string>("Path",tmp));
        index =  delemiter+1;
        delemiter = str.find("\r\n",index+1);
        tmp = str.substr(index,delemiter - index);
        request.insert(std::pair<std::string ,std::string>("Version",tmp));
        if ((request.at("Method") == "GET" || request.at("Method") == "POST" || request.at("Method") == "DELETE") && (request.at("Version") == "HTTP/1.1" || request.at("Version") == "HTTP/1.0"))
            first_line = 1;
        else
            status_code = 400;
    }
    void Request::check_request(char *tmp)
    {
        std::string check = tmp;
        if (!first_line)
        {
            if (check.find("\r\n\r\n",0) != -1)
            {
                header = 1;
                valid_request(check);
                buffer +=check;
            }
            else if (check.find("\r\n",0) != -1)
            {
                valid_request(check);
                buffer += check;
            }
            else
                status_code = 400;
        }
        else if (check.find("\r\n\r\n",0) != -1)
        {
            buffer += check;
            header = 1;
        }
        else
            buffer += check;
    }
    void Request::handle_request(char *str)
    {
        int index = 0;
        std::string delemiter = "\r\n";
        std::string last = "\r\n\r\n";
        std::string value;
        int hold = 0;
        int i = 0;
        check_request(str);
        if (header && first_line)
        {
            index = buffer.find(delemiter,index) + 2;
                do{ 
                    hold = buffer.find(delemiter,index);
                    value = buffer.substr(index,hold-index);
                    i = value.find(":",0);
                    request.insert(std::pair<std::string ,std::string>(value.substr(0,i),value.substr(i+1,value.size()-i)));
                    value.clear();
                    index = hold+2;
                }while(buffer.substr(buffer.find(delemiter,index-2),buffer.size()) != last);
        }
    }