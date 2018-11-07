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
#define TAB exptab[expidx].tab

struct FUNCITEM functab[FUNCMAX]={};
struct EXPITEM exptab[EXPMAX]={};
int funccnt = 0;
int funcidx = 0;
int PASSTWO = 0;
//int tabidx = 0;
int expidx = 0;
//int codeidx = 0;
#define CODEIDX exptab[expidx].codecnt
#define TABIDX  exptab[expidx].tabcnt
#define VARNUM exptab[expidx].var_num
#define EXPSTR exptab[expidx].str
#define EXPSTRIDX exptab[expidx].strlen

void init(int passtwo){
    PASSTWO = passtwo;
    funcidx = 0;
    expidx = 0;
    reset_lex(fin);
}

void functions(){
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
}

void program(){// 程序 = 语句 {语句}
    while(SYMID==IDENTSYM || SYMID==NOTSYM || SYMID==LPARENNTSYM || SYMID==DEFSYM){
        sentence();
    }
    //LOG(DEBUG_LOG,LOGSRC,L"this is a program");
    if(!readEOF()){
        error(INVAID_SENTENCE,LOGSRC,LIN,COL);
    }
}

void sentence(){// 语句 = 逻辑语句 | 定义语句
    if(SYMID == DEFSYM){
        def_sentence();
    }else{
        TABIDX = 0;
        CODEIDX = 0;
        VARNUM = 0;
        EXPSTRIDX = 0;
        logic_sentence();
        LOG(DEBUG_LOG,LOGSRC,L"this is a sentence with %d para.",exptab[expidx].var_num);
        expidx += 1;
    }
    //LOG(DEBUG_LOG,LOGSRC,L"this is a sentence");
}

void def_sentence(){
    assert(SYMID==DEFSYM);
    updateSymBuf(fin);
    while(SYMID==IDENTSYM && NSYMID==NUMSYM){
        func_def();
    }
    //LOG(DEBUG_LOG,LOGSRC,L"this is a def sentence");
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
    //LOG(DEBUG_LOG,LOGSRC,L"this is a func def");
}
int logic_sentence(){
    int res_tid = imp_term();
    if(SYMID==EQUSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        int arg1 = res_tid;
        int arg2 = imp_term();
        res_tid = getTemVar();
        emitMid(EQUOP,arg1,arg2,res_tid,TIDXARG,TIDXARG,TIDXARG);
        //LOG(DEBUG_LOG,LOGSRC,L"this is a EQU sentence");
    }
    //LOG(DEBUG_LOG,LOGSRC,L"this is a logic sentence");
    return res_tid;
}

int imp_term(){
    int res_tid = xor_term();
    if(SYMID==IMPSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        int arg1 = res_tid;
        int arg2 = xor_term();
        res_tid = getTemVar();
        emitMid(IMPOP,arg1,arg2,res_tid,TIDXARG,TIDXARG,TIDXARG);
        //LOG(DEBUG_LOG,LOGSRC,L"this is a imp term");
    }
    return res_tid;
}
int xor_term(){
    int res_tid = disj_term();
    if(SYMID==XORSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        int arg1 = res_tid;
        int arg2 = disj_term();
        res_tid = getTemVar();
        emitMid(XOROP,arg1,arg2,res_tid,TIDXARG,TIDXARG,TIDXARG);
        //LOG(DEBUG_LOG,LOGSRC,L"this is a xor term");
    }
    return res_tid;
}

int disj_term(){
    int res_tid = conj_term();
    while(SYMID==DISJSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        int arg1 = res_tid;
        int arg2 = conj_term();
        res_tid = getTemVar();
        emitMid(DISJOP,arg1,arg2,res_tid,TIDXARG,TIDXARG,TIDXARG);
        //LOG(DEBUG_LOG,LOGSRC,L"this is a disj term");
    }
    return res_tid;
}
int conj_term(){
    int res_tid = factor();
    while(SYMID==CONJSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        int arg1 = res_tid;
        int arg2 = factor();
        res_tid = getTemVar();
        emitMid(CONJOP,arg1,arg2,res_tid,TIDXARG,TIDXARG,TIDXARG);
        //LOG(DEBUG_LOG,LOGSRC,L"this is a conj term");
    }
    return res_tid;
}

int factor(){
    int res_tid = NOTFOUND;
    if(SYMID==LPARENNTSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        res_tid =  logic_sentence();
        assert(SYMID==RPARENTSYM);
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
    }else if(SYMID==NOTSYM){
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        int arg1_tid = factor();
        res_tid = getTemVar();
        emitMid(NOTOP,arg1_tid,NON,res_tid,TIDXARG,NULARG,TIDXARG);
    }else if(SYMID==IDENTSYM && NSYMID==LPARENNTSYM){
        if(!PASSTWO || lookup_func(TOKEN)!=NOTFOUND){
            res_tid = func_call();
        }else{
            res_tid = lookup_name(TOKEN);
            if(res_tid == NOTFOUND){
                res_tid = TABIDX;
                enter(TOKEN,VARTYPE,NON);
                exptab[expidx].varidx[VARNUM]=res_tid;
                VARNUM += 1;
            }
            wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
            EXPSTRIDX += wcslen(TOKEN);
            updateSymBuf(fin);
        }
    }else if (SYMID == NUMSYM){
        assert(wcslen(TOKEN)==1);
        res_tid = getTemVar();
        TAB[res_tid].type = CONSTTYPE;
        TAB[res_tid].value = (int)wcstol(TOKEN,NULL,10);
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
    }else if(SYMID==IDENTSYM){
        res_tid = lookup_name(TOKEN);
        if(res_tid == NOTFOUND){
            res_tid = TABIDX;
            enter(TOKEN,VARTYPE,NON);
            exptab[expidx].varidx[VARNUM]=res_tid;
            VARNUM += 1;
        }
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
    }//TODO INVALID FACTOR
    //LOG(DEBUG_LOG,LOGSRC,L"this is a factor");
    return res_tid;
}
int func_call(){
    assert(SYMID==IDENTSYM && NSYMID==LPARENNTSYM);
    int funcid = lookup_func(TOKEN);
    //TODO CHECK NOTFOUND
    wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
    EXPSTRIDX += wcslen(TOKEN);
    updateSymBuf(fin);
    wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
    EXPSTRIDX += wcslen(TOKEN);
    updateSymBuf(fin);
    int res_tid = getTemVar();
    int paracnt = para_list();
    emitMid(CALLOP,funcid,paracnt,res_tid,FUNCARG,VALUEARG,TIDXARG);
    assert(SYMID==RPARENTSYM);
    wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
    EXPSTRIDX += wcslen(TOKEN);
    updateSymBuf(fin);
    //LOG(DEBUG_LOG,LOGSRC,L"this is a func call");
    return res_tid;
}
int para_list(){// return para cnt
    int paracnt = 0;
    int resID = logic_sentence();
    emitMid(PARAOP,NON,NON,resID,NULARG,NULARG,TIDXARG);
    paracnt += 1;
    while(SYMID!=RPARENTSYM){
        assert(SYMID==COMMASYM);
        wcsncpy(EXPSTR+EXPSTRIDX,TOKEN,wcslen(TOKEN));
        EXPSTRIDX += wcslen(TOKEN);
        updateSymBuf(fin);
        resID = logic_sentence();
        emitMid(PARAOP,NON,NON,resID,NULARG,NULARG,TIDXARG);
        paracnt += 1;
    }
    //LOG(DEBUG_LOG,LOGSRC,L"this is a para list, paracnt: %d",paracnt);
    return paracnt;
}