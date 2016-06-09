# Makefile for finalLang
finalLang	: finalLang.tab.o finalLang.o
		g++ -o $@ finalLang.tab.o finalLang.o


finalLang.o	: finalLang.h finalLang.c finalLang.tab.h VarStore.h
		g++ -c finalLang.c -g


finalLang.tab.o : finalLang.h finalLang.tab.c finalLang.tab.h \
		  VarStore.h Statement.h
		g++ -c finalLang.tab.c -g


finalLang.c	: finalLang.lex
		flex -o$@ finalLang.lex


finalLang.tab.c : finalLang.y
		bison -d finalLang.y --debug --verbose

finalLang.tab.h : finalLang.y
		bison -d finalLang.y --debug --verbose
