%{

#include <string.h>
#include "y.tab.h"

void yyerror(char * str);

%}

%%

[#].*\n		;	/* Ignore comments */

[a-zA-Z0-9]+	{
			yylval.string = strdup(yytext);
			return VARIABLE;
		}

[\n="]		return *yytext;

[ \t]		;

.		yyerror("Unknown character.");

%%
