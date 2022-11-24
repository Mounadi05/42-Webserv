#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
class Request
{
private:
    std::map<std::string, std::string> request;
    int header;
    int first_line;
    int done;
    int _length;
    int body_length;
    int fd;
    int finished;
    int send;
    int size;
    int connection;
public:
    std::string buffer;
    char *body;
    Request();
    ~Request();
    Request(const Request &req);
    Request &operator=(const Request &req);
    std::map<std::string, std::string> &Getrequest(void);
    int &Getheader(void);
    // int &Getstatus_code(void);
    int &Getfirst_line(void);
    void valid_request(std::string str);
    void check_request(char *tmp);
    void handle_request(char *str);
    int &getLength(void);
    void setLength(int l);
    std::string get_header(std::string str);
    void get_body(char *str);
    void write_body(char *str);
    void open_file(void);
    void delete_space(std::string &str);
    int &getFinished();
    int &get_send(void);
    int &get_size(void);
    void init_map(void);
    void handel_host_port(void);

};
#endif