/*-------------------------------------------------------------------------*
 *---									---*
 *---		assign4.h						---*
 *---									---*
 *---	    This file declares common classes, vars and functions used	---*
 *---	by the YACC-style grammar for the language at the bottom of	---*
 *---	page 173 of Fischer, Cytron, LeBlanc "Crafting a Compiler"	---*
 *---	Addison-Wesley, 2010.						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2016 May 25		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/


//--			Common inclusions:				--//

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	<string>
#include	"NumberList.h"


//--		Declaration of constants and global vars:		--//
//  PURPOSE:  To tell the maximum allowed length of C-strings.
#define	LINE_LEN	256

//  PURPOSE:  To tell the FILE* object from which 'yylex()' should read.
extern	FILE*		yyin;

//  PURPOSE:  To show the error message pointed to by 'cPtr'.  Returns '0'.
extern	int		yyerror		(const char *cPtr);

//  PURPOSE:  To return the integer that encodes the next read token, or '0'
//	if there are no more tokens to read.
extern	int		yylex		();

//  PURPOSE:  To attempt to parse the input.  No parameters.  Returns '0'.
extern	int		yyparse		();

//  PURPOSE:  To hold the value that results from parse.
extern	double		result;

#define	MIN(x,y)	(((x)<(y)) ? (x) : (y))
