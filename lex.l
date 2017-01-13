%{

#include <string.h>
#include "y.tab.h"

void yyerror(char * str);

%}

%%

[#].*\n		;	/* Ignore comments */
;;.*\n		;	/* Alternate comment */

[a-zA-Z0-9_]+	{
			yylval.string = strdup(yytext);
			return VARIABLE;
		}

[="]		return *yytext;

[ \t\n]		;

.		yyerror("Unknown character.");

%%
