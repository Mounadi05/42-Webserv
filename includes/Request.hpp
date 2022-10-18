#ifndef REQUEST_HPP
#define REQUEST_HPP

 #include <iostream>
#include <map>
class Request{
    private:
        std::map <std::string,std::string> request;
        int header;
        int first_line;
        int status_code;  
        int lent;
    public:
        std::string buffer;
        Request();
        ~Request();
        std::map <std::string,std::string> &Getrequest(void);
        int &Getheader(void);
        int &Getstatus_code(void);
        int &Getfirst_line(void);
        void valid_request(std::string str);
        void check_request(char *tmp);
        void handle_request(char *str);
};
#endif