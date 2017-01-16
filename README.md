# Conflite
A micro configuration system for C projects.

## Installation
To install Conflite, simply pull down the repository and run: 

`make`

If you already have a Makefile  in your repository, be aware because this will overwrite that as of right now.

## Usage
### Main
In order to actually use Conflite in your project, all you need is to include "conflite.h" and call the intializer:

`#include "conflite.h"`

`conflite_init();`

in your main() function. Also, at the end of your program or when you are done using configurations you will need to call:
	
`conflite_free();`

### Using configuration variables
#### .conflite file
In order to create a variable for use you need a .conflite file either in your project directory or in your HOME directory (~/.conflite). It is preferred you have it in your project directory, as this will always take precedence over your HOME directory.

##### Overwriting the default file
The default configuration file may be overwritten by setting the conflite.file variable in your host program (wherever your main function is). This must be set before calling conflite_init();

`conflite_set_file("./bin/target.ini");`

This will change the default configuration to be parsed from ./.conflite to ./bin/target.ini.

#### Setting variables
Inside of your .conflite file simply set variables as such:

`foo = bar`

#### Retrieving variables
`conflite_value("foo");` will in this case return "bar". That simple!
##### Note
All you need to do to return values is create a char ptr (char *val = conflite_value('foo')).

If you want to overwrite a node's value, you can retrieve the entire node by using:

`conflite_find("foo")`

This will retire an entire node containing key, value, & the next k/v pair in the table.

`hash_t *lookup = conflite_find("foo");  printf("%s\n", lookup->value);`

Would in this case print "bar".

### System Variables
There are some system variables that you can set in your .conflite file.

- debug_print_table	
  - When true, TRUE, or 1 Conflite will print the entire lookup table after each new entry. This is handy for debugging variables and getting a look at the current lookup table after a new entry.
  
## Sample Host Program
*test.c*
```

#include "conflite.h"

int main()
{
  conflite_set_file("./bin/target");
  conflite_init();
  conflite_free();
}

```

## Host program using conflite value
*test.c*
```

#include "conflite.h"

void log_res(int a, int b)
{
  int res = a + b;

  if (conflite_true("print_all_results"))
    {
      printf("Result is: %d\n", res);
    }
}

int main()
{
  conflite_set_file("./bin/target");

  conflite_init();

  int i = 5;
  int q = 10;

  log_res(i, q);

  conflite_free();
}

```
