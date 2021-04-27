#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED
#include <inttypes.h>
#define MAX_REGS 16
#define MAX_MEM 65536
#define MAX_DELMITER 100
#define MAX_RGSTR  8

struct state
{
    uint16_t pc;
    uint16_t ir;
    int16_t gpr[MAX_REGS];
    uint8_t  mem[MAX_MEM];
    uint16_t maxpc;
};

struct instruction
{
    uint8_t  opcode;
    uint8_t  dest;
    uint8_t  source;
    uint8_t  jump;
    int16_t immediate;
};
#endif // DATATYPE_H_INCLUDED
