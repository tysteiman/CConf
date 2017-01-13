/*************************************************************************
 * CConf - A micro configuration system for C projects.                  *
 * parse.y                                                               *
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
	int    integer;
}

%token <string>  VARIABLE
%token <string>  STRING
%token <string>  INTEGER

%%

program:
        program statement
        |
	;

statement:
	VARIABLE '=' INTEGER
        {
		cconf_create($1, $3, "integer");
        }
        |       
        VARIABLE '=' VARIABLE
        {
		cconf_create($1, $3, "string");
        }
        |
        VARIABLE '=' STRING
        {
		cconf_create($1, $3, "string");
        }
        ;
