#ifndef CGI_HPP
#define CGI_HPP

# include <string>

class Cgi
{
    private:
        Request                     request;
        std::vector<std::string>    enVariables;
        char                        **envp;
        char                        *args[3];
        std::string                 scriptPath;
        std::string                 filePath;
        int                         outputFile;
        int                         inputFile;
    public:
        Cgi(std::string scriptPath, std::string  filePath, Request request);
        ~Cgi();
        void setEnvironements();
        void executeCgi();
};
#endif