#include <iostream>
   std::string delete_space(std::string str)
    {   
        for(int i = 0;  str[i] && str[i] == ' '; i++)
            if (str[i] == ' ')
            {
                str.erase(i,1);
                i--;
            }
        for(int i = str.size()-1; i > 0 && str[i] == ' '; i--)
            if (str[i] == ' ')
                str.erase(i,1);
        return str;
    }
int main (int ac , char **av)
{
    std::string tmp = delete_space(av[1]);
    std::cout << "|"<< tmp << "|" << std::endl; 
}