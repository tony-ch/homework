//
// Created by tony on 11/7/18.
//
#include "common.h"

#define LOGSRC "RUN"
int stack[STACKSZ]={0};
int pstack = 0;

#define getbit(x,y) ((x)>>(y)&1)

int run_math(enum MOP op, int a1, int a2){
    switch (op){
        case EQUOP:
            return a1==a2;
        case IMPOP:
            return a1==0 || a2==1;
        case CONJOP:
            return a1==1 && a2==1;
        case DISJOP:
            return a1==1 || a2==1;
        case XOROP:
            return a1!=a2;
        case NOTOP:
            return a1==0?1:0;
    }
}

void run_code(struct MIDCODE code, struct TABITEM *tab){
    enum ARGTYPE a1t = code.a1_type;
    enum ARGTYPE a2t = code.a2_type;
    enum ARGTYPE rt = code.r_type;
    int a1 = a1t==TIDXARG ? tab[code.arg1.tidx].value: code.arg1.value;
    int a2 = a1t==TIDXARG ? tab[code.arg2.tidx].value: code.arg2.value;
    int truthidx = 0;
    switch (code.op){
        case PARAOP://r argtidx
            push(tab[code.res.value].value);
            break;
        case CALLOP: // a1 funcid a2 paranum
            for(int i=0;i<a2;i++){
                int tmp = pop();
                truthidx = truthidx | tmp<i;//TODO check
            }
            tab[code.res.tidx].value = functab[a1].truth_table[truthidx];
            break;
        case EQUOP:
        case IMPOP:
        case CONJOP:
        case DISJOP:
        case XOROP:
        case NOTOP:
            tab[code.res.tidx].value = run_math(code.op,a1,a2);
            break;
    }
}

void run_exp(struct EXPITEM *exp){
    int var_num = exp->var_num;
    int total_condition = 1 << var_num;
    int truth_cnt=0;
    for(int i=0;i<total_condition;i++){
        for (int j = 0;j <var_num;j++){// set value
            exp->tab[exp->varidx[j]].value = getbit(i,j);
        }
        for(int j =0;j<exp->codecnt;j++){
            run_code(exp->codes[j],exp->tab);
        }
        int res = exp->codes[exp->codecnt - 1].res.value;
        if(exp->codes[exp->codecnt-1].r_type==TIDXARG) {
            res = exp->tab[res].value;
        }
        if(res == 1){
            truth_cnt++;
            for (int j = 0;j <var_num;j++){// set value
                wprintf(L"%ls: %d  ",exp->tab[exp->varidx[j]].name,getbit(i,j));
            }
            wprintf(L"\n");
            LOG(DEBUG_LOG,LOGSRC,L"%d true",i);
        }
    }
    LOG(INFO_LOG,LOGSRC,L"%d true, %d condition total",truth_cnt,total_condition);
}

void run(){
    for(int i=0;i<expidx;i++){
        wprintf(L"%ls\n",exptab[i].str);
        run_exp(&(exptab[i]));
    }
}