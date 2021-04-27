#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED
#include "constant.h"
#include "inttypes.h"

struct instruction
{
    char* opstr;
    int reg1;
    int reg2;
    int reg3;
    int immediate;
    int curparam;
    int address;
    char str[256];
    struct instruction* next;
};

int is_pseudoinstruct(char* );
int is_branchtype(char* );
int is_jmptype(char* );
int is_itype(char* );
int is_rtype(char* );
int is_lwsw(char* );

void process_instruction(FILE* ,struct instruction* );
void process_la(FILE* , struct instruction* );

uint16_t process_rtype(struct instruction* );
uint16_t process_itype(struct instruction*);
uint16_t process_lwsw(struct instruction*  );
uint16_t process_brtype(struct instruction* );
uint16_t process_jmp(struct instruction* );

int regstr2int(char* );

#endif // INSTRUCTION_H_INCLUDED
