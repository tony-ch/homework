//
// Created by tony on 11/6/18.
//
#include "common.h"
#include "syntax.h"

#define LOGSRC "SYN"
#define SYMID (symBuf[symBufIdx].id)
#define NSYMID (symBuf[(symBufIdx+1)%SYMBUFSZ].id)
#define TOKEN (symBuf[(symBufIdx)%SYMBUFSZ].token)
#define LIN (symBuf[symBufIdx].line)
#define COL (symBuf[symBufIdx].col)

struct FUNCITEM functab[FUNCMAX]={};
struct EXPITEM exptab[EXPMAX]={};
int funccnt = 0;
int funcidx = 0;
int PASSTWO = 0;

void init(int passtwo){
    PASSTWO = passtwo;
    funcidx=0;
    reset_lex(fin);
}

void functions(){
#define ONEPASS
    while(SYMID!=EOFSYM){
        if(SYMID!=DEFSYM){
            updateSymBuf(fin);
            continue;
        }
        updateSymBuf(fin);
        while(SYMID==IDENTSYM && NSYMID==NUMSYM){
            wcscpy(functab[funccnt].name,TOKEN);
            updateSymBuf(fin);
            int para_num = (int)wcstol(TOKEN,NULL,10);
            updateSymBuf(fin);
            functab[funccnt].para_num = para_num;
            int truth_num = 1 << para_num;
            functab[funccnt].truth_table = (uint8_t*)malloc(sizeof(truth_num));
            for (int i=0;i<truth_num;i++){
                functab[funccnt].truth_table[i] = (uint8_t)wcstol(TOKEN,NULL,10);
                updateSymBuf(fin);
            }
            funccnt +=1;
        }
    }
#if LOG_LEVEL==DEBUG_LOG
    for(int i=0;i<funccnt;i++){
        wprintf(L"FUNC: name:%ls,para_num:%d\n",functab[i].name,functab[i].para_num);
        for(int j=0;j<1<<functab[i].para_num;j++){
            wprintf(L"%d ",functab[i].truth_table[j]);
        }
        wprintf(L"\n");
    }
#endif
#undef ONEPASS
}

void program(){// 程序 = 语句 {语句}
    while(SYMID==IDENTSYM || SYMID==NOTSYM || SYMID==LPARENNTSYM || SYMID==DEFSYM){
        sentence();
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a program");
    if(!readEOF()){
        error(INVAID_SENTENCE,LOGSRC,LIN,COL);
    }
}

void sentence(){// 语句 = 逻辑语句 | 定义语句
    if(SYMID == DEFSYM){
        def_sentence();
    }else{
        logic_sentence();
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a sentence");
}

void def_sentence(){
    assert(SYMID==DEFSYM);
    updateSymBuf(fin);
    while(SYMID==IDENTSYM && NSYMID==NUMSYM){
        func_def();
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a def sentence");
}
void func_def(){
    assert(SYMID==IDENTSYM);
    updateSymBuf(fin);
    assert(SYMID==NUMSYM);
    int para_num = (int)wcstol(TOKEN,NULL,10);
    if(para_num>10){
        error(PARA_NUM_TOO_LARGE_ERR,LOGSRC,LIN,COL);
    }
    updateSymBuf(fin);
    int truth_num = 1 << para_num;
    for (int i=0;i<truth_num;i++){
        if(SYMID!=LOGICNUMSYM){
            error(FUNC_DEF_ERR,LOGSRC,LIN,COL);
        }
        updateSymBuf(fin);
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a func def");
}
void logic_sentence(){
    imp_term();
    if(SYMID==EQUSYM){
        updateSymBuf(fin);
        imp_term();
        LOG(DEBUG_LOG,LOGSRC,L"this is a EQU sentence");
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a logic sentence");

}

void imp_term(){
    xor_term();
    if(SYMID==IMPSYM){
        updateSymBuf(fin);
        xor_term();
        LOG(DEBUG_LOG,LOGSRC,L"this is a imp term");
    }
}
void xor_term(){
    disj_term();
    if(SYMID==XORSYM){
        updateSymBuf(fin);
        disj_term();
        LOG(DEBUG_LOG,LOGSRC,L"this is a xor term");
    }
}

void disj_term(){
    conj_term();
    while(SYMID==DISJSYM){
        updateSymBuf(fin);
        conj_term();
        LOG(DEBUG_LOG,LOGSRC,L"this is a disj term");
    }
}
void conj_term(){
    factor();
    while(SYMID==CONJSYM){
        updateSymBuf(fin);
        factor();
        LOG(DEBUG_LOG,LOGSRC,L"this is a conj term");
    }
}
void factor(){
    if(SYMID==LPARENNTSYM){
        updateSymBuf(fin);
        logic_sentence();
        assert(SYMID==RPARENTSYM);
        updateSymBuf(fin);
    }else if(SYMID==NOTSYM){
        updateSymBuf(fin);
        factor();
    }else if(SYMID==IDENTSYM && NSYMID==LPARENNTSYM){
        if(!PASSTWO || lookup_func(TOKEN)>=0){
            func_call();
        }else{
            updateSymBuf(fin);
        }
    }else if (SYMID == NUMSYM){
        assert(wcslen(TOKEN)==1);
        updateSymBuf(fin);
    }else if(SYMID==IDENTSYM){
        updateSymBuf(fin);
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a factor");
}
void func_call(){
    assert(SYMID==IDENTSYM && NSYMID==LPARENNTSYM);
    updateSymBuf(fin);
    updateSymBuf(fin);
    para_list();
    assert(SYMID==RPARENTSYM);
    updateSymBuf(fin);
    LOG(DEBUG_LOG,LOGSRC,L"this is a func call");
}
void para_list(){
    logic_sentence();
    while(SYMID!=RPARENTSYM){
        assert(SYMID==COMMASYM);
        updateSymBuf(fin);
        logic_sentence();
    }
    LOG(DEBUG_LOG,LOGSRC,L"this is a para list");
}