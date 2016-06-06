#--------------------------------------------------------------------------#
#---									---#
#---		makefile						---#
#---									---#
#---	    This file defines a Makefile for Lex, Yacc and C files that	---#
#---	implement the language at the bottom of page 173 of Fischer,	---#
#---	Cytron, LeBlanc "Crafting a Compiler" Addison-Wesley, 2010.	---#
#---									---#
#---	----	----	----	----	----	----	----	----	---#
#---									---#
#---	Version 1.2		2015 May 22		Joseph Phillips	---#
#---									---#
#--------------------------------------------------------------------------#

# Makefile for assign4
assign4		: assign4.tab.o assign4.o
	  	g++ -o $@ assign4.tab.o assign4.o


assign4.o	: assign4.c assign4.tab.h assign4.h NumberList.h
	  	g++ -c assign4.c -g


assign4.tab.o	: assign4.tab.c assign4.tab.h assign4.h NumberList.h
	  	g++ -c assign4.tab.c -g


assign4.c	: assign4.lex
		flex -o$@ assign4.lex


assign4.tab.c	: assign4.y
		bison -d assign4.y --debug --verbose

assign4.tab.h	: assign4.y
		bison -d assign4.y --debug --verbose
