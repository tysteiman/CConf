#ifndef CONFIGURE_LIB
#define CONFIGURE_LIB

/* We need to be able to change which file is getting parsed. */
extern FILE * yyin;

#define TRUE 1
#define FALSE 0

/**
 * Main structure for our lookup table. A simple linked list
 * that allows us to enter as many records as we need.
 */
typedef struct Hash {
  char *key;
  char *value;
  struct Hash *next;
} hash_t;

/* Initialize lookup table so we can access it throughout. */
hash_t *table;

/* BEGIN DECLS */
void configure_create(char *key, char *value);
void configure_print_table();
void configure_free(hash_t *table);
hash_t *configure_find(char *key);
int configure_streql(char *str1, char *str2);
char *configure_value(char *key);

#endif
