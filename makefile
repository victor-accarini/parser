all:
	gcc -w -g main.cpp lex.cpp parser.cpp symbol_table.cpp -o parser
