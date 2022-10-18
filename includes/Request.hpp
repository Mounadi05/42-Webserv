 #ifndef Request_class
#define Request_class

 #include <iostream>
#include <map>
class Request{
    private:
        std::map <std::string,std::string> request;
        int header;
        int first_line;
        int status_code;  
        std::string buffer;
        int lent;
    public:
      
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