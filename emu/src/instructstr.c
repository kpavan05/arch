#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructstr.h"

void do_str(char* s, struct instruction* instr)
{
    char rgdest[MAX_RGSTR] = "";
    char rgsrc[MAX_RGSTR] = "";
    char rgjmp[MAX_RGSTR] = "";
    int len = 0;
    get_register(rgdest, instr->dest);
    get_register(rgsrc, instr->source);

    memset(s, ' ', BUFSIZ);
    switch(instr->opcode)
    {
    case 0:
        break;
    case 1:
        sprintf(s, "%s %s,%d","addi",rgdest, instr->immediate);
        break;
    case 2:
        sprintf(s, "%s %s,%d","ori",rgdest, instr->immediate);
        break;
    case 3:
        sprintf(s, "%s %s,%d","lui",rgdest, instr->immediate);
        break;
    case 4:
        sprintf(s, "%s %s,%s","lw",rgdest, rgsrc);
        break;
    case 5:
        sprintf(s, "%s %s,%s","sw",rgdest, rgsrc);
        break;
    case 6:
        sprintf(s, "%s %s,%s","lb",rgdest, rgsrc);
        break;
    case 7:
        sprintf(s, "%s %s,%s","sb",rgdest, rgsrc);
        break;
    case 8:
        get_register(rgjmp, instr->jump);
        sprintf(s, "%s %s,%s,%s","jreq",rgdest, rgsrc, rgjmp);
        break;
    case 9:
        get_register(rgjmp, instr->jump);
        sprintf(s, "%s %s,%s,%s","jrne",rgdest, rgsrc, rgjmp);
        break;
    case 10:
        sprintf(s, "%s %d","j",instr->immediate);
        break;
    case 11:
        sprintf(s, "%s %d","jal",instr->immediate);
        break;
    default:
        break;
    }
    len = strlen(s);
    s[len]='\0';
}

void do_rtypestr(char* s, struct instruction* instr, int func)
{
    char rgdest[MAX_RGSTR] = "";
    char rgsrc[MAX_RGSTR] = "";
    int len = 0;
    get_register(rgdest, instr->dest);
    get_register(rgsrc, instr->source);
    memset(s, ' ', BUFSIZ);
    switch(func)
    {
    case 0:
        sprintf(s, "%s %s,%s","add",rgdest, rgsrc);
        break;
    case 1:
        sprintf(s, "%s %s,%s","sub",rgdest, rgsrc);
        break;
    case 2:
        sprintf(s, "%s %s,%s","and",rgdest, rgsrc);
        break;
    case 3:
        sprintf(s, "%s %s,%s","or",rgdest, rgsrc);
        break;
    case 4:
        sprintf(s, "%s %s,%s","xor",rgdest, rgsrc);
        break;
    case 5:
        sprintf(s, "%s %s,%s","nor",rgdest, rgsrc);
        break;
    case 6:
        sprintf(s, "%s %s,%s","sll",rgdest, rgsrc);
        break;
    case 7:
        sprintf(s, "%s %s,%s","srl",rgdest, rgsrc);
        break;
    case 8:
        sprintf(s, "%s %s,%s","slt",rgdest, rgsrc);
        break;
    default:
        break;
    }
    len = strlen(s);
    s[len]='\0';
}


void get_register(char* s, int rg)
{
    switch (rg)
    {
    case 0:
        strcpy(s,"$zero");
        break;
    case 1:
        strcpy(s,"$sp");
        break;
    case 2:
        strcpy(s,"$v1");
        break;
    case 3:
        strcpy(s,"$v2");
        break;
    case 4:
        strcpy(s,"$a1");
        break;
    case 5:
        strcpy(s,"$a2");
        break;
    case 6:
        strcpy(s,"$s1");
        break;
    case 7:
        strcpy(s,"$s2");
        break;
    case 8:
        strcpy(s,"$t1");
        break;
    case 9:
        strcpy(s,"$t2");
        break;
    case 10:
        strcpy(s,"$t3");
        break;
    case 11:
        strcpy(s,"$t4");
        break;
    case 12:
        strcpy(s,"$t5");
        break;
    case 13:
        strcpy(s,"$t6");
        break;
    case 14:
        strcpy(s,"$base");
        break;
    case 15:
        strcpy(s,"$ra");
        break;
    default:
        break;
    }
}
