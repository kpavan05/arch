#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "table.h"
#include "utilities.h"
#include "instruction.h"
#include "instrlist.h"
#include "constant.h"
#include "assembler.h"

static FILE* fpsrc = NULL;
static FILE* fpdest = NULL;
static struct instruction* head = NULL;


/*
instruction file will be as follows
.text
  instruction
  instruction
label:
  instruction..
    ...
done with instructions

.data
  label:
  .asciiz string


*/
int main(int ac, char** av)
{
     if (ac < 3) return -1;

    /* open the instruction file */
    if ((fpsrc = fopen(av[1], "r")) == NULL)
		return -1;

    /* create a file for mif generation */
    if ((fpdest = fopen(av[2], "w")) == NULL)
		return -1;

    fputs("DEPTH = 16384;\n", fpdest);
    fputs("WIDTH = 16;\n", fpdest);
    fputs("ADDRESS_RADIX = HEX;\n", fpdest);
    fputs("DATA_RADIX = HEX;\n", fpdest);
    fputs("CONTENT\n",fpdest);
    fputs("BEGIN\n", fpdest);
	parse(fpsrc);

	process(fpdest);
	tablerelease();
    listrelease(head);

    fputs("END\n", fpdest);
    fclose(fpsrc);
    fclose(fpdest);
    return 0;
}
/*
do two passes. one for reading the labels and storing in a table
(max of 100 labels are allowed just a limit can be increased in the #define)
another pass to read each instruction line and store in the linked list
*/
void parse(FILE* fp)
{
    char buf[BUF_SIZE] = "";
    int address = 0;
    int linenum =0;

    /* read labels and store in symbol table */
    if (readsymbols(fp) == 0)
    {
        error("too many labels.\n" ,-1);
    }

    fseek(fp,0, SEEK_SET);

	while (readline(fp, buf)!= -1)
    {
        linenum++;
        if (buf[0] == '\0' || buf[0] == '\n')
            continue;

        if (findtoken( buf, '#') != -1)
            continue;

        if (findtoken(buf, ':') != -1)
            continue;

        if (strcmp(buf, ".text") == 0)
            continue;

        if (strcmp(buf, ".data") == 0)
            break;


        parseinstruction(buf, linenum, &address);
        address += 1;
    }
}


/*
parse each instruction line and load the struct variables
such as opcode, registers, immediate and word address
then store this loaded struct in a linked list
*/
void parseinstruction(char* buf, int linenum, int* address)
{
    int pos = 0;
    char tmp[BUF_SIZE] = "";
    char* ptr = NULL;

    struct instruction* instr = (struct instruction*)(malloc(sizeof(struct instruction)));
    instr->curparam = 0;
    instr->address = *address;
    strcpy(instr->str, buf);
    instr->next = NULL;

    if ((pos = get_opcode(buf, instr)) == -1)
    {
        error("no such opcode exists at line", linenum);
    }
    /*
        increment the address one more for "la"
        since it will be split into two instructions
        as lui and ori while writing the mif file
    */
    if (is_pseudoinstruct(instr->opstr))
    {
        *address = *address + 1;
    }

    /* tokenize the string on comma */
    strcpy(tmp, buf+pos);
    ptr = strtok(tmp, ",");

    while(ptr != NULL)
    {
        /*
        if a '$' is found in the token then it is a register
        else it is a immediate
        */
        if(findtoken(ptr, '$') != -1)
        {
            instr->curparam++;
            if (get_register(ptr, instr) == -1)
            {
                error("no such register exists at line.", linenum);
            }
        }
        else
        {
            instr->curparam++;
            if (get_immediate(ptr, instr) == -1)
            {
                error("no label is found at line.\n", linenum);
            }
        }
        ptr = strtok(NULL,",");
    }
    /*
    if it is the first instruction then it is the head of
    the linked list or else add to end of the list
    */
    if (head == NULL) {head = instr;}
    else write2list(head, instr);

    ptr = NULL;
}

/*
once the instruction file is read and all the instructions
loaded into linked list, take each item from the list and process them
to get the instruction id and the actual instruction
at the end process the string in the .data section of the instruction file
*/
void process(FILE* fp)
{
    struct instruction* item = head;
    /* processing the linked list*/
    while (item != NULL)
    {
        process_instruction(fp, item);
        item = item->next;
    }
    /* process the data strings */
    processdata(fp);
}


/*
opcode string is separated by a space from the parameters
search the string till the first space character is encountered.
get the substring from beginning to the index where ' ' is found
from the substring get the code and load into struct instruction
*/
int get_opcode(char* str, struct instruction* instr)
{
    int opbegin = -1;
    int opend = -1;
    char opstr[MAX_PARAM]= "";
    int pos = 0;

    if (instr == NULL) return -1;

    while (str[pos] != '\0')
    {
        if (str[pos] == ' ' || str[pos] == '\t')
        {
            if (opbegin != -1 && opend == -1)
            {
                opend = pos - 1;
                getsubstring(str, opbegin, opend, opstr);
                instr->opstr = strdup(opstr);
                return ++pos;
            }
            else
            {
                pos++;
                continue;
            }
        }
        if (opbegin == -1) opbegin = pos;
        pos++;
    }
    return -1;
}


int get_immediate(char* str, struct instruction* instr)
{
    int address = 0;
    int num =0;
    char* ptr;
    char temp[BUF_SIZE] = "";
    if (instr == NULL) return -1;

    strcpy(temp, str);
    trim(temp);
    errno = 0;
    num = strtol(temp, &ptr, 0);
    if (*ptr != '\0')
    {
        if (findsymbol(temp, &address) == -1)
        {
            return -1;
        }
        if (is_pseudoinstruct(instr->opstr))
        {
            instr->immediate = address;
            return 0;
        }
        /*
        if the immediate is the first parameter then it is 12 bit
        such as in j, jal instruction
        */
        if (instr->curparam == 1)
        {
            instr->immediate = address & 0x00000FFF;
        }
        /* if the immediate is the second parameter processed, then it is 8 bits*/
        else if (instr->curparam == 2)
        {
            instr->immediate = address & 0x000000FF;
        }
    }
    else
    {
        instr->immediate = num;
    }
    return 0;
}
/* get the register string and convert them to integer*/
/*increment the number of parameters processed in the instruction */
int get_register(char* str, struct instruction* instr)
{
    int ret;
    int i =0, pos = 0;
    char rgstr[MAX_PARAM] ="";

    if (instr == NULL) return -1;

    while(str[i] != '\0')
    {
        if (str[i] != ' ' && str[i] != '\t')
        rgstr[pos++] = str[i];
        i++;
    }
    rgstr[pos] ='\0';

    if ((ret = regstr2int(rgstr)) == -1)
    {
       return -1;
    }

    if (instr->curparam == 1)instr->reg1 = ret;
    if (instr->curparam == 2)instr->reg2 = ret;
    if (instr->curparam == 3)instr->reg3 = ret;

    return 0;
}

/*
if error in processing the file then release all
data structs and exit
*/
void error(char* s, int linenum)
{
     char msg[BUF_SIZE] ="";
     sprintf(msg, "\%s-\%d\n", s, linenum);
     if (fpdest != NULL)
        fputs(s, fpdest);

     tablerelease();
     listrelease(head);
     fclose(fpsrc);
     fclose(fpdest);
     exit(1);
}
