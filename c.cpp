#include <iostream>	
#include <math.h>
int hex_dec(char *str)
{
    int decimalnumber, place;
    int i = 0, val, len;
    int n = 0;
  
    len = strlen(str);
    len--;
  
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
            val = str[i] - 48;
        else if (str[i] >= 'a' && str[i] <= 'f') 
            val = str[i] - 97 + 10;
        else if (str[i] >= 'A'&& str[i] <= 'F') 
            val = str[i] - 65 + 10;
        n += val * pow(16, len);
        len--;
        i++;
    }
    printf("%d\n",n);
    return n;
}

int main (int ac , char **av)
{
    hex_dec(av[1]);
}
 