#ifndef Webserv_class
#define Webserv_class

#include "Config.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "IOMultiplexing.hpp"
#include <fstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include "Client.hpp"
#include "Request.hpp"
#include <map>
#include "Response.hpp"

enum e_state {
    INIT = 1,
    BEGIN,
    PEOCESSING,
    END,
    ERROR,
    DENIEDMETHOD = 405,
    NOTFOUND = 404,
    UNAUTHORIZED = 401
};

#endif