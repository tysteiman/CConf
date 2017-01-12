#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

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
  else {
    hash_t *head = table;

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

hash_t *configure_lookup(char *key)
{
  hash_t *head;
  head = table;

  while (head != NULL)
    {
      if (strncmp(head->key, key, strlen(key)) == 0)
        {
          printf("Key: %s --> %s\n", head->key, head->value);
          return head;
        }
      else
        {
          head = head->next;
        }
    }

  printf("Key %s not found.\n", key);
  return FALSE;
}
