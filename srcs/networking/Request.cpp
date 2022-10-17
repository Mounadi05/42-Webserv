#include "../../includes/Webserv.hpp"

Request::Request()
{

}

Request::~Request()
{

}
Request::Request(std::string Buffer){
    this->_Rbuffer = Buffer;
}

std::string & Request::getRequestBuffer(){
    return this->_Rbuffer;
}

void Request::setRequestBuffer(std::string Buffer)
{
    this->_Rbuffer = Buffer;
}

// void Request::ConcatToBuffer(std::string newBuffer);
// {

// }

// int Request::checkDelemiter(std::string Buffer){

//     if (Buffer.find("\r\n") != std::string::npos)
//     {
//         _delemiter = '\r';
//     }
//     else
//         _delemiter = '\n';
// }