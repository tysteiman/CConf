%{

#include <stdio.h>
#include "configure.h"

int yywrap()
{
	return 1;
}

void yyerror(char * str)
{
	fprintf(stderr, "%s\n", str);
}

int yylex();

%}

%union {
	char * string;
}

%token <string> VARIABLE

%%

program:
	program statement '\n'
	|
	;

statement:
	VARIABLE '=' VARIABLE
	{
		configure_create($1, $3);
		configure_print_table();
	}
	|
	VARIABLE
	{
		hash_t *tmp;
		tmp = configure_find($1);

		if (tmp != FALSE)
		{
			printf("%s\n", tmp->value);
		}
		else
		{
			printf("ERROR: Key '%s' does not exist.\n", $1);
		}
	}
	;
