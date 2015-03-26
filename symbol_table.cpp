#include "symbol_table.h"

/* This SymbolTable can't handle scope of a variable, so it can't handle variables with the same name */

int lookup(char *s)
{
	int i;
	for (i=0; i < Tsize-1; i++)
	{
		if (strcmp(s,SymbolTable[i].token_name) == 0)
			return i;
	}
	return -1;
}

int insert(tpToken token)
{
	int i=-1;
	while (strcmp(SymbolTable[++i].token_name,"") != 0);
	SymbolTable[i].token = token.token;
	strcpy(SymbolTable[i].token_name,token.token_name);
	strcpy(SymbolTable[i].token_type,token.token_type);
	SymbolTable[i].token_value = token.token_value;

	return i;
}

int insertAt(tpType token, char* name, char* type, int value)
{
	int i=-1;
	while (strcmp(SymbolTable[++i].token_name,"") != 0){}
	SymbolTable[i].token = token;
	strcpy(SymbolTable[i].token_name,name);
	strcpy(SymbolTable[i].token_type,type);
	SymbolTable[i].token_value = value;

	return i;
}

void report()
{
	int i = 0;
	FILE *report;

	report = fopen("report.log","w+");
	if (report == NULL)
	{
		printf("Error: could not dump the Symbol Table.");
	}

	fprintf(report," token \t \t name \t \t type \t \t value\n");
	while (SymbolTable[i].token != INVALID)
	{
		if (SymbolTable[i].token == num)
		{
			fprintf(report," NUM \t \t %s \t \t \t %s \t \t %d\n", SymbolTable[i].token_name,SymbolTable[i].token_type,SymbolTable[i].token_value);
		}
		else
		{
			fprintf(report," ID \t \t %s \t \t \t %s \t \t %d\n", SymbolTable[i].token_name,SymbolTable[i].token_type,SymbolTable[i].token_value);
		}
		i++;
	}
	return;
}

void remove()
{
	int i;
	for (i=0; i<Tsize-1; i++)
	{
		strcpy(SymbolTable[i].token_name,"");
		SymbolTable[i].token = INVALID;
		strcpy(SymbolTable[i].token_type,"");
		SymbolTable[i].token_value = 0;
	}
	return;
}

void init()
{
	int i;
	for (i=0; i<Tsize-1; i++)
	{
		strcpy(SymbolTable[i].token_name,"");
		SymbolTable[i].token = INVALID;
		strcpy(SymbolTable[i].token_type,"");
		SymbolTable[i].token_value = 0;
	}

	// Initialize the reserved words
	insertAt(keywd_program, "program", "keywd_program", 0);
	insertAt(keywd_var, "var", "keywd_var", 0);
	insertAt(keywd_of, "of", "keywd_of", 0);
	insertAt(keywd_array, "array", "keywd_array", 0);
	insertAt(keywd_int, "integer", "keywd_int", 0);
	insertAt(keywd_real, "real", "keywd_real", 0);
	insertAt(keywd_bool, "boolean", "keywd_bool", 0);
	insertAt(keywd_func, "function", "keywd_func", 0);
	insertAt(keywd_proc, "procedure", "keywd_proc", 0);
	insertAt(keywd_begin, "begin", "keywd_begin", 0);
	insertAt(keywd_end, "end", "keywd_end", 0);
	insertAt(keywd_if, "if", "keywd_if", 0);
	insertAt(keywd_then, "then", "keywd_then", 0);
	insertAt(keywd_else, "else", "keywd_else", 0);
	insertAt(keywd_while, "while", "keywd_while", 0);
	insertAt(keywd_do, "do", "keywd_do", 0);
	insertAt(keywd_not, "not", "keywd_not", 0);
	insertAt(l_paren, "(", "l_paren", 0);
	insertAt(r_paren, ")", "r_paren", 0);
	insertAt(semi_colon, ";", "semi_colon", 0);
	insertAt(comma, ",", "comma", 0);
	insertAt(colon, ":", "colon", 0);
	insertAt(left_bkt, "[", "left_bkt", 0);
	insertAt(right_bkt, "]", "right_bkt", 0);
	insertAt(dotdot, "..", "dotdot", 0);
	insertAt(assign_op, ":=", "assign_op", 0);
	insertAt(relop_eq, "==", "relop_eq", 0);
	insertAt(relop_gt, ">", "relop_gt", 0);
	insertAt(relop_lt, "<", "relop_lt", 0);
	insertAt(relop_ne, "<>", "relop_ne", 0);
	insertAt(relop_ge, ">=", "relop_ge", 0);
	insertAt(relop_le, "<=", "relop_le", 0);
	insertAt(plus_op, "+", "plus_op", 0);
	insertAt(sub_op, "-", "sub_op", 0);
	insertAt(mult_op, "*", "mult_op", 0);
	insertAt(div_op, "/", "div_op", 0);
	insertAt(mod_op, "mod", "mod_op", 0);

	return;
}