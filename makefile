all:
	gcc -w main.cpp lex.cpp parser.cpp symbol_table.cpp -o parser
