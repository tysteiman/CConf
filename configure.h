#ifndef CONFIGURE_LIB
#define CONFIGURE_LIB

extern FILE * yyin;

#define TRUE 1
#define FALSE 0

typedef struct Hash {
  char *key;
  char *value;
  struct Hash *next;
} hash_t;

hash_t *table;

void configure_create(char *key, char *value);
void configure_print_table();
void configure_free(hash_t *table);
hash_t *configure_find(char *key);

#endif
