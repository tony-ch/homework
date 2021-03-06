﻿//
// Created by tony on 11/5/18.
//
#include "common.h"

#define LLENMAX 400 // line length limit
#define NLENMAX 10  // num length limit
#define IDENLENMAX 20 // ident length limix
#define CHCNTMAX 10000
#define LOGSRC "LEX"

int fileend = 0;
wchar_t all_ch[CHCNTMAX] = {0};

wchar_t ch=L' ';//current ch
wchar_t line[LLENMAX];//current line
struct SYMITEM symBuf[SYMBUFSZ];
int symBufIdx=0;
int lcnt = 0;//line num
int ccnt = 0;//col cnt
int chcnt = 0;//ch cnt
int lleng = 0;//len of currnt line

void reset_lex(FILE *fin){
    ch=L' ';//current ch
    //wchar_t line[LLENMAX];//current line
    //struct SYMITEM symBuf[SYMBUFSZ];
    symBufIdx=0;
    lcnt = 0;//line num
    ccnt = 0;//col cnt
    lleng = 0;//len of currnt line
	chcnt = 0;
    initSymBuf(fin);
}

void getch(FILE* fin){//读取下一个字符，放到ch中
	if (fileend > 0) {//secend pass
		if (chcnt >= fileend) {
			ch = WEOF;
			return;
		}
		lcnt = lcnt == 0 ? 1 : lcnt;
		ch = all_ch[chcnt];
		//chcnt += 1;
		if (ch == L'\n') {
			lcnt += 1;
			ccnt = 0;
		}
		ccnt += 1;
	}
	else { // first pass
		if (ccnt == lleng) {
			if ((fgetws(line, LLENMAX, fin)) == NULL) {
				ch = WEOF;
				fileend = chcnt;
				return;
			}
			lcnt += 1;
			ccnt = 0;
			lleng = 0;
			if (fin != stdin)
				wprintf(L"%s", line);
			LOG(DEBUG_LOG, LOGSRC, L"get line:%s", line);
			lleng = wcslen(line);
			//line[lleng-1] = L'\n';
		}
		ch = line[ccnt];
		all_ch[chcnt] = ch;
		ccnt += 1;
	}
	chcnt += 1;
    LOG(DEBUG_LOG,LOGSRC,L"get ch:%x,%lc,%d,%d",ch,ch,chcnt,fileend);
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
            if(is_alpha(ch)) {
                int i=0;
                do{
                    if(i<=IDENLENMAX-3){
                        symBuf[symBufIdx].token[i] = ch;
                        i += 1;
                    }
                    getch(fin);
                }while (is_alnum(ch));
                symBuf[symBufIdx].token[i]=0;
                symBuf[symBufIdx].id = IDENTSYM;
            }else if(is_digit(ch)){
                enum SYMBOL lastid = symBuf[(symBufIdx+SYMBUFSZ-1)%SYMBUFSZ].id;
                if(lastid==NUMSYM || lastid == LOGICNUMSYM){
                    symBuf[symBufIdx].id = LOGICNUMSYM;
                    symBuf[symBufIdx].token[0]=ch;
                    symBuf[symBufIdx].token[1]=0;
                    getch(fin);
                }else{
                    symBuf[symBufIdx].id = NUMSYM;
                    int i=0;
                    while (iswdigit(ch)){
                        if(i<NUMMAX-1){
                            symBuf[symBufIdx].token[i] = ch;
                            i += 1;
                        } else{
                            LOG(WARN_LOG,LOGSRC,L"num to long, line %d, col %d",lcnt,ccnt);
                        }
                        getch(fin);
                    }
                    symBuf[symBufIdx].token[i] = 0;
                }
            }else {
                error(INVAID_CHAR_ERR, LOGSRC, lcnt, ccnt);
                symBuf[symBufIdx].id = NULSYM;
                symBuf[symBufIdx].token[0] = ch;
                symBuf[symBufIdx].token[1] = 0;
                getch(fin);
            }
            break;
    }
}

void initSymBuf(FILE* fin){
    for(symBufIdx=0;symBufIdx<SYMBUFSZ;symBufIdx+=1){
        getsym(fin);
        LOG(DEBUG_LOG,LOGSRC,L"get %S, token:%s",getSymStr(symBuf[symBufIdx].id),symBuf[symBufIdx].token);
    }
    symBufIdx=0;
}

void updateSymBuf(FILE* fin){
    if(readEOF()){
        error(INCOMPLETE_INPUT_ERR,LOGSRC,lcnt,ccnt);
    }
    getsym(fin);
    LOG(DEBUG_LOG,LOGSRC,L"get %S, token:%s",getSymStr(symBuf[symBufIdx].id),symBuf[symBufIdx].token);
    symBufIdx = (symBufIdx+1)%SYMBUFSZ;
}

int readEOF(){
    return (symBuf[symBufIdx].id == EOFSYM);
}