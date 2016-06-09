%{
//  finalLang.y
//  $ bison --verbose -d --debug finalLang.y
//  $ gcc finalLang.tab.c -c

//+= operators

#include  "finalLang.h"
%}

%union
{
  Statement*	statePtr_;
  char*		charPtr_;
  double	const_;
  std::string*	stringPtr_;
}

%start			expr
%token			BEGIN_P END_P
%token			PRINT READ IF WHILE DO PRINTLN
%nonassoc		EQUALTO NOTEQUALTO
%right			EQUAL
%left			LESS GREATER INCREMENT DECREMENT
%left			PLUS MINUS PLUSEQUAL MINUSEQUAL MULTEQUAL DIVEQUAL
%left			STAR SLASH
%nonassoc       NOT
%token	<charPtr_>	VARIABLE
%token	<const_>	NUMBER
%token	<stringPtr_>	STRING
%type	<statePtr_>	expr
%type	<statePtr_>	list
%nonassoc		ERROR

%%

list	: list expr
	  {
	    ((BlockStatement*)$1)->add($2);
	    $$ = $1;
	  }
	|
	  {
	    // lambda production
	    $$ = new BlockStatement;
	  };

expr	: BEGIN_P PRINT expr END_P
	  { 
	    resultPtr = $$ = new PrintStatement($3);
	  }
    | BEGIN_P PRINT STRING END_P
      {
        resultPtr = $$ = new PrintStatement($3);
      }
    | BEGIN_P READ END_P
	  {
	    resultPtr = $$ = new ReadStatement();
	  }
	| BEGIN_P IF expr expr END_P
	  {
	    resultPtr = $$ = new IfStatement($3, $4);
	  }
	| BEGIN_P IF expr expr expr END_P
	  {
	    resultPtr = $$ = new IfStatement($3, $4, $5);
	  }
	| BEGIN_P WHILE expr expr END_P
	  {
	    resultPtr = $$ = new WhileStatement($3, $4);
	  }
	| BEGIN_P list END_P
	  {
	    resultPtr = $$ = $2;
	  }
	| BEGIN_P expr END_P
	  {
	    resultPtr = $$ = $2;
	  };
    | BEGIN_P EQUAL VARIABLE expr END_P
	  {
	    resultPtr = $$ = new AssignStatement($3, $4);
	  }
    | BEGIN_P LESS expr expr END_P
	  {
	    resultPtr = $$ = new BinaryOpStatement($3, '<', $4);
	  }
	| BEGIN_P GREATER expr expr END_P
	  {
	    resultPtr = $$ = new BinaryOpStatement($4, '<', $3);
	  }
	| BEGIN_P PLUS expr expr END_P
	  {
	    resultPtr = $$ = new BinaryOpStatement($3, '+', $4);
	  }
	| BEGIN_P MINUS expr expr END_P
	  {
	    resultPtr = $$ = new BinaryOpStatement($3, '-', $4);
	  }
	| BEGIN_P STAR expr expr END_P
	  {
	    resultPtr = $$ = new BinaryOpStatement($3, '*', $4);
	  }
	| BEGIN_P SLASH expr expr END_P
	  {
	    resultPtr = $$ = new BinaryOpStatement($3, '/', $4);
	  }
    | BEGIN_P EQUALTO expr expr END_P
      {
        resultPtr = $$ = new BinaryOpStatement($3, '=', $4);
      }
    | BEGIN_P NOTEQUALTO expr expr END_P
      {
        resultPtr = $$ = new BinaryOpStatement($3, '!', $4);
      }
    | BEGIN_P INCREMENT VARIABLE END_P
      {
        
      }
    | BEGIN_P DECREMENT VARIABLE END_P
      {
    
      }
    | BEGIN_P PLUSEQUAL VARIABLE NUMBER END_P
      {
    
      }
    | BEGIN_P MINUSEQUAL VARIABLE NUMBER END_P
      {
    
      }
    | BEGIN_P MULTEQUAL VARIABLE NUMBER END_P
      {
    
      }
    | BEGIN_P DIVEQUAL VARIABLE NUMBER END_P
      {
    
      }
    | BEGIN_P PRINTLN expr END_P
      {
        resultPtr = $$ = new PrintLnStatement($3);
      }
    | BEGIN_P PRINTLN STRING END_P
      {
        resultPtr = $$ = new PrintLnStatement($3);
      }
    | BEGIN_P NOT expr END_P
      {
        resultPtr = $$ = new UnaryOpStatement('!', $3);
      }
    | BEGIN_P MINUS expr END_P
      {
        resultPtr = $$ = new UnaryOpStatement('-', $3);
      }
    | BEGIN_P DO list expr END_P
      {
          ((BlockStatement*)$3)->add($4);
          resultPtr = $$ = $3;
      }
    | NUMBER
	  {
	    resultPtr = $$ = new ConstantStatement($1);
	  }
	| VARIABLE
	  {
	    resultPtr = $$ = new RValVarNameStatement($1);
	  };

%%
