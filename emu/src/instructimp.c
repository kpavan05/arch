#include <stdio.h>
#include <stdlib.h>
#include "instructimp.h"
#include "instructstr.h"
#include "rtype.h"

int do_rtype(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] ="";
    uint16_t curinstr = fsm->ir;
    uint8_t func;
    int ret=0;
    if (instr == NULL) return 0;

    func = (curinstr & 0x000F);
    switch(func)
    {
    case 0:
        ret = do_add(fsm, instr);
        break;
    case 1:
        ret = do_sub(fsm, instr);
        break;
    case 2:
        ret = do_and(fsm, instr);
        break;
    case 3:
        ret = do_or(fsm, instr);
        break;
    case 4:
        ret = do_xor(fsm, instr);
        break;
    case 5:
        ret = do_nor(fsm, instr);
        break;
    case 6:
        ret = do_sll(fsm, instr);
        break;
    case 7:
        ret = do_srl(fsm, instr);
        break;
    case 8:
        ret = do_slt(fsm, instr);
        break;
    default:
        break;
    }
    do_rtypestr(str, instr, func);
    fprintf(fp, "%s", str);
    return ret;
}

int do_addi(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    int8_t immediate = 0;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    immediate = (curinstr & 0x00FF);
    if (immediate < 0 )
    {
        instr->immediate = immediate | 0xFF00;
    }
    else
    {
        instr->immediate = immediate;
    }
    fsm->gpr[instr->dest] += instr->immediate;
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_ori(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->immediate = (curinstr & 0x00FF);

    fsm->gpr[instr->dest] |= instr->immediate;
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_lui(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->immediate = curinstr & 0x00FF;

    fsm->gpr[instr->dest] = instr->immediate << 8;
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_lw(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    uint16_t addr;
    int c;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    addr = fsm->gpr[instr->source];
    if (addr == 0xFF00)
    {
        fsm->gpr[instr->dest] = 0x000F;
    }
    else if (addr == 0xFF04)
    {
        c = getchar();
        fsm->gpr[instr->dest] = c;
    }
    else
    {
        //fsm->gpr[instr->dest] = ((fsm->mem[addr]) << 8 )+ fsm->mem[addr + 1];
        fsm->gpr[instr->dest] = ((fsm->mem[addr + 1]) << 8 )+ fsm->mem[addr];
    }
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_sw(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    uint16_t addr;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    addr = fsm->gpr[instr->source];
    if (addr == 0xFF00)
    {
        if ((fsm->gpr[instr->dest] & 0x02) == 0x02)
            fflush(stdout);
    }
    else if (addr == 0xFF04)
    {
        putchar(fsm->gpr[instr->dest] & 0x00FF);
    }
    else
    {
        //fsm->mem[addr] = (fsm->gpr[instr->dest] & 0xFF00) >> 8;
        //fsm->mem[addr + 1] = fsm->gpr[instr->dest] ;
        fsm->mem[addr + 1] = (fsm->gpr[instr->dest] & 0xFF00) >> 8;
        fsm->mem[addr] = fsm->gpr[instr->dest] ;
    }
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_lb(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    uint16_t addr;
    uint8_t c;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    addr = fsm->gpr[instr->source];
    if (addr == 0xFF00)
    {
        fsm->gpr[instr->dest] = 0x000F;
    }
    else if (addr == 0xFF04)
    {
        c = getchar();
        fsm->gpr[instr->dest] = c;
    }
    else
    {
        c = fsm->mem[addr] & 0x00FF;
        fsm->gpr[instr->dest] = c;
    }
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_sb(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    uint16_t addr;
    uint8_t c;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    addr = fsm->gpr[instr->source];
    c = fsm->gpr[instr->dest] & 0x00FF;
    if (addr == 0xFF00)
    {
        if ((fsm->gpr[instr->dest] & 0x02) == 0x02)
            fflush(stdout);
    }
    else if (addr == 0xFF04)
    {
        putchar(c);
    }
    else
    {
        fsm->mem[addr] = c;
    }
    fsm->pc = fsm->pc + 2;
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_jreq(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;
    instr->jump = (curinstr & 0x000F) ;

    if (fsm->gpr[instr->dest] == fsm->gpr[instr->source])
    {
        fsm->pc = fsm->gpr[instr->jump];
    }
    else
    {
        fsm->pc = fsm->pc + 2;
    }
    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_jrne(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;
    instr->jump = (curinstr & 0x000F) ;

    if (fsm->gpr[instr->dest] != fsm->gpr[instr->source])
    {
        fsm->pc = fsm->gpr[instr->jump];
    }
    else
    {
        fsm->pc = fsm->pc + 2;
    }

    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_jmp(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->immediate = (curinstr & 0x0FFF);

    fsm->pc = (fsm->pc & 0xF000) + instr->immediate;

    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

int do_jal(FILE* fp, struct state* fsm, struct instruction* instr)
{
    char str[BUFSIZ] = "";
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->immediate = (curinstr & 0x0FFF);

    fsm->gpr[15] = fsm->pc + 2;
    fsm->pc = (fsm->pc & 0xF000) + instr->immediate;

    do_str(str, instr);
    fprintf(fp, "%s", str);
    return 1;
}

