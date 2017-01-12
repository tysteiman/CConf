#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configure.h"

int main(int argc, char **argv)
{
  table = NULL;

  if (argc > 1)
    {
      yyin = fopen(argv[1], "r");
    }

  yyparse();

  if (argc > 1)
    {
      fclose(yyin);
    }

  configure_free(table);
}

/**
 * Create an entry into the configuration lookup table. If it is not
 * initialized (if set to NULL) it will initialize the table and set
 * the initial values as the key/value args passed. Otherwise it simply
 * creates another key/value record in the table for later.
 */
void configure_create(char *key, char *value)
{
  /* if head has not been initialized yet */
  if (table == NULL)
    {
      table        = malloc(sizeof(hash_t));
      table->key   = key;
      table->value = value;
      table->next  = NULL;
    }
  else
    {
      hash_t *head = table;
      hash_t *tmp  = configure_find(key);

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
          head->next->next  = NULL;
        }
    }
}

/**
 * Used primarily for debugging, this simply loops through the lookup table
 * and prints each key/value pair.
 */
void configure_print_table()
{
  hash_t *head = table;
  while (head != NULL)
    {
      printf("Key: %s\t\t-->\t\t%s\n", head->key, head->value);
      head = head->next;
    }
  printf("-------------------------------------------------\n");
}

/**
 * Loop through the lookup table and free each node.
 */
void configure_free(hash_t *table)
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
hash_t *configure_find(char *key)
{
  hash_t *head;
  head = table;

  while (head != NULL)
    {
      if (configure_streql(head->key, key))
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
int configure_streql(char *str1, char *str2)
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
 * This utilizes configure_value() to fetch the entire node, then return only
 * the value itself. This is useful for fetching back the raw node's value itself
 * instead of the entire node.
 */
char *configure_value(char *key)
{
  hash_t *tmp = configure_find(key);

  if (!tmp)
    {
      return FALSE;
    }
  else
    {
      return tmp->value;
    }
}
