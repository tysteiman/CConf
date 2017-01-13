/*************************************************************************
 * CConf - A micro configuration system for C projects.                  *
 * cconf.h                                                               *
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

#ifndef CCONF_LIB
#define CCONF_LIB

#include <stdio.h>

/* We need to be able to change which file is getting parsed. */
extern FILE * yyin;

#define TRUE 1
#define FALSE 0
#define CCONF_FILE_LOOKUP_TABLE 2

/**
 * Main structure for our lookup table. A simple linked list
 * that allows us to enter as many records as we need.
 */
typedef struct Hash {
  char        * key;
  char        * value;
  char        * type;
  struct Hash * next;
} hash_t;

typedef struct CConf {
  char   * file;
  hash_t * table;
} cconf_t;

/* Initialize lookup table so we can access it throughout. */
cconf_t   cconf;

/* BEGIN DECLS */
void     cconf_init();
void     cconf_create(char *key, char *value, char *type);
void     cconf_print_table();
void     cconf_free(hash_t *table);
hash_t * cconf_find(char *key);
int      cconf_streql(char *str1, char *str2);
int      cconf_assert(char *str);
char   * cconf_value(char *key);
int      cconf_value_print(char *key);
char   * cconf_file();
int      cconf_true(char *key);

#endif
