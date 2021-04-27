#ifndef RTYPE_H_INCLUDED
#define RTYPE_H_INCLUDED

#include "datatype.h"

int do_add(struct state* , struct instruction* );
int do_sub(struct state* ,struct instruction* );
int do_and(struct state* ,struct instruction* );
int do_or(struct state* ,struct instruction* );
int do_xor(struct state* ,struct instruction* );
int do_nor(struct state* ,struct instruction* );
int do_sll(struct state* ,struct instruction* );
int do_srl(struct state* ,struct instruction* );
int do_slt(struct state* ,struct instruction* );

#endif // RTYPE_H_INCLUDED
