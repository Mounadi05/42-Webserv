#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>
#include "../../includes/Response.hpp"

#define foldercdn "https://ucarecdn.com/7f99b91a-57cd-480a-ad68-7cf9e9214fb5/folder.gif"
#define filecdn "https://ucarecdn.com/16634eec-a351-428f-ad7f-2895fb636faf/"

#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"
typedef std::vector<std::pair<std::pair<std::string, std::string>, int> > t_dir;
typedef std::pair<std::string, std::string> t_spair;

enum
{
  ISDIR = 1,
  ISFILE = 2
};

std::string getsize(long long st_size)
{
  std::stringstream stream;
  std::string result;
  if (st_size < 1048576)
  {
    stream << std::fixed << std::setprecision(2) << (long double)(st_size) / 1024;
    result = stream.str();
    return result + "K";
  }
  else if (st_size < 524288000)
  {
    stream << std::fixed << std::setprecision(2) << (long double)(st_size) / 1048576;
    result = stream.str();
    return result + "M";
  }
  else
  {
    stream << std::fixed << std::setprecision(2) << (long double)(st_size) / 1073741824;
    result = stream.str();
    return result + "G";
  }
}

t_dir show_dir_content(std::string path)
{
  DIR *d = opendir(path.c_str());
  t_dir ret;

  if (d)
  {
    struct dirent *dir;
    while ((dir = readdir(d)))
    {
      if (dir->d_type != DT_DIR && dir->d_name[0] != '.')
      {
        struct stat stats;

        stat((std::string(path) + "/" + std::string(dir->d_name)).c_str(), &stats);

        t_spair FileAndSize;
        FileAndSize.first = dir->d_name;
        FileAndSize.second = getsize(stats.st_size).c_str();

        t_dir::value_type PairAndType;
        PairAndType.first = FileAndSize;
        PairAndType.second = ISFILE;

        ret.push_back(PairAndType);
      }
      else if (dir->d_type == DT_DIR && dir->d_name[0] != '.')
      {
        struct stat stats;

        stat((std::string(path) + "/" + std::string(dir->d_name)).c_str(), &stats);

        t_spair DirAndSize;
        DirAndSize.first = dir->d_name;
        DirAndSize.second = "-";

        t_dir::value_type PairAndType;
        PairAndType.first = DirAndSize;
        PairAndType.second = ISDIR;

        ret.push_back(PairAndType);
      }
    }
    closedir(d);
  }
  return ret;
}

std::string generate_autoindex(std::string path)
{
  std::string ret = "";
  if (!path.empty())
  {
    t_dir vec = show_dir_content(path);
    ret += "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\"><html><head><title>Index of " + path.substr(1) + "</title></head>";
    ret += "<body><h1>Index of " + path.substr(1) + "</h1>";
    ret += "<table><tr><th valign='top'></th><th><a href='?C=N;O=D'>Name</a></th><th><a href='?C=S;O=A'>Size</a></th></tr><tr>\
            <th colspan='5'><hr></th></tr>";
    for (size_t i = 0; i < vec.size(); i++)
    {
      ret += "<tr>";
      ret += "<td valign='top'><img src='" + ((vec[i].second == ISFILE) ? std::string(filecdn) : std::string(foldercdn)) + "' alt='[DIR]'></td>";
      ret += "<td><a href='" + vec[i].first.first + "'>" + vec[i].first.first + "</a></td>";
      ret += "<td align='right'> " + vec[i].first.second + " </td>";
      ret += "<td>&nbsp;</td>";
    }
    ret += "<tr><th colspan='5'><hr></th></tr></table></body>";
  }
  return ret;
}