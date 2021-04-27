#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utilities.h"
#include "constant.h"

/* read the file line by line */
int readline(FILE* fp, char* line)
{
    int c;
    int index =0;

    if ((c = getc(fp)) == EOF)
        return -1;

    if (c == '\0')
    {
        line[index] = c;
        return 0;
    }

    do
    {
        if (c == '\n') break;
        line[index++] = c;
    }while((c = getc(fp)) != '\n' && c != EOF);

    line[index] = '\0';
    return 0;
}

void getsubstring(char* str,int start,int last, char* res)
{
    int i =0, pos =0;
    if (start == -1 || last == -1)
        return;
    if (last < start)
        return;

    for(i = start; i <= last; i++)
        res[pos++] = str[i];


}

/* trim the white spaces string*/
void trim(char* buf)
{
    char tmp[BUF_SIZE] = "";
    int pos = 0, i =0;
    if (buf[0] == '\0') return;

    while(buf[pos] != '\0')
    {
        if (buf[pos] == ' ' || buf[pos] == '\t')
        {
            pos++;
            continue;
        }
        tmp[i] = buf[pos];
        i++;
        pos++;
    }
    tmp[i] ='\0';
    memset(buf,0, BUF_SIZE);
    strcpy(buf, tmp);
}

int findtoken(char* buf, char token)
{
    int index = 0;

    while (buf[index] != '\0')
    {
        if (buf[index] == token)
            return index;
        index++;
    }
    return -1;
}


void stoupper(char* s)
{
   while (*s)
   {
       *s = toupper(*s);
       s++;
   }
}
