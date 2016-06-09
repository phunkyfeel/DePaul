%{
//  finalLang.lex
//  unix> flex -o finalLang.c finalLang.lex
//  unix> gcc finalLang.c -c
//  unix> gcc -o finalLang finalLang.tab.o finalLang.o

#include	"finalLang.h"
#include	"finalLang.tab.h"

#undef		YY_INPUT
#define		YY_INPUT(buffer,result,maxSize)		\
		{ result = ourInput(buffer,maxSize); }

extern
int		ourInput(char* buffer, int maxSize);

%}

%%

[ \t\n]			{ /* ignore whitespace */ }
"/*"			{
			  /* ignore comments */
			  char	first	= yyinput();
			  char	second	= yyinput();

			  while  ( (first != '*') && (second != '/') )
			  {
			    first	= second;
			    second	= yyinput();
			  }
			}
(\-|\+)?([0-9]+|([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))      { yylval.const_ = strtod(yytext,NULL); return(NUMBER); }
print		{ return(PRINT); }
printLn		{ return(PRINTLN); }
read		{ return(READ); }
while		{ return(WHILE); }
do			{ return(DO); }
if			{ return(IF); }
==          { return(EQUALTO); }
!=          { return(NOTEQUALTO); }
[a-zA-Z_][a-zA-Z_0-9]*	{
			  yylval.charPtr_ = strdup(yytext);
			  return(VARIABLE);
			}
"++"        { return(INCREMENT); }
"--"        { return(DECREMENT); }
"+="        { return(PLUSEQUAL); }
"-="        { return(MINUSEQUAL); }
"*="        { return(MULTEQUAL); }
"/="        { return(DIVEQUAL); }
\+			{ return(PLUS); }
\-			{ return(MINUS); }
\*			{ return(STAR); }
\/			{ return(SLASH); }
\(			{ return(BEGIN_P); }
\)			{ return(END_P); }
\<			{ return(LESS); }
\>			{ return(GREATER); }
\=			{ return(EQUAL); }
\!          { return(NOT); }
\"[^\"]*\"		{
			  yytext[strlen(yytext)-1]	= '\0';
			  yylval.stringPtr_	= new std::string(yytext+1);
			  return(STRING);
			}
.			{
			  printf("What's '%c'?\n",yytext[0]);
			  return(ERROR);
			}

%%

int	yywrap	() { return(1); }



FILE*		filePtr		= NULL;
char*		textPtr		= NULL;
char*		textEndPtr	= NULL;
VarStore	varStore;
Statement*	resultPtr;
bool		havePrinted	= false;

int	yyerror	(const char *cPtr)
{
  printf("%s, sorry!\n",cPtr);
  return(0);
}


int	ourInput(char* buffer, int maxSize)
{
  unsigned int	n;

  if  (filePtr == NULL)
  {
    n	= MIN(maxSize,textEndPtr - textPtr);

    if  (n > 0)
    {
      memcpy(buffer,textPtr,n);
      textPtr	+= n;
    }
  }
  else
  {
    errno	= 0;

    while  ( (n = fread(buffer,1,maxSize,yyin)) == 0 && ferror(yyin))
    {
      if  (errno != EINTR)
      {
        fprintf(stderr,"Reading file failed: %s\n",strerror(errno));
	exit(EXIT_FAILURE);
      }

      errno	= 0;
      clearerr(yyin);
    }

  }

  return(n);
}


int	main	(int argc, char* argv[])
{
  char	line[LINE_LEN];

  if  ( (argc >= 2)  &&  ( (filePtr = fopen(argv[1],"r")) != NULL ) )
  {
    yyin	= filePtr;
   }
  else
  {
    printf("Please enter an expression: ");
    textPtr	= fgets(line,LINE_LEN,stdin);
    textEndPtr	= textPtr + strlen(textPtr);
   }

  yyparse();

  if  (resultPtr != NULL)
  {
    double result = resultPtr->eval();

    if  (!havePrinted)
      printf("%g\n",result);

    delete(resultPtr);
  }

  if  (filePtr != NULL)
    fclose(filePtr);

  return(EXIT_SUCCESS);
}
