/*************************************************************************
 * Conflite - A micro configuration system for C projects.               *
 * conflite.c                                                            *
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

#include "conflite.h"

int yyparse();

/**
 * Main initialization process for conflite. This will set our configuration
 * table to nil, and make sure we have a config file to parse. If we have
 * a file that is accessible we start yyparse() which will begin the parsing
 * sequence and build conflite.table
 */
void conflite_init()
{
  conflite.table = NULL;

  char *conf = conflite_file();

  if (conf)
    {
      yyin = fopen(conf, "r");
      yyparse();
      fclose(yyin);
    }
  else
    {
      printf("ERROR: No configuration file found! Please set: '%s'\n", conflite.file);
      exit(EXIT_FAILURE);
    }
}

/**
 * Return configuration file based on path. The local folder
 * takes precedence over the HOME directory. Therefore if a .conflite
 * file is located in the current file, it will return that file. If not,
 * it will check to see if there is a .conflite file found in HOME.
 */
char *conflite_file()
{
  if (!conflite.file)
    {
      conflite.file = "./.conflite";
    }

  char *lookup[CONFLITE_FILE_LOOKUP_TABLE];
  lookup[0]  = conflite.file;

  char *home = getenv("HOME");
  char *path = "/.conflite";
  size_t len = strlen(home) + strlen(path) + 1;

  char *full = malloc(len);

  strcpy(full, home);
  strcat(full, path);

  lookup[1] = full;

  free(full);

  int i;

  for (i = 0; i <= (CONFLITE_FILE_LOOKUP_TABLE - 1); i++)
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
void conflite_create(char *key, char *value, char *type)
{
  /* if head has not been initialized yet */
  if (conflite.table == NULL)
    {
      conflite.table        = malloc(sizeof(hash_t));
      conflite.table->key   = key;
      conflite.table->value = value;
      conflite.table->type  = type;
      conflite.table->next  = NULL;
    }
  else
    {
      hash_t *head = conflite.table;
      hash_t *tmp  = conflite_find(key);

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
  if (conflite_true("debug_print_table"))
    {
      conflite_print_table();
    }
}

/**
 * Assert truthiness of a string. Since non existent values return as
 * FALSE, we have to first make sure our passed argument isn't FALSE.
 * then, if the string is 'true', 'TRUE', or '1', we consider it to be
 * true and return t/f.
 */
int conflite_assert(char *str)
{
  if (str == FALSE                ||
      (!conflite_streql(str, "true") &&
       !conflite_streql(str, "TRUE") &&
       !conflite_streql(str, "1")))
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
void conflite_print_table()
{
  hash_t *head = conflite.table;
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
void conflite_free()
{
  hash_t *tmp;
  hash_t *table;

  table = conflite.table;
  
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
hash_t *conflite_find(char *key)
{
  hash_t *head;
  head = conflite.table;

  while (head != NULL)
    {
      if (conflite_streql(head->key, key))
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
int conflite_streql(char *str1, char *str2)
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
 * This utilizes conflite_value() to fetch the entire node, then return only
 * the value itself. This is useful for fetching back the raw node's value itself
 * instead of the entire node.
 */
char *conflite_value(char *key)
{
  hash_t *tmp = conflite_find(key);

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
int conflite_value_print(char *key)
{
  char *val = conflite_value(key);
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
int conflite_true(char *key)
{
  if (conflite_assert(conflite_value(key)))
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
void conflite_set_file(char *file)
{
  conflite.file = file;
}
