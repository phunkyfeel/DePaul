/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BEGIN_P = 258,
     END_P = 259,
     PRINT = 260,
     READ = 261,
     IF = 262,
     WHILE = 263,
     DO = 264,
     PRINTLN = 265,
     NOTEQUALTO = 266,
     EQUALTO = 267,
     EQUAL = 268,
     GREATER = 269,
     LESS = 270,
     MINUS = 271,
     PLUS = 272,
     SLASH = 273,
     STAR = 274,
     NOT = 275,
     UMINUS = 276,
     VARIABLE = 277,
     NUMBER = 278,
     STRING = 279,
     ERROR = 280
   };
#endif
/* Tokens.  */
#define BEGIN_P 258
#define END_P 259
#define PRINT 260
#define READ 261
#define IF 262
#define WHILE 263
#define DO 264
#define PRINTLN 265
#define NOTEQUALTO 266
#define EQUALTO 267
#define EQUAL 268
#define GREATER 269
#define LESS 270
#define MINUS 271
#define PLUS 272
#define SLASH 273
#define STAR 274
#define NOT 275
#define UMINUS 276
#define VARIABLE 277
#define NUMBER 278
#define STRING 279
#define ERROR 280




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 9 "finalLang.y"
{
  Statement*	statePtr_;
  char*		charPtr_;
  double	const_;
  std::string*	stringPtr_;
}
/* Line 1529 of yacc.c.  */
#line 106 "finalLang.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

