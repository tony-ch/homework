user-sh : bison.tab.o execute.o lex.yy.o execOuterCmd.o
	cc -Wall -o user-sh bison.tab.o execute.o lex.yy.o execOuterCmd.o
bison.tab.o : bison.tab.c global.h
	cc -c -Wall bison.tab.c
execute.o : execute.c global.h
	cc -c -Wall execute.c
execOuterCmd.o : execOuterCmd.c global.h
	cc -c -Wall execOuterCmd.c
lex.yy.o : lex.yy.c
	cc -c -Wall lex.yy.c
lex.yy.c :
	lex shell.lex
bison.tab.c bison.tab.h:
	bison -d bison.y
clean :
	rm user-sh bison.tab.o execute.o bison.tab.c bison.tab.h lex.yy.o lex.yy.c execOuterCmd.o
