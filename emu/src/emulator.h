#ifndef EMULATOR_H_INCLUDED
#define EMULATOR_H_INCLUDED


#include "datatype.h"


void init_state(struct state* );
void read_mif(FILE* , FILE* , struct state*);
void run_instruction(FILE* , struct state* );
int construct_instr(FILE* , struct state* );
int get_instruction(char* , uint16_t* , uint16_t* );
void write_log(FILE* , struct state* );
void write_memory(FILE* , struct state* );
#endif // EMULATOR_H_INCLUDED
