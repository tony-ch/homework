%{
#include "global.h"
#include "bison.tab.h"
%}
%%
[\n]			{strcat(inputBuff,"\0");return 0;}
[\t ]+			{strcat(inputBuff,yytext); }
[\|><&]			{strcat(inputBuff,yytext);return *yytext;}
[^\|\n\t <>&]+	{strcat(inputBuff,yytext);yylval.str=yytext;return STRING;}
%%

int yywrap()
{
	return 1;
}
