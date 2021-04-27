#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

void parse(FILE* );
void parseinstruction(char* , int ,int* );
void process(FILE* );

int get_opcode(char* , struct instruction* );
int get_register(char* , struct instruction* );
int get_immediate(char* , struct instruction* );

void error(char* , int );
#endif // ASSEMBLER_H_INCLUDED
