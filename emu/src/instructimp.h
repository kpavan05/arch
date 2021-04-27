#ifndef INSTRUCTIMP_H_INCLUDED
#define INSTRUCTIMP_H_INCLUDED

#include "datatype.h"

int do_rtype(FILE* , struct state* , struct instruction* );
int do_addi(FILE* , struct state* ,struct instruction* );
int do_ori(FILE* , struct state* ,struct instruction* );
int do_lui(FILE* , struct state* ,struct instruction* );
int do_lw(FILE* , struct state* ,struct instruction* );
int do_sw(FILE* , struct state* ,struct instruction* );
int do_lb(FILE* , struct state* ,struct instruction* );
int do_sb(FILE* , struct state* ,struct instruction* );
int do_jreq(FILE* , struct state* ,struct instruction* );
int do_jrne(FILE* , struct state* ,struct instruction* );
int do_jmp(FILE* , struct state* ,struct instruction* );
int do_jal(FILE* , struct state* ,struct instruction* );


#endif // INSTRUCTIMP_H_INCLUDED
