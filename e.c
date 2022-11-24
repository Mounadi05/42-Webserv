#include <stdio.h>

int a[]= {1,2,3,4,4,3,2,1,9,7,8,9,7};

int main ()
{
	int i = 0;
	int c = -1;       
	while(++c < 13)
		i ^=a[c];
	printf("%d\n",i);
}
