#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"
#include "utilities.h"
#include "constant.h"

void process_instruction(FILE* fp, struct instruction* instr)
{
    uint16_t result =0;
    char line[BUF_SIZE] = "";
    if (fp == NULL) return;

    /* process rtype if add, sub, or etc for opcode -0000*/
    if (is_rtype(instr->opstr))
    {
        result = process_rtype(instr);
    }
    /* if it is a "la" then split into two */
    else if (is_pseudoinstruct(instr->opstr))
    {
        process_la(fp, instr);
        return;
    }
    /* process instructions such as addi, ori,..*/
    else if (is_itype(instr->opstr))
    {
        result = process_itype(instr);
    }
    /* process lw, sw, lb, sb*/
    else if (is_lwsw(instr->opstr))
    {
        result = process_lwsw(instr);
    }
    /* process j, jal */
    else if (is_jmptype(instr->opstr))
    {
       result = process_jmp(instr);
    }
    /*process jreq, jrne*/
    else if (is_branchtype(instr->opstr))
    {
        result = process_brtype(instr);
    }
    else
    {
        return;
    }

    if (result == -1)return;

    result = result & 0x0000FFFF;
    sprintf(line,"%02x: %02x; --%s\n",instr->address, result, instr->str);
    fputs(line, fp);
}

int is_rtype(char* str)
{
    char s[MAX_PARAM] = "";

    strcpy(s, str);
    stoupper(s);
    if (strcmp(s, "ADD")   == 0 ||
        strcmp(s, "SUB")   == 0 ||
        strcmp(s, "AND")   == 0 ||
        strcmp(s, "OR")    == 0 ||
        strcmp(s, "XOR")   == 0 ||
        strcmp(s, "NOR")   == 0 ||
        strcmp(s, "SLL")   == 0 ||
        strcmp(s, "SRL")   == 0 ||
        strcmp(s, "SLT")   == 0 ||
        strcmp(s, "MULT")  == 0 ||
        strcmp(s, "MFHI")  == 0 ||
        strcmp(s, "MFLO")  == 0
        )
    {
        return 1;
    }

    return 0;
}

int is_itype(char* str)
{
    char s[MAX_PARAM] = "";
    strcpy(s, str);
    stoupper(s);
    if (
        strcmp(s, "ADDI")   == 0 ||
        strcmp(s, "ORI")   == 0 ||
        strcmp(s, "LUI")   == 0 ||
        strcmp(s, "LA")   == 0
        )
    {
        return 1;
    }

    return 0;
}

int is_lwsw(char* str)
{
    char s[MAX_PARAM] = "";
    strcpy(s, str);
    stoupper(s);
    if (
        strcmp(s, "LW")    == 0 ||
        strcmp(s, "SW")   == 0 ||
        strcmp(s, "LB")   == 0 ||
        strcmp(s, "SB")   == 0
        )
    {
        return 1;
    }

    return 0;
}

int is_branchtype(char* str)
{
    char s[MAX_PARAM] = "";
    strcpy(s, str);
    stoupper(s);

    if  (
        strcmp(s, "JREQ")   == 0 ||
        strcmp(s, "JRNE")   == 0
        )
    {
        return 1;
    }

    return 0;
}

int is_jmptype(char* str)
{
    char s[MAX_PARAM] = "";
    strcpy(s, str);
    stoupper(s);
    if  (
        strcmp(s, "J")   == 0 ||
        strcmp(s, "JAL")   == 0
        )
    {
        return 1;
    }

    return 0;
}

int is_pseudoinstruct(char* str)
{
    char s[MAX_PARAM] = "";
    strcpy(s, str);
    stoupper(s);
    if (strcmp(s, "LA") == 0)
        return 1;
    return 0;
}
uint16_t process_rtype(struct instruction* instr)
{
    int func;
    int r1,r2,result;
    char s[MAX_PARAM] = "";
    strcpy(s, instr->opstr);
    stoupper(s);

    if (strcmp(s, "ADD") == 0)
    {
        func = 0;
    }
    else if (strcmp(s, "SUB") == 0)
    {
        func = 1;
    }
    else if (strcmp(s, "AND") == 0)
    {
        func = 2;
    }
    else if (strcmp(s, "OR") == 0)
    {
        func = 3;
    }
    else if (strcmp(s, "XOR") == 0)
    {
        func = 4;
    }
    else if (strcmp(s, "NOR") == 0)
    {
        func = 5;
    }
    else if (strcmp(s, "SLL") == 0)
    {
        func = 6;
    }
    else if (strcmp(s, "SRL") == 0)
    {
        func = 7;
    }
    else if (strcmp(s, "SLT") == 0)
    {
        func = 8;
    }
    else if (strcmp(s, "MULT") == 0)
    {
        func = 9;
    }
     else if (strcmp(s, "MFHI") == 0)
    {
        func = 11;
        instr->reg2 = 0;
    }
     else if (strcmp(s, "MFLO") == 0)
    {
        func = 10;
        instr->reg2 = 0;
    }
    else
        return -1;

    r1 = (instr->reg1) << 8;
    r2 = (instr->reg2) << 4;
    result = r1 + r2 + func;

    return result;
}

uint16_t process_brtype(struct instruction* instr)
{
    int result = 0;
    int opcode =0;
    int r1 = 0 , r2 =0, r3 =0;
    char s[MAX_PARAM] = "";
    strcpy(s, instr->opstr);
    stoupper(s);

    if  (strcmp(s, "JREQ")   == 0)
    {
         opcode = 8;
    }
    else if (strcmp(s, "JRNE") == 0)
    {
         opcode = 9;
    }
    else
    {
        return -1;
    }
    opcode = opcode << 12;
    r1 = instr->reg1 << 8;
    r2 = instr->reg2 << 4;
    r3 = instr->reg3;
    result = opcode + r1 + r2 + r3;
    return result;
}

uint16_t process_itype(struct instruction*  instr)
{
    int result = 0;
    int opcode = 0;
    int reg = 0;
    char s[MAX_PARAM] = "";
    strcpy(s, instr->opstr);
    stoupper(s);

    if (strcmp(s, "ADDI")   == 0 )
    {
        opcode = 1;
    }
    else if (strcmp(s, "ORI")   == 0 )
    {
        opcode = 2;
    }
    else if (strcmp(s, "LUI")   == 0 )
    {
        opcode = 3;
    }
    else
    {
        return -1;
    }
    opcode = opcode << 12;
    reg = instr->reg1 << 8;
    if (instr->immediate < 0)
    {
        /* mask the 8 bits*/
        instr->immediate = instr->immediate & 0x00FF;
    }
    result = opcode + reg + instr->immediate;
    return result;
}

uint16_t process_lwsw(struct instruction*  instr)
{
    int result = 0;
    int opcode = 0;
    int reg = 0;
    int base = 0;
    char s[MAX_PARAM] = "";
    strcpy(s, instr->opstr);
    stoupper(s);

    if (strcmp(s, "LW")    == 0 )
    {
        opcode = 4;
        instr->immediate = 0;
    }
    else if (strcmp(s, "SW")   == 0 )
    {
        opcode = 5;
        instr->immediate = 0;
    }
    else if (strcmp(s, "LB")   == 0 )
    {
        opcode = 6;
        instr->immediate = 0;
    }
    else if (strcmp(s, "SB")   == 0 )
    {
         opcode = 7;
         instr->immediate = 0;
    }
    else
    {
        return -1;
    }
    opcode = opcode << 12;
    reg = instr->reg1 << 8;
    base = instr->reg2 << 4;

    result = opcode + reg + base;
    return result;
}

uint16_t process_jmp(struct instruction* instr)
{
    int opcode =0;
    int result = 0;
    char s[MAX_PARAM] = "";
    strcpy(s, instr->opstr);
    stoupper(s);
    if  (strcmp(s, "J")   == 0 )
    {
        opcode = 10;
    }
    else if (strcmp(s, "JAL")   == 0)
    {
        opcode = 11;
    }
    else
    {
        return -1;
    }
    opcode = opcode << 12;
    result = opcode + instr->immediate;
    return result;
}

void process_la(FILE* fp, struct instruction* instr)
{
    char line[BUF_SIZE] = "";
    int opcode = 0, reg =0;
    int upper =0, lower = 0;
    uint16_t result =0;

    opcode = 3 << 12;
    reg = instr->reg1 << 8;
    upper = instr->immediate & 0xFFFFFF00;
    upper = upper >> 8;

    result = opcode + reg + upper;
    sprintf(line,"%02x: %02x;--%s\n",instr->address, result, instr->str);
    fputs(line, fp);

    opcode = 2 << 12;
    reg = instr->reg1 << 8;
    lower = instr->immediate & 0x000000FF;
    result = opcode + reg + lower;
    sprintf(line,"%02x: %02x;--%s\n",instr->address + 1, result, instr->str);
    fputs(line, fp);

}

/* get register numbers from strings*/
int regstr2int(char* rgstr)
{
    if (rgstr[0] == '\0') return -1;

    if (strcmp(rgstr,"$base") == 0)
        return BASE_REGISTER;
    else if (strcmp(rgstr,"$zero") == 0)
        return ZERO_REGISTER;
    else if (strcmp(rgstr, "$sp") == 0)
        return STACK_REGISTER;
    else if  (strcmp(rgstr,"$a1") == 0)
        return A1_REGISTER;
    else if  (strcmp(rgstr,"$a2") == 0)
        return A2_REGISTER;
    else if  (strcmp(rgstr,"$t1") == 0)
        return T1_REGISTER;
    else if  (strcmp(rgstr,"$t2") == 0)
        return T2_REGISTER;
    else if  (strcmp(rgstr,"$t3") == 0)
        return T3_REGISTER;
    else if  (strcmp(rgstr,"$t4") == 0)
        return T4_REGISTER;
    else if  (strcmp(rgstr,"$t5") == 0)
        return T5_REGISTER;
    else if  (strcmp(rgstr,"$t6") == 0)
        return T6_REGISTER;
    else if  (strcmp(rgstr,"$s1") == 0)
        return S1_REGISTER;
    else if  (strcmp(rgstr,"$s2") == 0)
        return S2_REGISTER;
    else if  (strcmp(rgstr,"$v1") == 0)
        return V1_REGISTER;
    else if  (strcmp(rgstr,"$v2") == 0)
        return V2_REGISTER;
    else if  (strcmp(rgstr,"$ra") == 0)
        return RET_REGISTER;
    else
        return -1;
}
