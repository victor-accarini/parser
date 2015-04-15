#include "parser.h"

FILE *trace = NULL;
FILE *tracelex = NULL;
tpToken CTok;
int tabcounter, n;
int idL_list[20];
int idLCounter = 0, varCounter = 0;
int varfuncCounter = 0, varfuncNumber;
char idtype[10];
char arglisttype[500];

void clearIdL_list()
{
	for (int i = 0; i < 20; i++)
	{
		idL_list[i] = -1;
	}
	idLCounter = 0;
}

void parser()
{
	CTok.token = INVALID;
	tabcounter = 0;
	strcpy(idtype, "");
	//Start the log
	trace = fopen("trace.log","w+");
	if (trace == NULL){
		printf("Error: Parser trace log file missing.");
	}
	tracelex = fopen("tracelex.log","w+");
	if (trace == NULL){
		printf("Error: Lex trace log file missing.");
	}

	clearIdL_list();

	fprintf(trace,"Starting the parser.\n");
	n = getToken(&CTok);
	while(CTok.token != MyEOF)
	{
		P();
	}
	
	fprintf(trace,"Closing the parser.\n");
	fclose(trace);
	fclose(tracelex);
}

void P()
{
	ENTER("P");
	MATCH(keywd_program);
	MATCH(id);
	MATCH(l_paren);
	idL();
	MATCH(r_paren);
	MATCH(semi_colon);
	decls();
	subdecls();
	cmpdS();
	EXIT("P");
}

void idL()	
{
	ENTER("idL");
	idL_list[idLCounter++] = n;
	MATCH(id);
	while (CTok.token == comma)
	{
		MATCH(comma);
		idL_list[idLCounter++] = n;
		MATCH(id);
	}
	EXIT("idL");
}

void decls()
{
	ENTER("decls");
	while (CTok.token == keywd_var)
	{
		clearIdL_list();
		MATCH(keywd_var);
		idL();
		varCounter = idLCounter;
		MATCH(colon);
		type();
		MATCH(semi_colon);
	}
	EXIT("decls");
}

void type()
{
	ENTER("type");
	if (CTok.token == keywd_array)
	{
		if (idLCounter > 0)
		{
			for (; idLCounter > 0;)
			{
				changeToken(idL_list[--idLCounter],array_id);
			}
		}
		MATCH(keywd_array);
		MATCH(left_bkt);
		MATCH(num);
		MATCH(dotdot);
		MATCH(num);
		MATCH(right_bkt);
		MATCH(keywd_of);
	}
	idLCounter = 0;
	Stype();
	EXIT("type");
}

void Stype()
{
	ENTER("Stype");
	int i;
	if (CTok.token == keywd_int)
	{
		for(i = 0; i < varCounter; i++)
		{
			changeType(idL_list[i],"integer");
		}
		if (varfuncCounter == 1)
		{
			changeType(varfuncNumber,"integer");
		}
		MATCH(keywd_int);
	}
	else if (CTok.token == keywd_real)
	{
		for(i = 0; i < varCounter; i++)
		{
			changeType(idL_list[i],"real");
		}
		if (varfuncCounter == 1)
		{
			changeType(varfuncNumber,"real");
		}
		MATCH(keywd_real);
	}
	else
	{
		for(i = 0; i < varCounter; i++)
		{
			changeType(idL_list[i],"boolean");
		}
		if (varfuncCounter == 1)
		{
			changeType(varfuncNumber,"boolean");
		}
		MATCH(keywd_bool);
	}
	varCounter = 0;
	EXIT("Stype");
}

void subdecls()
{
	ENTER("subdecls");
	while (CTok.token == keywd_func || CTok.token == keywd_proc)
	{
		subdecl();
		MATCH(semi_colon);
	}
	EXIT("subdecls");
}

void subdecl()
{
	ENTER("subdecl");
	subhead();
	decls();
	cmpdS();
	EXIT("subdecl");
}

void subhead()
{
	int aux = -1;
	ENTER("subhead");
	if (CTok.token == keywd_func)
	{
		MATCH(keywd_func);
		if (CTok.token == id)
		{
			// Change token
			changeToken(n,func_id);
			// Change CT
			CTok.token = func_id;
		}
		varfuncCounter = 1;
		varfuncNumber = n;
		aux = n;
		MATCH(func_id);
		args();
		// Assign the arguments to the function
		strcpy(SymbolTable[aux].funcargs, arglisttype);
		MATCH(colon);
		Stype();
		varfuncNumber = 0;
		MATCH(semi_colon);
	}
	else
	{
		MATCH(keywd_proc);
		if (CTok.token == id)
		{
			// Change token type
			changeToken(n,proc_id);
			// Change CT
			CTok.token = proc_id;
		}
		aux = n;
		MATCH(proc_id);
		args();
		// Assign the arguments to the function
		strcpy(SymbolTable[aux].funcargs, arglisttype);
		MATCH(semi_colon);
	}
	EXIT("subhead");
}

void args()
{
	ENTER("args");
	// Clear arguments type list
	strcpy(arglisttype,"");
	if (CTok.token == l_paren)
	{
		MATCH(l_paren);
		paramL();
		MATCH(r_paren);
	}
	EXIT("args");
}

void paramL()
{	
	ENTER("paramL");
	idL();
	MATCH(colon);
	type();
	while (CTok.token == semi_colon)
	{
		MATCH(semi_colon);
		idL();
		MATCH(colon);
		type();
	}
	EXIT("paramL");
}

void cmpdS()
{
	ENTER("cmpdS");
	MATCH(keywd_begin);
	optS();
	MATCH(keywd_end);
	EXIT("cmpdS");
}

void optS()
{
	ENTER("optS");
	if (CTok.token == id || CTok.token == array_id ||CTok.token == func_id ||CTok.token == proc_id ||
			CTok.token == keywd_begin || CTok.token == keywd_if || CTok.token == keywd_while)
	{
		SL();
	}
	EXIT("optS");
}

void SL()
{
	ENTER("SL");
	S();
	while (CTok.token == semi_colon)
	{
		MATCH(semi_colon);
		S();
	}
	EXIT("SL");
}

void S()
{
	ENTER("S");
	if (CTok.token == keywd_begin)
	{
		cmpdS();
	}
	else if (CTok.token == proc_id)
	{
		procS();
	}
	else if (CTok.token == keywd_if)
	{
		MATCH(keywd_if);
		E();
		MATCH(keywd_then);
		S();
		MATCH(keywd_else);
		S();
	}
	else if (CTok.token == keywd_while)
	{
		MATCH(keywd_while);
		E();
		MATCH(keywd_do);
		S();
	}
	else
	{
		strcpy(idtype, SymbolTable[n].token_type);
		V();
		MATCH(assign_op);
		E();
	}
	EXIT("S");
}

void V()
{
	ENTER("V");
	if (CTok.token == id)
	{
		MATCH(id);
	}
	else if (CTok.token == array_id)
	{
		MATCH(array_id);
		MATCH(left_bkt);
		SE();
		MATCH(right_bkt);
	}
	else
	{
		MATCH(func_id);
	}
	EXIT("V");
}

void procS()
{
	ENTER("procS");
	if (CTok.token == id)
	{
		// Change token type
		changeToken(n,proc_id);
		// Change CT
		CTok.token = proc_id;
	}
	MATCH(proc_id);
	if (CTok.token == l_paren)
	{
		MATCH(l_paren);
		EL();
		MATCH(r_paren);
	}
	EXIT("procS");
}

void EL()
{
	ENTER("EL");
	E();
	while (CTok.token == comma)
	{	
		MATCH(comma);
		E();
	}
	EXIT("EL");
}

void E()
{
	ENTER("E");
	SE();
	if (CTok.token == relop_eq)
	{
		MATCH(relop_eq);
		SE();
	}
	else if (CTok.token == relop_ge)
	{
		MATCH(relop_ge);
		SE();
	}
	else if (CTok.token == relop_gt)
	{
		MATCH(relop_gt);
		SE();
	}
	else if (CTok.token == relop_le)
	{
		MATCH(relop_le);
		SE();
	}
	else if (CTok.token == relop_lt)
	{
		MATCH(relop_lt);
		SE();
	}
	else if (CTok.token == relop_ne)
	{
		MATCH(relop_ne);
		SE();
	}
	EXIT("E");
}

void SE()
{
	ENTER("SE");
	if (CTok.token == plus_op)
	{
		MATCH(plus_op);
	}
	else if (CTok.token == sub_op)
	{
		MATCH(sub_op);
	}
	T();
	while (CTok.token == plus_op || CTok.token == sub_op || CTok.token == mod_op)
	{
		if (CTok.token == plus_op)
		{
			MATCH(plus_op);
		}
		else if (CTok.token == sub_op)
		{
			MATCH(sub_op);
		}
		else if (CTok.token == mod_op)
		{
			MATCH(mod_op);
		}
		T();
	}
	EXIT("SE");
}

void T()
{
	ENTER("T");
	F();
	while(CTok.token == mult_op || CTok.token == div_op)
	{
		if (CTok.token == mult_op)
		{
			MATCH(mult_op);
		}	
		else if (CTok.token == div_op)
		{
			MATCH(mult_op);
		}
		F();
	}
	EXIT("T");
}

void F()
{
	ENTER("F");
	if (CTok.token == id)
	{
		if (strcmp(idtype,SymbolTable[n].token_type) != 0)
		{
			fprintf(trace, "Warning: Expected variable of type %s\n",idtype); 
		}
		MATCH(id);
	}
	else if (CTok.token == func_id)
	{
		if (strcmp(idtype,SymbolTable[n].token_type) != 0)
		{
			fprintf(trace, "Warning: Expected variable of type %s\n",idtype); 
		}
		MATCH(func_id);
		MATCH(l_paren);
		EL();
		MATCH(r_paren);
	}
	else if (CTok.token == num)
	{
		if (strcmp(idtype,SymbolTable[n].token_type) != 0)
		{
			fprintf(trace, "Warning: Expected variable of type %s\n",idtype); 
		}
		MATCH(num);
	}
	else if (CTok.token == l_paren)
	{
		MATCH(l_paren);
		E();
		MATCH(r_paren);
	}
	else if (CTok.token == keywd_not)
	{
		MATCH(keywd_not);
		F();
	}
	else //(CTok.token == array_id)
	{
		if (strcmp(idtype,SymbolTable[n].token_type) != 0)
		{
			fprintf(trace, "Warning: Expected variable of type %s\n",idtype); 
		}
		MATCH(array_id);
		MATCH(left_bkt);
		SE();
		MATCH(right_bkt);
	}
	EXIT("F");
}


void MATCH(enum tpType tok)
{
	char expected[50];
	char received[50];
	if (SymbolTable[n].token == tok)
	{
		ToAscii(tok, received, n);
		fprintf(trace,"%s\n",received);
		n = getToken(&CTok);
	}
	else
	{
		ToAscii(tok, expected, -1);
		ToAscii(SymbolTable[n].token, received, n);
		fprintf(trace,"Error: Expected token %s, received %s\n",expected,received);
		exit(-1);
	}
}

void ENTER(char *str)
{
	int i;
	for (i = 0; i < tabcounter; i++)
	{
		fprintf(trace,"\t");
	}
	fprintf(trace,"-->%s\n", str);
	tabcounter++;
}

void EXIT(char *str)
{
	int i;
	tabcounter--;
	for (i = 0; i < tabcounter; i++)
	{
		fprintf(trace,"\t");
	}
	fprintf(trace,"<--%s\n", str);
}
