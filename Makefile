all:
	lex *.l
	yacc -d *.y
	if [ ! -d ./bin ]; then mkdir bin; fi
	gcc -o bin/configure *.c

run:
	make
	if [ -f ./configure ]; then ./configure; fi
