//
// Created by tony on 11/5/18.
//
#include "common.h"

#define LLENMAX 400 // line length limit
#define NLENMAX 10  // num length limit
#define IDENLENMAX 20 // ident length limix
#define LOGSRC "LEX"

wchar_t ch=L' ';//current ch
wchar_t line[LLENMAX];//current line
struct SYMBUF symBuf[SYMBUFSZ];
int symBufIdx=0;
int lcnt = 0;//line num
int ccnt = 0;//col cnt
int lleng = 0;//len of currnt line

void getch(FILE* fin){//读取下一个字符，放到ch中
    if(ccnt==lleng){
        if(feof(fin)){
            ch = WEOF;
            return;
        }
        lcnt += 1;
        ccnt = 0;
        lleng = 0;
        while((ch = getwc(fin)) != WEOF && ch !=L'\n'){
            line[lleng]=ch;
            lleng += 1;
        }
        line[lleng] = 0;
        LOG(DEBUG_LOG,LOGSRC,L"get line:%ls",line);
        line[lleng] = '\n';
        lleng +=1;
    }
    ch = line[ccnt];
    ccnt += 1;
    LOG(DEBUG_LOG,LOGSRC,L"get ch:%x,%lc",ch,ch);
}

void getsym(FILE* fin){
    while(iswspace(ch)){
        getch(fin);
    }
    symBuf[symBufIdx].line = lcnt;
    symBuf[symBufIdx].col = ccnt;

    switch (ch) {
        case L'#':case L'(':case L')':case L',':
        case L'↔':case L'→':case L'∨':case L'∧':
        case L'⊕':case L'¬':
            symBuf[symBufIdx].id = ch;
            symBuf[symBufIdx].token[0] = ch;
            symBuf[symBufIdx].token[1] = 0;
            getch(fin);
            break;
        case WEOF:
            symBuf[symBufIdx].id = EOFSYM;
            symBuf[symBufIdx].token[0] = 0;
            break;
        default:
            if(iswalpha(ch) || ch == L'_' ) {
                int i=0;
                do{
                    if(i<=IDENLENMAX-3){
                        symBuf[symBufIdx].token[i] = ch;
                        i += 1;
                    }
                    getch(fin);
                }while (iswalnum(ch) || ch == L'_');
                symBuf[symBufIdx].id = IDENTSYM;
            }else if(iswdigit(ch)){
                symBuf[symBufIdx].token[0] = ch;
                symBuf[symBufIdx].token[1] = 0;
                if(ch==L'0' || ch == L'1'){
                    symBuf[symBufIdx].id = LOGNUMSYM;
                }else{
                    symBuf[symBufIdx].id = NOLOGNSYM;
                }
                getch(fin);
            }else {
                error(INVAID_CHAR_ERR, LOGSRC, lcnt, ccnt);
                symBuf[symBufIdx].id = NULSYM;
                symBuf[symBufIdx].token[0] = ch;
                symBuf[symBufIdx].token[1] = 0;
                getch(fin);
            }
            break;
    }

    LOG(DEBUG_LOG,LOGSRC,L"get %s, token:%ls",getSymStr(symBuf[symBufIdx].id),symBuf[symBufIdx].token);
}

void initSymBuf(FILE* fin){
    for(symBufIdx=0;symBufIdx<SYMBUFSZ;symBufIdx+=1){
        getsym(fin);
    }
    symBufIdx=0;
}

void updateSymBuf(FILE* fin){
    if(readEOF()){
        error(INCOMPLETE_INPUT_ERR,LOGSRC,lcnt,ccnt);
    }
    getsym(fin);
    symBufIdx = (symBufIdx+1)%SYMBUFSZ;
}

int readEOF(){
    return (symBuf[(symBufIdx+1) % SYMBUFSZ].id == EOFSYM);
}