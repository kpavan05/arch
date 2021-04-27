#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "instructimp.h"
#include "utility.h"
#include "emulator.h"

/*
since this program reads only the mif format that is generated
by my assembly code, hard coded the file name and will be in same
directory as this program. it is easy to make it external as well.
*/

/*
the program first reads the mif file and stores the instructions in
an struct array and then run each instruction one by one.
There is struct for memory finite state machine this struct contains
array for holding memory contents of current machine machine,
array for general purpose registers
holder for pc value.
There is a struct for instruction (this is solely used for writing the log file)
As it runs pc value in the struct is updated then
it  takes the instruction at the locaton of array where pc is pointing to
and runs it.
While running, log file is generated which contains the current instruction
string and the values of the registers before the instruction is run.
*/
int main(int ac, char** av)
{
    FILE* fp = NULL;
    FILE* fplog = NULL;
    struct state* fsm = NULL;
	
    if (ac < 1) 
    {
	exit(1);
    }
    if ((fp = fopen(av[1]/*"emulator.mif"*/, "rb")) == NULL)
    {
        exit(1);
    }

    if ((fplog = fopen("log.txt","w")) == NULL)
    {
        exit(1);
    }
    /* create memory struct */
    fsm = (struct state*)malloc(sizeof(struct state));
    if (fsm == NULL)
    {
        exit(1);
    }
    init_state(fsm);

    read_mif(fp, fplog, fsm);

    run_instruction(fplog, fsm);

    fclose(fp);
    fclose(fplog);
    free(fsm);

    fsm = NULL;
    fp =NULL;
    fplog = NULL;

    return 0;
}

/*initializes the the struct state */
void init_state(struct state* fsm)
{
    int i = 0;
    fsm->pc = 0;
    fsm->ir = 0;
    for(i =0; i < MAX_REGS; i++)
    {
        fsm->gpr[i]= 0;
    }
    for (i = 0; i < MAX_MEM; i++)
    {
        fsm->mem[i] = 0;
    }
}

/* read mif file line by line, construction the instruction back */
void read_mif(FILE* fp, FILE* fplog, struct state* fsm)
{
    char buffer[BUFSIZ] = "";
    int start = 0;
    uint16_t inum = 0;
    uint16_t iid = 0;
    uint16_t lastnum = 0;/*
    if jump condition is satisfied then update the pc
    value to value in jump register else increment by 2
    */

    while (readline(fp, buffer)!= -1)
    {
        if (strstr(buffer, "BEGIN") != NULL)
        {
            start = 1;
            continue;
        }
        if (strcmp(buffer, "END") == 0)
        {
            break;
        }
        if (start != 1) continue;

        if (get_instruction(buffer, &iid, &inum) != 0)
        {
            /*data part starts*/
            if (inum - lastnum > 100)
            {
                fsm->maxpc = 2*lastnum;
            }
            fsm->mem[2*inum] = iid & 0x00FF ;
            fsm->mem[2*inum + 1] = (iid & 0xFF00) >> 8;
            lastnum =  inum;
        }
    }
    write_memory(fplog, fsm);
}

/* tokenizes the line using strtk in the mif file and get the instruction number*/
int get_instruction(char* buf, uint16_t* iid, uint16_t* inum)
{
     char tmp[BUFSIZ] ="";
     char* ptr = NULL;
     char* p1;
     char* p2;
     uint16_t num = 0;
     uint16_t icode = 0;

     strcpy(tmp, buf);
     if ((ptr = strtok(tmp, ":")) != NULL)
     {
        num = strtol(ptr, &p1, 16);
         /* see if strtol is successful before getting value */
        if (*p1 == '\0')
        {
            *inum = num;
        }
        ptr = strtok(NULL, ":");
        if (ptr == NULL) return 0;

        icode = strtol(ptr, &p2, 16);
        /* see if strtol is successful */
        if (*p2 != '\0')
        {
            return 0;
        }
        else
        {
            *iid= icode;
            return 1;
        }
     }
     return 0;
}

/* run the instructions based on current pc value*/
void run_instruction(FILE* fp, struct state* fsm)
{
    fsm->pc = 0;
    int hi, lo;
    while(fsm->pc < 0xF000)
    {
        hi = fsm->mem[fsm->pc + 1];
        lo = fsm->mem[fsm->pc];
        fsm->ir = (hi << 8 )+ lo;
        construct_instr(fp, fsm);
    }
}

/* construction struct instruction from the number */
/* and execute the instruction */
int construct_instr(FILE* fp, struct state* fsm)
{
    uint16_t ivar = fsm->ir;
    uint8_t opcode = (ivar & 0xF000) >> 12;
    int ret = 0;

    write_log(fp, fsm);

    /* construct the instruction struct which will be useful for logging*/
    struct instruction* instr = (struct instruction*) malloc(sizeof(struct instruction));
    memset(instr, 0, sizeof(struct instruction));
    switch(opcode)
    {
    case 0:
        ret = do_rtype(fp, fsm, instr);
        break;
    case 1:
        ret = do_addi(fp, fsm, instr);
        break;
    case 2:
        ret =do_ori(fp, fsm, instr);
        break;
    case 3:
        ret =do_lui(fp, fsm, instr);
        break;
    case 4:
        ret =do_lw(fp, fsm, instr);
        break;
    case 5:
        ret =do_sw(fp, fsm, instr);
        break;
    case 6:
        ret =do_lb(fp, fsm, instr);
        break;
    case 7:
        ret =do_sb(fp, fsm, instr);
        break;
    case 8:
        ret =do_jreq(fp, fsm, instr);
        break;
    case 9:
        ret =do_jrne(fp, fsm, instr);
        break;
    case 10:
        ret =do_jmp(fp, fsm, instr);
        break;
    case 11:
        ret =do_jal(fp, fsm, instr);
        break;
    default:
        break;
    }
    free(instr);
    instr = NULL;
    return ret;
}

/* write the log file with the contents of the registers */
void write_log(FILE* fp, struct state* s)
{
    int i;

    fprintf(fp,"\n");
    /* write the instruction number to log file*/
    fprintf(fp, "instruction id: %"PRIx16"\n", s->pc);

    /* write the register values to log file*/
    for (i = 0; i< MAX_REGS; i++)
    {
        fprintf(fp, "%d : %"PRId16"\n", i, s->gpr[i]);
    }
    for (i=0 ; i < MAX_DELMITER; i++)
    {
        fprintf(fp,"*");
    }
    fprintf(fp, "\n");
}

/* write memory array to file */
void write_memory(FILE* fp, struct state* s)
{
    int i, j;
    uint16_t res;
    FILE* fpmem =NULL;
    int max_index = MAX_MEM/2;
    if ((fpmem = fopen("memory.txt","w")) == NULL)
    {
        exit(1);
    }
    for (i = 0; i< max_index; i++)
    {
        j = 2*i;
        res = ((s->mem[j+1]) << 8)  + s->mem[j];
        fprintf(fpmem, "%02x : %04x\n", i, res);
    }
    fclose(fpmem);
    fpmem = NULL;
}
