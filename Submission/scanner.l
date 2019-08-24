%{
# include <stdlib.h>
# include "parser.tab.h"
# include "storetokens.hh"
%}

digit [0-9]
num {digit}+
letter [a-zA-Z_]
name {letter}({letter}|{digit})*
double [0-9]*[\.][0-9]+|[0-9]+[\.][0-9]*
metachar [,\;\(\)\{\}\?\:]
cmt \/\/(.)*
op [+*\-/]
ws [ \t]

%%

{ws}+		;

{cmt}		;

"if"		{
				store_token_name("IF", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return IF;	
			}

"else"		{
				store_token_name("ELSE", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return ELSE;	
			}		

"do"		{
				store_token_name("DO", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return DO;	
			}				

"while"		{
				store_token_name("WHILE", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return WHILE;	
			}

"int"		{
				store_token_name("INTEGER", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return INTEGER;
			}

"float"		{
				store_token_name("FLOAT", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return FLOAT;
			}

"void"		{
				store_token_name("VOID", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return VOID;
			}

"print"		{
				store_token_name("PRINT", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return PRINT;
			}

"return"	{
				store_token_name("RETURN", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return RETURN;
			}	

{name}		{
				store_token_name("NAME", yytext, yylineno);
				if(strcmp(yytext,"main"))
					yylval.string_value = new std::string(strcat(yytext,"_"));
				else	
					yylval.string_value = new std::string(yytext);
				return NAME;
			}

"&&"		{
				store_token_name("AND", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return AND;
			}

"||"		{
				store_token_name("OR", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return OR;
			}	

"<="		{
				store_token_name("LESS_THAN_EQUAL", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return RELOP;
			}

">="		{
				store_token_name("GREATER_THAN_EQUAL", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return RELOP;
			}

"<"			{
				store_token_name("LESS_THAN", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return RELOP;
			}

">"			{
				store_token_name("GREATER_THAN", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return RELOP;
			}

"=="		{
				store_token_name("EQUAL", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return EQOP;
			}

"!="		{
				store_token_name("NOT_EQUAL", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return EQOP;
			}

"!"			{
				store_token_name("NOT", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return NOT;
			}

"="			{
				store_token_name("ASSIGN_OP", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return ASSIGN;
			}

{num} 		{
				store_token_name("NUM", yytext, yylineno);
				yylval.integer_value = atoi(yytext);
				return INTEGER_NUMBER;
			}

{double}		{
				store_token_name("FNUM", yytext, yylineno);
				yylval.double_value = atof(yytext);
				return DOUBLE_NUMBER;
			}

{op} 		{
				store_token_name("ARITHOP", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return yytext[0];
			}

{metachar}	{
				store_token_name("META CHAR", yytext, yylineno);
				yylval.string_value = new std::string(yytext);
				return yytext[0];
			}

.			{
				fprintf(stderr, "cs316: Error\n");	
			}

\n 			{
				
			}

%%
