#include "parser.h"

FILE *trace = NULL;
FILE *tracelex = NULL;
tpToken CTok;
int tabcounter, n;

void parser()
{
	CTok.token = INVALID;
	tabcounter = 0;
	//Start the log
	trace = fopen("trace.log","w+");
	if (trace == NULL){
		printf("Error: Parser trace log file missing.");
	}
	tracelex = fopen("tracelex.log","w+");
	if (trace == NULL){
		printf("Error: Lex trace log file missing.");
	}
	fprintf(trace,"Starting the parser.\n");
	n = getToken(&CTok);
	while(CTok.token != MyEOF)
	{
		fprintf(trace, "%s\n", SymbolTable[n].token_type);
		n = getToken(&CTok);
		/*if(!MATCH(CHARACTER,"#"))
		{
			fprintf(trace,"Error: Sentence must start with #\nTerminating program...\n");
			exit(1);
		}*/
		EL();
		/*if (!MATCH(CHARACTER,"#"))
		{
			fprintf(trace,"Error: Sentence must end with #\nTerminating program...\n");
			exit(1);
		}*/
	}
	
	fprintf(trace,"Closing the parser.\n");
	fclose(trace);
	fclose(tracelex);
}

void P()
{
}

void idL()	
{
}

void idL_()
{
}

void decls()
{
}

void decls_(){}
void type(){}
void Stype(){}
void subdecls(){}
void subdecls_(){}
void subdecl(){}
void subhead(){}
void args(){}
void paramL(){}
void paramL_(){}
void cmpdS(){}
void optS(){}
void SL(){}
void SL_(){}
void S(){}
void V(){}
void procS(){}

void EL()
{
	ENTER("EL");
	E();
	EL_();
	EXIT("EL");
}

void EL_()
{
	ENTER("EL\'");
	if (CTok.token == comma)
	{
		MATCH(comma);
		E();
		EL_();
	}
	EXIT("EL\'");
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
	SE_();
	EXIT("SE");
}

void SE_()
{
	ENTER("SE\'");
	if (CTok.token == plus_op)
	{
		MATCH(plus_op);
		T();
		SE_();
	}
	else if (CTok.token == sub_op)
	{
		MATCH(sub_op);
		T();
		SE_();
	}
	else if (CTok.token == mod_op)
	{
		MATCH(mod_op);
		T();
		SE_();
	}
	EXIT("SE\'");
}

void T()
{
	ENTER("T");
	F();
	T_();
	EXIT("T");
}

void T_()
{
	ENTER("T\'");
	if (CTok.token == mult_op)
	{
		MATCH(mult_op);
		F();
		T_();
	}	
	else if (CTok.token == div_op)
	{
		MATCH(mult_op);
		F();
		T_();
	}
	EXIT("T\'");
}

void F()
{
	ENTER("F");
	if (CTok.token == id)
	{
		MATCH(id);
	}
	else if (CTok.token == func_id)
	{
		MATCH(func_id);
		MATCH(l_paren);
		EL();
		MATCH(r_paren);
	}
	else if (CTok.token == num)
	{
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
	else if (CTok.token == array_id)
	{
		MATCH(array_id);
		MATCH(left_bkt);
		SE();
		MATCH(right_bkt);
	}
	else
	{
		//Error;
	}
	EXIT("F");
}

int MATCH(tpType tok)
{
	if (CTok.token == tok)
	{
		getToken(&CTok);
		fprintf(trace,"%s",CTok.token_name);
	}
	else
	{
		//Error
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
