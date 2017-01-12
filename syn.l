%{

#include <string.h>
#include "y.tab.h"

void yyerror(char * str);

%}

%%

[a-zA-Z0-9]+	{
			yylval.string = strdup(yytext);
			return VARIABLE;
		}

[\n="]		return *yytext;

[ \t]		;

.		yyerror("Unknown character.");

%%
