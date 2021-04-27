#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constant.h"
#include "table.h"
#include "utilities.h"


static struct symbol* table = NULL;
static int cur_entry =0;

static struct databuf* data = NULL;
static int curbuf =0;
/* initialize array to hold symbols/label*/
void tableinit()
{
    if (table == NULL)
    {
       table = (struct symbol*)(malloc(MAX_SYM*sizeof(struct symbol)));
    }
}
/* release the table once done with parsing */
void tablerelease()
{
    int i;
    if (table != NULL)
    {
        for(i= 0 ; i < cur_entry; i++)
        {
            free(table[i].label);
        }
        free(table);
        table = NULL;
    }
}

/* read the labels and store them in symbol table*/
int readsymbols(FILE* fp)
{
    char buf[BUF_SIZE] = "";
    int datastart = 0;
    int pc = 0;
	while (readline(fp, buf)!= -1)
    {
        if (buf[0] == '\0' || buf[0] == '\n')
            continue;

        if (findtoken( buf, '#') != -1)
            continue;

        if (findtoken(buf, ':') == -1)
        {
            if (strstr(buf, ".text") != NULL)
                continue;
            if (strstr(buf, ".data") != NULL)
            {
                datastart = 1;
                pc =  pc + 100;
                continue;
            }

            if (datastart == 1)
            {
                readdata(buf, &pc);
                pc = pc + 1;
                continue;
            }
            else
            {
                //increment PC for actual instruction lines
                pc = pc + 1;
                if (strstr(buf, "la") != NULL)pc = pc + 1;
                continue;
            }
        }

        if (write2table(buf, &pc) == 0)
        {
            return 0;
        }
    }
    return 1;
}
/* store the data strings here*/
void datainit()
{
    if (data == NULL)
    {
       data = (struct databuf*)(malloc(MAX_BUF*sizeof(struct databuf)));
    }
}
/* release the table once done with parsing */
void datarelease()
{
    if (data != NULL)
    {
        free(data);
        data = NULL;
    }
}

void readdata(char* buf, int* address)
{
    int pos = 0;
    int curaddr = *address;
    char str[BUF_SIZE] ="";

    strcpy(str, buf+9);

    while (write2data(str, pos, &curaddr) != 1)
    {
        curaddr =  curaddr + 1;
        pos = pos + 2;
    }
    *address = curaddr;
}

int write2data(char* buf, int pos, int* addr)
{
    int ret = 0;
    datainit();

    if (buf[pos] == '"')
    {
        /*
            add extra null characters for padding to get the
            word boundary
        */
        data[curbuf].str[0] = '\0' ;
        data[curbuf].str[1] = '\0' ;
        data[curbuf].address = *addr;
        curbuf++;
        data[curbuf].str[0] = '\0';
        data[curbuf].str[1] = '\0';
        data[curbuf].address = ++(*addr);
        ret = 1;
    }
    else if (buf[pos + 1] == '"')
    {
         /*
            add extra null characters for padding to get the
            word boundary
        */
        data[curbuf].str[0] = buf[pos];
        data[curbuf].str[1] = '\0';
        data[curbuf].address = *addr;
        curbuf++;
        data[curbuf].str[0] = '\0';
        data[curbuf].str[1] = '\0';
        data[curbuf].address = ++(*addr);
        ret = 1;
    }
    else if (buf[pos] == '\0')
    {
        return 1;
    }
    else
    {
        data[curbuf].str[0] = buf[pos];
        data[curbuf].str[1] = buf[pos+1];
        data[curbuf].address = *addr;
        ret = 0;
    }
    curbuf++;
    return ret;
}
/* put the data in mif file in little endian order */
void processdata(FILE* fp)
{
    char line[BUF_SIZE] = "";
    int i;
    uint16_t result;
    uint16_t hibuf, lobuf;
    if (fp == NULL) return;


    if (data != NULL)
    {
        for(i= 0 ; i < curbuf; i++)
        {
           hibuf =  data[i].str[1];
           lobuf = data[i].str[0];
           hibuf = hibuf << 8;
           result = hibuf + lobuf;
           /*if (
               data[i].str[1] == '\n' ||
               data[i].str[0] == '\n' ||
               data[i].str[0] == '\0' ||
               data[i].str[1] == '\0'
               )
                sprintf(line,"%02x: %d\n",data[i].address, result);
          else
                sprintf(line,"%02x: %d -- %c%c\n",data[i].address, result, data[i].str[1], data[i].str[0]);
          */
          sprintf(line,"%02x: %02x;\n",data[i].address, result);
          fputs(line, fp);
        }
    }

}

/* add labels to symbol table */
int write2table(char* buf, int* addr)
{
    int pos = 0;
    char temp[BUF_SIZE] ="";
    if (cur_entry == 100)
    {
       return 0;
    }
    strcpy(temp, buf);
    trim(temp);

    pos = findtoken(temp, ':');
    temp[pos] = '\0';

    tableinit();
    table[cur_entry].label = strdup(temp);
    table[cur_entry].address =2 *(*addr);
    cur_entry++;

    return 1;
}
int findsymbol(char* label, int* address)
{
    int i;
    char s1[LABEL_SZ] = "";
    char s2[LABEL_SZ] = "";

    strcpy(s1, label);
    stoupper(s1);

    for (i = 0; i < cur_entry; i++)
    {
        strcpy(s2, table[i].label);
        stoupper(s2);
        if (strcmp(s1, s2) == 0)
        {
            *address = table[i].address;
            return 0;
        }
    }
    return -1;
}
