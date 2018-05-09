%{
#include "global.h"
#include "bison.tab.h"
extern int isPipeFlag;
%}
%%
[\n]			{strcat(inputBuff,"\0");return 0;}
[\t ]+			{strcat(inputBuff,yytext); }
[\|]			{isPipeFlag=1;strcat(inputBuff,yytext);return *yytext;}
[><&]			{strcat(inputBuff,yytext);return *yytext;}
[^\|\n\t <>&]+	{strcat(inputBuff,yytext);yylval.str=yytext;return STRING;}
%%

int yywrap()
{
	return 1;
}
