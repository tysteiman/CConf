%{

#include <stdio.h>
#include "cconf.h"

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
		cconf_create($1, $3);
	}
	;
