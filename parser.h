#ifndef PARSER_ANALYSER
#define PARSER_ANALYSER

#include "symbol_table.h"
#include "lex.h"

void parser();
void P();
void idL();
void idL_();
void decls();
void decls_();
void type();
void Stype();
void subdecls();
void subdecls_();
void subdecl();
void subhead();
void args();
void paramL();
void paramL_();
void cmpdS();
void optS();
void SL();
void SL_();
void S();
void V();
void procS();
void EL();
void EL_();
void E();
void SE();
void SE_();
void T();
void T_();
void F();

int MATCH(tpType tok, char *str);
void ENTER(char *str);
void EXIT(char *str);

#endif