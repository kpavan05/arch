#include <stdio.h>
#include <stdlib.h>
#include "rtype.h"

int do_add(struct state* fsm, struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    fsm->gpr[instr->dest] += fsm->gpr[instr->source];
    fsm->pc = fsm->pc + 2;
    return 1;
}

int do_sub(struct state* fsm, struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    fsm->gpr[instr->dest] -= fsm->gpr[instr->source];
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_and(struct state* fsm, struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    fsm->gpr[instr->dest] &= fsm->gpr[instr->source];
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_or(struct state* fsm, struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    fsm->gpr[instr->dest] |= fsm->gpr[instr->source];
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_xor(struct state* fsm,struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    fsm->gpr[instr->dest] ^= fsm->gpr[instr->source];
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_nor(struct state* fsm,struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    uint16_t idest , isource;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    idest = fsm->gpr[instr->dest];
    isource =  fsm->gpr[instr->source];

    idest = idest | isource;
    fsm->gpr[instr->dest] = ~idest;
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_sll(struct state* fsm, struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    uint16_t idest , isource;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    idest = fsm->gpr[instr->dest];
    isource =  fsm->gpr[instr->source];
    fsm->gpr[instr->dest] = idest << isource;
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_srl(struct state* fsm, struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    uint16_t idest , isource;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    idest = fsm->gpr[instr->dest];
    isource =  fsm->gpr[instr->source];
    fsm->gpr[instr->dest] = idest >> isource;
    fsm->pc = fsm->pc + 2;
    return 1;
}
int do_slt(struct state* fsm,struct instruction* instr)
{
    uint16_t curinstr = fsm->ir;
    int16_t idest , isource;
    if (instr == NULL) return 0;

    instr->opcode = (curinstr & 0xF000) >> 12;
    instr->dest = (curinstr & 0x0F00) >> 8;
    instr->source = (curinstr & 0x00F0) >> 4;

    idest = fsm->gpr[instr->dest];
    isource =  fsm->gpr[instr->source];
    if (idest < isource)
    {
        fsm->gpr[14]= 1;
    }
    else
    {
       fsm->gpr[14] = 0;
    }
    fsm->pc = fsm->pc + 2;
    return 1;
}

