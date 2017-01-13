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
 * that allows us to enter as many records as we need. This is
 * to be included inside of a cconf_t struct and it should be
 * handled that way for the sake of not having 5,000 structs
 * throughout the application that are all connected.
 * @todo right now the type is only signifying the type of the
 *       value without ever getting parsed or returned as such.
 *       This should not be the default however it would be nice
 *       to have a helper function that would return what the
 *       variable really is when we want to be lazy.
 */
typedef struct Hash {
  char        * key;    /* Node's identifying key */
  char        * value;  /* Node's value */
  char        * type;   /* Node's type (string, integer) */
  struct Hash * next;   /* Ptr to next node */
} hash_t;

/**
 * The abstraction around the lookup table itself which also
 * contains other program meta data such as the default file name
 * of the configuration to be parsed. This should be the only struct
 * that is directly accessed throughout the program, everything
 * should have to be set through cconf.table->value, etc.
 */
typedef struct {
  char   * file;   /* Configuration file's name */
  hash_t * table;  /* hash_t ptr for the lookup table */
} cconf_t;

/* Initialize lookup table so we can access it throughout. */
cconf_t cconf;

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
void     cconf_set_file(char *file);

#endif
