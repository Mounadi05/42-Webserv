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
 #include <iostream>
#include <utility>
#include "Client.hpp"
#include "Request.hpp"
#include <map>
#include "Response.hpp"
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <algorithm>
#include <cstring>
#include <sys/wait.h>
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */

#endif