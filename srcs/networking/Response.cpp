#include "../../includes/Webserv.hpp"
#include <utility>

void Response::initStatusMessages()
{
    this->_statusCodeMsg.insert( std::pair<int, std::string> (503, "Service Unavailable") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (505, "Version Not Supported") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (414, "URI too long") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (431, "Request Header Fields Too Long") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (501, "Not Implemented") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (500, "internal Server Error") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (405, "Method Not Allowed") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (401, "Unothorized") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (100, "Continue") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (413, "Payload Too Large") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (415, "Unsupported Media Type") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (400, "Bad Request") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (403, "Forbidden") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (200, "OK") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (308, "Permanent Redirect") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (307, "Temporary Redirect") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (404, "Not Found") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (202, "Accepted") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (201, "Created") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (304, "Not Modified") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (204, "No Content") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (300, "Multiple Choices") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (303, "See Other") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (409, "Conflict") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (412, "Precondition Failed") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (410, "Gone") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (406, "Not Acceptable") );
    this->_statusCodeMsg.insert( std::pair<int, std::string> (417, "Expectation Failed") );
}

void Response::initMimeTypes()
{
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("aac", "audio/aac"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("abw", "application/x-abiworld"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("arc", "application/x-freearc"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("avi", "video/x-msvideo"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("bin", "application/octet-stream"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("bmp", "image/bmp"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("bz", "application/x-bzip"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("bz2", "application/x-bzip2"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("csh", "application/x-csh"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("css", "text/css"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("csv", "text/csv"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("doc", "application/msword"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("eot", "application/vnd.ms-fontobject"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("epub", "application/epub+zip"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("gz", "application/gzip"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("gif", "image/gif"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("htm", "text/html"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("html", "text/html"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ico", "image/vnd.microsoft.icon"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ics", "text/calendar"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("jar", "application/java-archive"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("jpeg", "image/jpeg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("jpg", "image/jpeg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("js", "text/javascript"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("json", "application/json"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("jsonld", "application/ld+json"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("mid", "audio/midi"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("midi", "audio/x-midi"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("mjs", "text/javascript"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("mp3", "audio/mpeg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("mpeg", "video/mpeg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("mpkg", "application/vnd.apple.installer+xml"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("odp", "application/vnd.oasis.opendocument.presentation"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ods", "application/vnd.oasis.opendocument.spreadsheet"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("odt", "application/vnd.oasis.opendocument.text"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("oga", "audio/ogg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ogv", "video/ogg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ogx", "application/ogg"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("opus", "audio/opus"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("otf", "font/otf"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("png", "image/png"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("pdf", "application/pdf"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("php", "application/x-httpd-php"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ppt", "application/vnd.ms-powerpoint"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("rar", "application/vnd.rar"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("rtf", "application/rtf"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("sh", "application/x-sh"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("svg", "image/svg+xml"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("swf", "application/x-shockwave-flash"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("tar", "application/x-tar"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("tif", "application/x-tar"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("tiff", "image/tiff"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ts", "video/mp2t"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("ttf", "font/ttf"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("txt", "text/plain"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("vsd", "application/vnd.visio"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("wav", "audio/wav"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("weba", "audio/webm"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("webm", "video/webm"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("webp", "image/webp"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("woff", "font/woff"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("woff2", "font/woff2"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("xhtml", "application/xhtml+xml"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("xls", "application/vnd.ms-excel"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("xml", "application/xml"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("xul", "application/vnd.mozilla.xul+xml"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("zip", "application/zip"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("3gp", "video/3gpp"));
    this->_mimeTypes.push_back(std::make_pair<std::string, std::string> ("3g2", "video/3gpp2"));
}
Response::Response(Request request, Server server, int clientFd)
{
    this->_request = request;
    this->_server = server;

    // init status codes messages
    initStatusMessages();
    // init Mime Types
    initMimeTypes();
    

    // setFirstLine
    this->_version = "HTTP/1.1 ";


    this->_statusCode = std::to_string(this->_request.Getstatus_code()).append(" ");
    this->_codeMessage = this->_statusCodeMsg[this->_request.Getstatus_code()];
    this->_responseString = this->_version + this->_statusCode +this->_codeMessage;
}

Response::Response()
{

}

Response::Response()
{
    
}

Response::~Response()
{

}

std::string & Response::getVersion()
{
    return _version;
}
std::string & Response::getStatusCode()
{
    return _statusCode;
}
std::map<int, std::string> & Response::getStatusCodeMsg()
{
    return _statusCodeMsg;
}

std::map<std::string, std::string> & Response::getHeaders()
{
    return _headers;
}

int                                     Response::isFirstLineFinished()
{
    if (_firstLineDone == true)
        return 1;
    return 0;
}
int                                     Response::isHeadersFinished()
{
    if (_headersDone == true)
        return 1;
    return 0;
}
int                                     Response::isResponseFinished()
{
    ;
}


bool    Response::isMethodImplemented()
{

    if(this->_request.Getrequest().at("Method").compare("GET") != 0 && this->_request.Getrequest().at("Method").compare("POST") != 0 && this->_request.Getrequest().at("Method").compare("DELETE") != 0)
    {
        this->_statusCode = std::to_string(501);
        return false;
    }
    return true;
}

bool    Response::isVersionSupported()
{
    if(this->_request.Getrequest().at("Version").compare("HTTP/1.1") != 0 && this->_request.Getrequest().at("Version").compare("HTTP/1.1") != 0)
    {
        this->_statusCode = std::to_string(505);
        return false;
    }
    return true;
}

void   Response::defineFileType(std::string filePath)
{
    struct stat file;
    if (stat(filePath.c_str(), &file))
    {
        if (file.st_mode & S_IFDIR)
            this->_rightToAccess = S_IFDIR;
        if (file.st_mode & S_IFREG)
            this->_rightToAccess = S_IFREG;
    }
    else
        this->_rightToAccess = -1;
}