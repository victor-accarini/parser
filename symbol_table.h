#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <stdio.h>
#include <string.h>

#define Tsize 500
#define Nsize 200

enum tpType {INVALID, keywd_program, id, func_id, proc_id, array_id, keywd_var, keywd_of, keywd_array, num, keywd_int, keywd_real, keywd_bool,
	keywd_func, keywd_proc, keywd_begin, keywd_end, keywd_if, keywd_then, keywd_else, keywd_while, keywd_do, keywd_not, character, MyEOF,
	l_paren, r_paren, semi_colon, comma, colon, left_bkt, right_bkt, dotdot, assign_op, relop_eq, relop_gt, relop_lt, relop_ne, relop_ge, relop_le,
	plus_op, sub_op, mult_op, div_op, mod_op, addop, multop};

struct {
	enum tpType token;
	char token_name[200];
	char token_type[50];
	int token_value;
	char funcargs[500];
} typedef tpToken;

extern tpToken SymbolTable[Tsize];

int lookup(char *s);
int insert(tpToken token);
void report();
void remove();
void init();
void ToAscii(enum tpType tok, char* expected, int n);
void changeType(int n, char *type);
void changeToken(int n, enum tpType type);

#endif
