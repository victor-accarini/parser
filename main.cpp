#include "lex.h"
#include "parser.h"
#include "symbol_table.h"

FILE *sourcefile;
char *line,*lineptr, ch;
tpToken SymbolTable[Tsize];

int main(int argc, char *argv[])
{
	// Initialize the symbol table
	init();
	sourcefile = NULL;
	line = (char *)malloc(270*sizeof(char));
	
	if (argc != 2)
	{
		printf("Error: Incorrect use of program.\n");
		printf("Usage: ./parser <sourcefile>.\n");
		exit(1);
	}
	// Open source file
	sourcefile = fopen(argv[1],"r");

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
