# CConf
A micro configuration system for C projects.

## Installation
To install CConf, simply pull down the repository and run: 

`make`

If you already have a Makefile  in your repository, be aware because this will overwrite that as of right now.

## Usage
### Main
In order to actually use CConf in your project, all you need is to include "cconf.h" and call the intializer:

`#include "cconf.h"`

`cconf_init();`

in your main() function. Also, at the end of your program or when you are done using configurations you will need to call:
	
`cconf_free();`

### Using configuration variables
#### .cconf file
In order to create a variable for use you need a .cconf file either in your project directory or in your HOME directory (~/.cconf). It is preferred you have it in your project directory, as this will always take precedence over your HOME directory.

#### Setting variables
Inside of your .cconf file simply set variables as such:

`foo = bar`

#### Retrieving variables
`cconf_value("foo");` will in this case return "bar". That simple!
##### Note
All you need to do to return values is create a char ptr (char *val = cconf_value('foo')).

If you want to overwrite a node's value, you can retrieve the entire node by using:

`cconf_find("foo")`

This will retire an entire node containing key, value, & the next k/v pair in the table.

`hash_t *lookup = cconf_find("foo");  printf("%s\n", lookup->value);`

Would in this case print "bar".

### System Variables
There are some system variables that you can set in your .cconf file.

- debug_print_table	
  - When true, TRUE, or 1 CConf will print the entire lookup table after each new entry. This is handy for debugging variables and getting a look at the current lookup table after a new entry.