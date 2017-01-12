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
	'\n'
	|
	VARIABLE '=' VARIABLE
	{
		configure_create($1, $3);
		configure_print_table();
	}
	|
	VARIABLE
	{
		char *val = configure_value($1);

		if (val != FALSE)
		{
			printf("%s\n", val);
		}
		else
		{
			printf("ERROR: Key '%s' does not exist.\n", $1);
		}
	}
	;
