//
// Created by tony on 11/6/18.
//

#include "common.h"
#define CODEIDX exptab[expidx].codecnt
#define CODE exptab[expidx].codes[CODEIDX]
#define TAB exptab[expidx].tab
#define TABIDX  exptab[expidx].tabcnt
#define LOGSRC "MID"

int tmpv_cnt=0;

void enter(wchar_t* name, enum ENTRY_TYPES typ, int value){
    //TODO CHECK TMAX
    //TODO CHECK LOOKUP
    wcscpy(TAB[TABIDX].name,name);
    TAB[TABIDX].value = value;
    TAB[TABIDX].type = typ;
    TABIDX +=1;
}

int lookup_func(const wchar_t* name){
    for(int i=0;i<funccnt;i++){
        if(wcscmp(name,functab[i].name)==0){
            return i;
        }
    }
    return NOTFOUND;
}

int lookup_name(const wchar_t* name){
    for(int i=0;i<TABIDX;i++){
        if(wcscmp(name,TAB[i].name)==0){
            return i;
        }
    }
    return NOTFOUND;
}
int getTemVar(){
    int ti = TABIDX;
    wchar_t name[STRMAX];
    //swprintf(name,STRMAX,L"&%d",ti);
	snwprintf(name, STRMAX, L"&%d", ti);
    tmpv_cnt +=1;
    enter(name,TEMTYPE, NON);
    return ti;
}

void arg2str(enum ARGTYPE typ, int value, wchar_t * str){
    switch (typ){
        case NULARG:
            //swprintf(str,STRMAX,L"  ");
            snwprintf(str,STRMAX,L"  ");
            break;
        case VALUEARG:
            //swprintf(str,STRMAX,L"%d",value);
            snwprintf(str,STRMAX,L"%d",value);
            break;
        case TIDXARG:
            wcscpy(str,TAB[value].name);
            break;
        case FUNCARG:
            wcscpy(str,functab[value].name);
    }
}

void emitMid(enum MOP op,int a1,int a2,int r,enum ARGTYPE a1t,enum ARGTYPE a2t, enum ARGTYPE rt){
    CODE.a1_type = a1t;
    CODE.a2_type = a2t;
    CODE.r_type = rt;
    CODE.arg1.value = a1;
    CODE.arg2.value = a2;
    CODE.res.tidx = r;
    CODE.op = op;
    wchar_t a1s[STRMAX];
    wchar_t a2s[STRMAX];
    wchar_t rs[STRMAX];
    arg2str(a1t,a1,a1s);
    arg2str(a2t,a2,a2s);
    arg2str(rt,r,rs);
    LOG(DEBUG_LOG,LOGSRC,L"EMIT %S %S %S %S",getMopStr(op),a1s,a2s,rs);
    CODEIDX += 1;
}
