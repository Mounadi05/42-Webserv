#ifndef Location_class
# define Location_class


#include <vector>
#include <iostream>

class Location
{
    private:
        std::string                                         _locationNumber;
        std::string                                         _locationPath;
        std::vector<std::string>                            _allowedMethods;
        std::string                                         _root;
        std::string                                         _clientMaxBodySize;
        std::vector<std::string>                            _index;
        std::string                                         _autoIndex;
        std::string                                         _uploadPath;
        std::pair<std::string, std::string>                 _redirection;
    public:
        Location();
        ~Location();

        std::string                                         & getLocationNumber();
        std::string                                         & getLocationPath();
        std::vector<std::string>                            & getAllowedMethods();
        std::string                                         & getRoot();
        std::string                                         & getClientMaxBodySize();
        std::vector<std::string>                            & getIndex();
        std::string                                         & getAutoIndex();
        std::string                                         & getUploadPath();
        std::pair<std::string, std::string>                 & getRedirection();

        void                                                setLocationNumber(std::string locationNumber);
        void                                                setLocationPath(std::string locationPath);
        void                                                setAllowedMethods(std::vector<std::string> allowedMethods);
        void                                                setRoot(std::string root);
        void                                                setClientMaxBodySize(std::string clientMaxBodySize);
        void                                                setIndex(std::vector<std::string> index);
        void                                                setAutoIndex(std::string autoIndex);
        void                                                setUploadPath(std::string uploadPath);
        void                                                setRedirection(std::pair<std::string, std::string> redirection);
        

};

#endif