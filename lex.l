/*************************************************************************
 * CConf - A micro configuration system for C projects.                  *
 * lex.l                                                                 *
 *                                                                       *
 * Copyright (C) 2017  Tyler M. Steiman                                  *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ************************************************************************/

%{

#include <string.h>
#include "y.tab.h"

void yyerror(char * str);

%}

%%

[#].*\n         ;       /* Ignore comments */
;;.*\n          ;       /* Alternate comment */

[a-zA-Z0-9_\-]+ {
                        yylval.string = strdup(yytext);
                        return VARIABLE;
                }

\"(\\.|[^\"])*\" {
                        yylval.string = strdup(yytext);
                        return STRING;
                 }

[="]            return *yytext;

[ \t\n]         ;

.               yyerror("Unknown character.");

%%
