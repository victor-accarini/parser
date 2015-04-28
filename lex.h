#ifndef LEXICAL_ANALYSER
#define LEXICAL_ANALYSER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern char *line;
extern char *lineptr;
extern char ch;
extern char scope[50];
extern FILE *sourcefile;
extern FILE *trace;
extern FILE *tracelex;
extern FILE *errorfile;

/* --------------------------------------------- */
/*             EXTERNAL FUNCTIONS                */
/* --------------------------------------------- */
int getToken(tpToken *token);

/* --------------------------------------------- */
/*           VERIFICATION FUNCTIONS              */
/* --------------------------------------------- */
int isAlpha(char ch);
int isAlphaNum(char ch);
int isNumeric(char ch);
int isSpace(char ch);

/* --------------------------------------------- */
/*            INTERNAL FUNCTIONS                 */
/* --------------------------------------------- */
char getaChar();
void getaLine();

#endif
