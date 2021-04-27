#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include "inttypes.h"
struct symbol
{
    char* label;
    int address;
};

struct databuf
{
    char str[2];
    int address;
};
/*
struct databuf
{
    char str;
    int address;
};
*/
void tableinit();
void tablerelease();
int readsymbols(FILE* );
int write2table(char* , int*);
int findsymbol(char* , int* );
void readdata(char* , int* );
void processdata(FILE* );
int write2data(char* , int , int* );
#endif // TABLE_H_INCLUDED
