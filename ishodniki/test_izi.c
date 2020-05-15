# include <stdio.h>
# include <stdlib.h>
#include <ctype.h>
#include <netinet/in.h>
char *print_upper(char *s)
{
	s[6]='Z';
	return s;
}
int main(void) /* вывод строки в верхнем регистре */
{
	char s[80]="privet zebra";
	char *d;
    char si[]="34910";
    int p;
    p=atoi(si);
    printf("p=%d htons=%d\n",p,htons(p));
	d=print_upper(s);
	printf("%s",d);
	return 0;
}
