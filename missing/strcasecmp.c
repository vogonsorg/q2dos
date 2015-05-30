#include <stdio.h>
#include <string.h>

int strcasecmp ( char *s1, char *s2)
{
int rc;
int len;
len=strlen(s1);
rc=strnicmp(s1,s2,len);
return(rc);
}

int strncasecmp(char *s1, char *s2)
{
return(strcasecmp(s1,s2));
}