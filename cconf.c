/*************************************************************************
 * CConf - A micro configuration system for C projects.                  *
 * cconf.c                                                               *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cconf.h"

int yyparse();

/**
 * Main initialization process for cconf. This will set our configuration
 * table to nil, and make sure we have a config file to parse. If we have
 * a file that is accessible we start yyparse() which will begin the parsing
 * sequence and build cconf.table
 */
void cconf_init()
{
  cconf.table = NULL;

  char *conf = cconf_file();

  if (conf)
    {
      yyin = fopen(conf, "r");
      yyparse();
      fclose(yyin);
    }
  else
    {
      printf("ERROR: No configuration file found! Please set: '%s'\n", cconf.file);
      exit(EXIT_FAILURE);
    }
}

/**
 * Return configuration file based on path. The local folder
 * takes precedence over the HOME directory. Therefore if a .cconf
 * file is located in the current file, it will return that file. If not,
 * it will check to see if there is a .cconf file found in HOME.
 */
char *cconf_file()
{
  if (!cconf.file)
    {
      cconf.file = "./.cconf";
    }

  char *lookup[CCONF_FILE_LOOKUP_TABLE];
  lookup[0]  = cconf.file;

  char *home = getenv("HOME");
  char *path = "/.cconf";
  size_t len = strlen(home) + strlen(path) + 1;

  char *full = malloc(len);

  strcpy(full, home);
  strcat(full, path);

  lookup[1] = full;

  free(full);

  int i;

  for (i = 0; i <= (CCONF_FILE_LOOKUP_TABLE - 1); i++)
    {
      if (access(lookup[i], F_OK|R_OK) != -1)
        {
          return lookup[i];
        }
    }

  return FALSE;
}

/**
 * Create an entry into the configuration lookup table. If it is not
 * initialized (if set to NULL) it will initialize the table and set
 * the initial values as the key/value args passed. Otherwise it simply
 * creates another key/value record in the table for later.
 */
void cconf_create(char *key, char *value, char *type)
{
  /* if head has not been initialized yet */
  if (cconf.table == NULL)
    {
      cconf.table        = malloc(sizeof(hash_t));
      cconf.table->key   = key;
      cconf.table->value = value;
      cconf.table->type  = type;
      cconf.table->next  = NULL;
    }
  else
    {
      hash_t *head = cconf.table;
      hash_t *tmp  = cconf_find(key);

      /* Simply overwrite the value if key already exists. */
      if (tmp)
        {
          tmp->value = value;
        }
      /* Log new entry when key is unique (not entered) */
      else
        {
          while (head->next != NULL)
            {
              head = head->next;
            }

          head->next        = malloc(sizeof(hash_t));
          head->next->key   = key;
          head->next->value = value;
          head->next->type  = type;
          head->next->next  = NULL;
        }
    }

  /* if debug_print_table is set we need to print the whole table. */
  if (cconf_true("debug_print_table"))
    {
      cconf_print_table();
    }
}

/**
 * Assert truthiness of a string. Since non existent values return as
 * FALSE, we have to first make sure our passed argument isn't FALSE.
 * then, if the string is 'true', 'TRUE', or '1', we consider it to be
 * true and return t/f.
 */
int cconf_assert(char *str)
{
  if (str == FALSE                ||
      (!cconf_streql(str, "true") &&
       !cconf_streql(str, "TRUE") &&
       !cconf_streql(str, "1")))
    {
      return FALSE;
    }
  else
    {
      return TRUE;
    }
}

/**
 * Used primarily for debugging, this simply loops through the lookup table
 * and prints each key/value pair.
 */
void cconf_print_table()
{
  hash_t *head = cconf.table;
  while (head != NULL)
    {
      printf("Key: %s\t\t-->\t\t%s\t(%s)\n", head->key, head->value, head->type);
      head = head->next;
    }
  printf("-------------------------------------------------\n");
}

/**
 * Loop through the lookup table and free each node.
 */
void cconf_free(hash_t *table)
{
  hash_t *tmp;

  while (table != NULL)
    {
      tmp = table;
      table = table->next;
      free(tmp);
    }
}

/**
 * Find a specific entry in the lookup table by key. If the key is not
 * present it will return FALSE. If the key is found, it will return
 * the entire node, not just the value itself. This is so values can
 * be changed depending on the result of this function when needed
 * (mainly for overwriting values).
 */
hash_t *cconf_find(char *key)
{
  hash_t *head;
  head = cconf.table;

  while (head != NULL)
    {
      if (cconf_streql(head->key, key))
        {
          return head;
        }
      else
        {
          head = head->next;
        }
    }

  return FALSE;
}

/**
 * Simple wrapper around strncmp to compare 2 strings. Return integer
 * based on whether the two strings match or not.
 */
int cconf_streql(char *str1, char *str2)
{
  if (strncmp(str1, str2, strlen(str2)) == 0)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

/**
 * Find a specific entry from the lookup table by key however this will
 * return the actual string value of 'value' if found. Otherwise return false.
 * This utilizes cconf_value() to fetch the entire node, then return only
 * the value itself. This is useful for fetching back the raw node's value itself
 * instead of the entire node.
 */
char *cconf_value(char *key)
{
  hash_t *tmp = cconf_find(key);

  if (!tmp)
    {
      return FALSE;
    }
  else
    {
      return tmp->value;
    }
}

/**
 * Print a single value if exists. If the value does not exist return FALSE.
 * When the value exists it will both print & return TRUE.
 */
int cconf_value_print(char *key)
{
  char *val = cconf_value(key);
  if (val == FALSE)
    {
      return FALSE;
    }
  else
    {
      printf("%s\n", val);
      return TRUE;
    }
}

/**
 * A simple wrapper around asserting a value.
 */
int cconf_true(char *key)
{
  if (cconf_assert(cconf_value(key)))
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

/**
 * Setter for custom configuration file to be parsed.
 */
void cconf_set_file(char *file)
{
  cconf.file = file;
}
