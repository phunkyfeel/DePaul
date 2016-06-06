%{
    
    /*-------------------------------------------------------------------------*
     *---									---*
     *---		assign4.y						---*
     *---									---*
     *---	    This file defines the YACC-style grammar for the language	---*
     *---	at the bottom of page 173 of Fischer, Cytron, LeBlanc "Crafting	---*
     *---	 a Compiler" Addison-Wesley, 2010.				---*
     *---									---*
     *---	----	----	----	----	----	----	----	----	---*
     *---									---*
     *---	Version 1.0		2016 May 25		Joseph Phillips	---*
     *---									---*
     *-------------------------------------------------------------------------*/
    
    /*
     Number	Production
     1	start   ->  value
     2	value   ->  NUM
     3	value   ->  LPAREN expr RPAREN
     4	expr    ->  PLUS value value
     5	expr    ->  STAR values
     6	values  ->  value values
     7	values  ->
     */
    
    #include		"assign4.h"
    
    %}

%union {
    double          number_;
    NumberList*		listPtr_;
}


%start                  start
%nonassoc               LPAREN RPAREN
%nonassoc               STAR PLUS
%token      <number_>	NUM
%type       <number_>	start
%type       <number_>	expr
%type       <number_>	value
%type       <listPtr_>	values



%%

    start   :   value
                { result = $$ = $1; };

    value    :  NUM
                { $$ = $1 }
            |   LPAREN expr RPAREN
                { $$ = $2; };

    expr    :   PLUS value value
            { result = $$ = $2 + $3; };
            |   STAR values
            { $$ = $2->product(); delete($2)};

    values  :   value values
            { $$ = $2;
            $$->putIn($1)};
            |   //Lambda case
            { $$ = new NumberList; };

%%
