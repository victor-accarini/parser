#include "lex.h"
#include "parser.h"
#include "symbol_table.h"

FILE *sourcefile;
char *line,*lineptr, ch;
tpToken SymbolTable[Tsize];

int main()
{
	// Initialize the symbol table
	init();
	sourcefile = NULL;
	line = (char *)malloc(270*sizeof(char));
	
	// Open source file
	sourcefile = fopen("source.txt","r");

	if (sourcefile == NULL)
	{
		printf("Couldn't find the file.\n");
		perror("Error");
		return 1;
	}

	// Set the beginning of the file
	fgets(line, 250, sourcefile);
	ch = line[0];
	lineptr = line;

	// Call the parser, returns the complete symbol table
	parser();
	report();
	fclose(sourcefile);
	// --------------------
	return 0;
}