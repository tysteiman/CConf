all:
	lex lex.l
	yacc -d parse.y
	if [ ! -d ./bin ]; then mkdir bin; fi
	gcc -o bin/configure *.c

run:
	make
	if [ -f ./bin/configure ]; then ./bin/configure; fi
