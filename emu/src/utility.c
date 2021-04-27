#include<stdlib.h>
#include <stdio.h>
#include <string.h>

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
        if (c == ';') break;
        if (c == '\n' ) break;
        if (c == '\r' ) continue;
        line[index++] = c;
    }while((c = getc(fp)) != '\n' && c != EOF);

    line[index] = '\0';
    return 0;
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
