//
// Created by tony on 11/5/18.
//
#include "common.h"
#include "lex.h"

#define LLEN 200 // line length limit
#define NLEN 10  // num length limit
#define LOGSRC "LEX"

void getch();
void getsym();

wchar_t ch;
wchar_t line[LLEN];
int lcnt = 0;//line num
int ccnt = 0;//col cnt
int lleng = 0;//len of currnt line

wchar_t *symstr[] = {"EOFSYM","DEFSYM","LPARENNTSYM","RPARENTSYM","COMMASYM",
                     "EQUSYM","IMPSYM","DISJSYM","CONJSYM","XORSYM","NOTSYM",
                     "IDENTSYM","LOGNUMSYM","NOLOGNSYM",
                     "NULSYM"};

void getch(FILE* fin){//读取下一个字符，放到ch中
    if(ccnt==lleng){
        if(feof(fin)){
            ch = EOF;
            return;
        }
        lcnt += 1;
        ccnt = 0;
        lleng = 0;
        while((ch = getwc(fin)) != EOF && ch !=L'\n'){
            line[lleng]=ch;
            lleng += 1;
        }
        line[lleng] = L' ';
        lleng +=1;
        swprintf(MSG,MSGLEN,L"get line:%ls",line);
        LOG(LOG_DEBUG,LOGSRC,MSG);
    }
    ch = line[ccnt];
    ccnt += 1;
    swprintf(MSG,MSGLEN,L"get ch:%x,%lc",ch,ch);
    LOG(LOG_DEBUG,LOGSRC,MSG);
}