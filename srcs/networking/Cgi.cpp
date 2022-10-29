#include "../../../includes/webserv.hpp"

# include <stdlib.h>

Cgi::Cgi(std::string scriptPath, std::string  filePath, Request request)
{
    this->request = request;
    this->filePath = filePath;
    this->scriptPath = scriptPath;

    executeCgi();
}

Cgi::~Cgi()
{
    
}

void Cgi::setEnvironements()
{
        // the following envs are not specific and are set for all requests
    this->enVariables.push_back("SERVER_SOFTWARE=WEBSERVER"); // Custom Given name ??!!
    this->enVariables.push_back("SERVER_NAME=localhost"); // since its our server who asks cgi to handle the resource
    this->enVariables.push_back("GATEWAY_INTERFACE=CGI/1.1");
        // the following environement are specific to the request fulfilled by the gateway program
    this->enVariables.push_back("SERVER_PROTOCOL=HTTP/1.1");
    this->enVariables.push_back("SERVER_PORT=" + this->request.getPort());
    this->enVariables.push_back("REQUEST_METHOD=" + this->request.getMethod());
    this->enVariables.push_back("SCRIPT_NAME=" + this->scriptPath.find_last_of('/'));
        // if query parameters
    this->enVariables.push_back("QUERY_STRING=" + this->request.getQueryParams()) ;
    //this->enVariables.push_back("REMOTE_HOST=" + );
    //this->enVariables.push_back("REMOTE_ADDR=" + );
    //this->enVariables.push_back("AUTH_TYPE=" + );
    //this->enVariables.push_back("REMOTE_USER=" + );
    //this->enVariables.push_back("REMOTE_IDENT=" + );
    this->enVariables.push_back("CONTENT_TYPE=" + this->request.getHeaders().at("Content-Type"));
    this->enVariables.push_back("CONTENT_LENGTH=" + this->request.getHeaders().at("Content-Length"));

    envp = (char **)malloc(sizeof(char *) * this->enVariables.size());
    int i = 0;
    for (std::vector<std::string>::iterator it = this->enVariables.begin(); it!= this->enVariables.end(); it++)
    {
        envp[i] = strdup((*it).c_str());
        i++;
    }
    envp[i] = NULL;
}

void Cgi::executeCgi()
{
    int pid;
    int status;

    setEnvironements();
    this->outputFile = open("./tmp/cgiOutput", O_CREAT | O_RDWR);
    if ((pid = fork()) == -1)
    {
        printf("error forking\n");
        exit(1);
    }
    if (pid == 0)
    {
        this->args[0] = const_cast<char *>(this->scriptPath.c_str());
        this->args[1] = const_cast<char *>(this->filePath.c_str());
        this->args[2] = NULL;
        // dup output of execve to  outputfile
        dup2(this->outputFile, 1);
        // if post request dup input of execve to inputfile
        if (this->request.getMethod() == "POST")
        {
            this->inputFile = open(this->request.getBodyFile().c_str(), O_RDONLY);
            dup2(this->inputFile, 0);
        }
        execve(this->args[0], this->args, envp);
    }
    else
    {
        waitpid(pid, &status, NULL);
        if (WIFEXITED(status))
        {
            ;
        }
        // not yet finished

        // if everything if good after test last step
        close(this->inputFile);
        close(this->outputFile);
        //should free the envp
    }
}
