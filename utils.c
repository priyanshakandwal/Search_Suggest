//utils.c

#include "utils.h"
#include<string.h>
//Comparing two strings
int string_compare(const char *str1,const char *str2)
{
    return strcmp(str1,str2);
}
//check if string starts with a prefix
bool string_starts(const char *str1,const char *prefix)
{
    while(*prefix)
    {
        if(*prefix!=*str1)
        return false;
        prefix++;
        str1++;
    }
    return true;
}
//Convert string to lower case
void to_lowercase(char *str)
{
    while(*str)
    {
        if(*str>='A'&& *str<='Z')
        {
            *str+=32;
        }
        str++;
    }
}
//Removing unwanted symbols
void remove_symbols(char *str)
{
   int i=0,j=0;
   while(str[i])
   {
        {
            if((str[i]>='A' && str[i]<='Z') || (str[i]>='a' && str[i]<='z') || (str[i]>='0' && str[i]<='9'))
            str[j++]=str[i];
        }
    i++;
   }
   str[j]='\0';
}


