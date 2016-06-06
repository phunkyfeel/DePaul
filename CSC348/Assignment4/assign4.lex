%{

/*-------------------------------------------------------------------------*
*---									---*
*---		assign4.lex						---*
*---									---*
*---	    This file defines the LEX-style tokenizer for the language	---*
*---	at the bottom of page 173 of Fischer, Cytron, LeBlanc "Crafting	---*
*---	 a Compiler" Addison-Wesley, 2010.				---*
*---									---*
*---	----	----	----	----	----	----	----	----	---*
*---									---*
*---	Version 1.0		2016 May 25		Joseph Phillips	---*
*---									---*
*-------------------------------------------------------------------------*/

#include		"assign4.h"
#include		"assign4.tab.h"

#undef			YY_INPUT
#define			YY_INPUT(buffer,result,maxSize)		\
                { result = ourInput(buffer,maxSize); }

extern int		ourInput(char* buffer, int maxSize);

%}

%%

[ \t\n\r]                           { }
(\-|\+)?([0-9]+|([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))      { yylval.number_ = strtod(yytext,NULL); return(NUM); }
\(                                                      { return(LPAREN); }
\)                                                      { return(RPAREN); }
\+                                                      { return(PLUS); }
\*                                                      { return(STAR); }
.                                                       {
                                                            printf("Unexpected char %c encountered.\n", yytext[0]);
                                                            exit(EXIT_FAILURE); }


%%

//  PURPOSE:  To hold the input file from which to read the program (if it is
//	not 'stdin').
FILE*		filePtr		= NULL;

//  PURPOSE:  To point to the beginning of the input yet to read (if being
//	typed from the command line).
char*		textPtr		= NULL;


//  PURPOSE:  To point to the end of all input (if being typed from the command
//	line).
char*		textEndPtr	= NULL;


//  PURPOSE:  To hold the value that results from parse.
double		result		= 0.0;


//  PURPOSE:  To return 1 if the tokenizer should quit after EOF is reached.
//	Returns 0 otherwise.  No parameters.
int	yywrap	()
{
return(1);
}


//  PURPOSE:  To show the error message pointed to by 'cPtr'.  Returns '0'.
int	yyerror	(const char *cPtr)
{
printf("%s, sorry!\n",cPtr);
return(0);
}


//  PURPOSE:  To get up to 'maxSize' more characters of input and put them
//	into 'buffer'.   Returns how many characters were obtained.
int		ourInput	(char*	     buffer,
int	     maxSize
)
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


//  PURPOSE:  To parse and execute the program whose filename is given on the
//	command line at 'argv[1]' (if 'argc' >= 1), or that the user types in.
//	Returns 'EXIT_SUCCESS' on success or 'EXIT_FAILURE' otherwise.
int	      main		(int		argc,
char*		argv[]
)
{
//  I.  Application validity check:

//  II.  Parse and execute program:
//  II.A.  Get program source:
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

//  II.B.  Attempt to parse program:
try
{
yyparse();
}
catch  (std::string*	errStrPtr)
{
fprintf(stderr,"Error: %s.\n",errStrPtr->c_str());
delete(errStrPtr);
}

//  II.C.  Print output:
printf("%g\n",result);

//  II.D. Clean up:
//  II.D.1.  Close file (if open):
if  (filePtr != NULL)
fclose(filePtr);

fflush(stdout);

//  III.  Finished:
return(EXIT_SUCCESS);
}
