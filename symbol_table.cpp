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

void changeToken(int n, enum tpType type)
{
	char str[50];
	ToAscii(type, str, n);
	SymbolTable[n].token = type;

}

void changeType(int n, char *type)
{
	strcpy(SymbolTable[n].token_type,type);
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

	fprintf(report," %30stoken %31sname %31stype %5svalue %10sscope\n","","","","","");
	fprintf(report,"---------------------------------------------------------------------------------------------------------------------------------------\n");
	char str[50];
	while (SymbolTable[i].token != INVALID)
	{
		ToAscii(SymbolTable[i].token,str,i);
		fprintf(report," %35s %35s %35s %10d %15s\n", str, SymbolTable[i].token_name,SymbolTable[i].token_type,SymbolTable[i].token_value,
				SymbolTable[i].scope);
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
		strcpy(SymbolTable[i].funcargs, "");
	}

	// Initialize the reserved words
	insertAt(proc_id,"read","built-in function", 0);
	insertAt(proc_id,"write","built-in function", 0);
	strcpy(SymbolTable[0].funcargs,"int int");
	strcpy(SymbolTable[1].funcargs,"int");
	insertAt(keywd_program, "program", "reserved", 0);
	insertAt(keywd_var, "var", "reserved", 0);
	insertAt(keywd_of, "of", "reserved", 0);
	insertAt(keywd_array, "array", "reserved", 0);
	insertAt(keywd_int, "integer", "reserved", 0);
	insertAt(keywd_real, "real", "reserved", 0);
	insertAt(keywd_bool, "boolean", "reserved", 0);
	insertAt(keywd_func, "function", "reserved", 0);
	insertAt(keywd_proc, "procedure", "reserved", 0);
	insertAt(keywd_begin, "begin", "reserved", 0);
	insertAt(keywd_end, "end", "reserved", 0);
	insertAt(keywd_if, "if", "reserved", 0);
	insertAt(keywd_then, "then", "reserved", 0);
	insertAt(keywd_else, "else", "reserved", 0);
	insertAt(keywd_while, "while", "reserved", 0);
	insertAt(keywd_do, "do", "reserved", 0);
	insertAt(keywd_not, "not", "reserved", 0);
	insertAt(l_paren, "(", "reserved", 0);
	insertAt(r_paren, ")", "reserved", 0);
	insertAt(semi_colon, ";", "reserved", 0);
	insertAt(comma, ",", "reserved", 0);
	insertAt(colon, ":", "reserved", 0);
	insertAt(left_bkt, "[", "reserved", 0);
	insertAt(right_bkt, "]", "reserved", 0);
	insertAt(dotdot, "..", "reserved", 0);
	insertAt(assign_op, ":=", "reserved", 0);
	insertAt(relop_eq, "==", "reserved", 0);
	insertAt(relop_gt, ">", "reserved", 0);
	insertAt(relop_lt, "<", "reserved", 0);
	insertAt(relop_ne, "<>", "reserved", 0);
	insertAt(relop_ge, ">=", "reserved", 0);
	insertAt(relop_le, "<=", "reserved", 0);
	insertAt(plus_op, "+", "reserved", 0);
	insertAt(sub_op, "-", "reserved", 0);
	insertAt(mult_op, "*", "reserved", 0);
	insertAt(div_op, "/", "reserved", 0);
	insertAt(mod_op, "mod", "reserved", 0);

	return;
}

void ToAscii(enum tpType tok, char* expected, int n)
{
	char name[70];
	switch (tok)
	{
		case INVALID:
			sprintf(name,"INVALID->%s",SymbolTable[n].token_name);
			strcpy(expected,name);
			break;
		case keywd_program:
			strcpy(expected,"program_kw");
			break;
		case id:
			sprintf(name,"id->%s",SymbolTable[n].token_name);
			strcpy(expected,name);
			break;
		case func_id:
			sprintf(name,"function id->%s",SymbolTable[n].token_name);
			strcpy(expected,name);
			break;
		case proc_id:
			sprintf(name,"procedure id->%s",SymbolTable[n].token_name);
			strcpy(expected,name);
			break;
		case array_id:
			sprintf(name,"array id->%s",SymbolTable[n].token_name);
			strcpy(expected,name);
			break;
		case keywd_var:
			strcpy(expected,"var_kw");
			break;
		case keywd_of:
			strcpy(expected,"of_kw");
			break;
		case keywd_array:
			strcpy(expected,"array_kw");
			break;
		case num:
			sprintf(name,"number->%s",SymbolTable[n].token_name);
			strcpy(expected,name);
			break;
		case keywd_int:
			strcpy(expected,"int_kw");
			break;
		case keywd_real:
			strcpy(expected,"real_kw");
			break;
		case keywd_bool:
			strcpy(expected,"bool_kw");
			break;
		case keywd_func:
			strcpy(expected,"function_kw");
			break;
		case keywd_proc:
			strcpy(expected,"procedure_kw");
			break;
		case keywd_begin:
			strcpy(expected,"begin_kw");
			break;
		case keywd_end:
			strcpy(expected,"end_kw");
			break;
		case keywd_if:
			strcpy(expected,"if_kw");
			break;
		case keywd_then:
			strcpy(expected,"then_kw");
			break;
		case keywd_else:
			strcpy(expected,"else_kw");
			break;
		case keywd_while:
			strcpy(expected,"while_kw");
			break;
		case keywd_do:
			strcpy(expected,"do_kw");
			break;
		case keywd_not:
			strcpy(expected,"not_kw");
			break;
		case character:
			strcpy(expected,"character");
			break;
		case MyEOF:
			strcpy(expected,"End of file");
			break;
		case l_paren:
			strcpy(expected,"l_paren");
			break;
		case r_paren:
			strcpy(expected,"r_paren");
			break;
		case semi_colon:
			strcpy(expected,"semi_colon");
			break;
		case comma:
			strcpy(expected,"comma");
			break;
		case colon:
			strcpy(expected,"colon");
			break;
		case left_bkt:
			strcpy(expected,"left_bkt");
			break;
		case right_bkt:
			strcpy(expected,"right_bkt");
			break;
		case dotdot:
			strcpy(expected,"dotdot");
			break;
		case assign_op:
			strcpy(expected,"assign_op");
			break;
		case relop_eq:
			strcpy(expected,"relop_eq");
			break;
		case relop_gt:
			strcpy(expected,"relop_gt");
			break;
		case relop_lt:
			strcpy(expected,"relop_lt");
			break;
		case relop_ne:
			strcpy(expected,"relop_ne");
			break;
		case relop_ge:
			strcpy(expected,"relop_ge");
			break;
		case relop_le:
			strcpy(expected,"relop_le");
			break;
		case plus_op:
			strcpy(expected,"plus_op");
			break;
		case sub_op:
			strcpy(expected,"sub_op");
			break;
		case mult_op:
			strcpy(expected,"mult_op");
			break;
		case div_op:
			strcpy(expected,"div_op");
			break;
		case mod_op:
			strcpy(expected,"mod_op");
			break;
		case addop:
			strcpy(expected,"addop");
			break;
		case multop:
			strcpy(expected,"multop");
			break;
	} 
}

