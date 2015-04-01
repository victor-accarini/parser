#include "lex.h"

tpType CT; // Current Token

int getToken(tpToken *token)
{
	int value; // Holds the numerical value, in case it's a number
	int bptr; // Holds the next position in the buffer
	int n = -1;
	char name[255]; // Buffer for the token

	// Start the variables
	value = 0;
	bptr = 0;
	CT = INVALID;
	strcpy(name, "");

	while (isSpace(ch)) // Get rid of white spaces, tabulations, etc..
	{
		ch = getaChar();
	}
	if (isAlpha(ch)) // Check for ID, [A-Za-z_][A-Za-z0-9_]*
	{
		CT = id; // Set the state for the token
		name[bptr++] = ch;
		ch = getaChar();
		while (isAlphaNum(ch))
		{
			name[bptr++] = ch;
			ch = getaChar();
		}
		name[bptr] = '\0'; // Close string
	}
	else if (isNumeric(ch)) //Check for Number, [0-9]+
	{
		CT = num; // Set the state for the token
		name[bptr++] = ch;
		ch = getaChar();
		while (isNumeric(ch))
		{
			name[bptr++] = ch;
			ch = getaChar();
		}
		name[bptr] = '\0'; //Close string
		value = atoi(name);
	}
	else if (ch == '+' || ch == '-')
	{
		CT = addop;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == '*' || ch == '/')
	{
		CT = multop;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == '(')
	{
		CT = l_paren;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == ')')
	{
		CT = r_paren;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == ';')
	{
		CT = semi_colon;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == ',')
	{
		CT = comma;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == ':')
	{
		CT = colon;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
		if (ch == '=')
		{
			CT = assign_op;
			name[bptr++] = ch;
			name[bptr] = '\0';
			ch = getaChar();
		}
	}
	else if (ch == '[')
	{
		CT = left_bkt;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == ']')
	{
		CT = right_bkt;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
	}
	else if (ch == '.')
	{
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
		if (ch == '.')
		{
			CT = dotdot;
			name[bptr++] = ch;
			name[bptr] = '\0';
			ch = getaChar();
		}
	}
	else if (ch == '=')
	{
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
		if (ch == '=')
		{
			CT = relop_eq;
			name[bptr++] = ch;
			name[bptr] = '\0';
			ch = getaChar();
		}
	}
	else if (ch == '>')
	{
		CT = relop_gt;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
		if (ch == '=')
		{
			CT = relop_ge;
			name[bptr++] = ch;
			name[bptr] = '\0';
			ch = getaChar();
		}
	}
	else if (ch == '<')
	{
		CT = relop_lt;
		name[bptr++] = ch;
		name[bptr] = '\0';
		ch = getaChar();
		if (ch == '>')
		{
			CT = relop_ne;
			name[bptr++] = ch;
			name[bptr] = '\0';
			ch = getaChar();
		}
		else if (ch == '=')
		{
			CT = relop_le;
			name[bptr++] = ch;
			name[bptr] = '\0';
			ch = getaChar();
		}
	}
	else if (ch == EOF || ch == '$')
	{
		CT = MyEOF;
	}

	// Insert the name and token type,
	// leave variable type and value to the parser
	token->token = CT;
	strcpy(token->token_name,name);
	strcpy(token->token_type,"");
	token->token_value = value;

	if (CT != MyEOF && CT != character)
	{
		fprintf(tracelex,"Token %s found!\n",token->token_name);
		if ((n = lookup(token->token_name)) == -1)
		{
			fprintf(tracelex,"Token %s inserted in SymbolTable.\n", token->token_name);
			n = insert(*token);
		}
		else
		{
			fprintf(tracelex,"Token %s is already in the SymbolTable.\n", token->token_name);
		}
	}

	return n;
}

int isAlpha(char ch)
{
	if ( (ch > 64 && ch < 91) || (ch > 96 && ch < 123) || ch == 95)
		return 1;
	else
		return 0;
}

int isAlphaNum(char ch)
{
	if ( (ch > 64 && ch < 91) || (ch > 96 && ch < 123) || ch == 95 || (ch > 47 && ch < 58))
		return 1;
	else
		return 0;
}

int isNumeric(char ch)
{
	if ((ch > 47 && ch < 58))
		return 1;
	else
		return 0;
}

int isSpace(char ch)
{
	if ((ch == 32) || (ch > 8 && ch < 13))
		return 1;
	else
		return 0;
}

char getaChar()
{
	lineptr++;
	if (*lineptr == '\n' && feof(sourcefile) == 0)
	{
		getaLine();
	}
	else if (feof(sourcefile) != 0)
	{
		return EOF;
	}
	return *lineptr;
}

void getaLine()
{
	fgets(line, 250, sourcefile);
	lineptr = line;
}
