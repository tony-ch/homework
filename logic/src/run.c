﻿//
// Created by tony on 11/7/18.
//
#include "common.h"

#define LOGSRC "RUN"
int stack[STACKSZ]={0};
int pstack = 0;
#define TRUTH_ELE 4
#define COMPLETE 16
wchar_t records[COMPLETE][EXPLENMAX]={0};

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
        default:
            return NON;
    }
}

void run_code(struct MIDCODE code, struct TABITEM *tab){
    enum ARGTYPE a1t = code.a1_type;
    enum ARGTYPE a2t = code.a2_type;
    //enum ARGTYPE rt = code.r_type;
    int a1 = a1t==TIDXARG ? tab[code.arg1.tidx].value: code.arg1.value;
    int a2 = a2t==TIDXARG ? tab[code.arg2.tidx].value: code.arg2.value;
    int truthidx = 0;
    switch (code.op){
        case PARAOP://r argtidx
            push(tab[code.res.value].value);
            break;
        case CALLOP: // a1 funcid a2 paranum
            for(int i=0;i<a2;i++){
                int tmp = pop();
                truthidx = truthidx | tmp<<i;
            }
            tab[code.res.tidx].value = functab[a1].truth_table[truthidx];
            LOG(DEBUG_LOG,LOGSRC,L"call fun %ls get %d",functab[a1].name,tab[code.res.tidx].value );
            break;
        case EQUOP:
        case IMPOP:
        case CONJOP:
        case DISJOP:
        case XOROP:
        case NOTOP:
            tab[code.res.tidx].value = run_math(code.op,a1,a2);
            LOG(DEBUG_LOG,LOGSRC,L"run %S %s:%d %s:%d get %d",getMopStr(code.op),tab[code.arg1.tidx].name,a1,tab[code.arg2.tidx].name,a2,tab[code.res.tidx].value);
            break;
        default:
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
                wprintf(L"%s: %d  ",exp->tab[exp->varidx[j]].name,getbit(i,j));
            }
            wprintf(L"\n");
            LOG(DEBUG_LOG,LOGSRC,L"%d true",i);
        }
    }
    LOG(INFO_LOG,LOGSRC,L"%d true, %d condition total",truth_cnt,total_condition);
	if (truth_cnt == total_condition) {
		LOG(INFO_LOG, LOGSRC, L"always true");
	}
	else if (truth_cnt == 0) {
		LOG(INFO_LOG, LOGSRC, L"always false");
	}
}

void run(){
	wprintf(L"\n######### exps #########\n");
    for(int i=0;i<expidx;i++){
        wprintf(L"%s\n",exptab[i].str);
        run_exp(&(exptab[i]));
    }
}

unsigned int int_to_int(unsigned int k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

int produce_narg_truth(int func, int n){
    int pstack_here = pstack - n;
    int recordIdx = 0;
    for(int i=0;i<TRUTH_ELE;i++){
        int tmp=0;
        for(int j=0;j<n;j++){//求每一位的真值
            tmp = getbit(stack[pstack_here+j],i)<<(n-j-1) | tmp;
        }
        recordIdx = recordIdx | functab[func].truth_table[tmp]<<i;
    }
    if(wcslen(records[recordIdx])==0){
        int stridx = 0;
		//swprintf(records[recordIdx] + stridx, EXPLENMAX, L"%s(", functab[func].name);
		snwprintf(records[recordIdx]+stridx,EXPLENMAX,L"%s(",functab[func].name);
        stridx+=wcslen(functab[func].name)+wcslen(L"(");
        for(int i=0;i<n;i++){
			//swprintf(records[recordIdx] + stridx, EXPLENMAX, L"%s,", records[stack[pstack_here + i]]);
			snwprintf(records[recordIdx]+stridx,EXPLENMAX,L"%s,",records[stack[pstack_here+i]]);
            stridx+=wcslen(records[stack[pstack_here+i]])+wcslen(L",");
        }
		//swprintf(records[recordIdx] + stridx - 1, EXPLENMAX, L")");
		snwprintf(records[recordIdx]+stridx-1,EXPLENMAX,L")");
        LOG(DEBUG_LOG,LOGSRC,L"%s %04u",records[recordIdx],int_to_int((unsigned int)recordIdx));
        return 1;
    } else{
        return 0;
    }
}

void check_complete(){
	if (funccnt == 0)
		return;
	wprintf(L"\n### check functions ####\n");
    //swprintf(records[0b0000],EXPLENMAX,L"0");
    //swprintf(records[0b1111],EXPLENMAX,L"1");
    //swprintf(records[0b0011],EXPLENMAX,L"p");
    //swprintf(records[0b0101],EXPLENMAX,L"q");
	snwprintf(records[0b0000], EXPLENMAX, L"0");
	snwprintf(records[0b1111], EXPLENMAX, L"1");
	snwprintf(records[0b0011], EXPLENMAX, L"p");
	snwprintf(records[0b0101], EXPLENMAX, L"q");
    int hasnew;
    do{
        hasnew=0;
        for(int func=0;func<funccnt;func++){
            int para_num = functab[func].para_num;
            for(int i=0;i<COMPLETE;i++){
                if(wcslen(records[i])==0)
                    continue;
                push(i);
                for(int j=0;j<COMPLETE;j++){
                    if(wcslen(records[j])==0)
                        continue;
                    push(j);
                    for(int k=0;k<COMPLETE;k++){
                        if(wcslen(records[k])==0)
                            continue;
                        push(k);
                        hasnew = hasnew | produce_narg_truth(func,para_num);
                        pop();
                    }
                    pop();
                }
                pop();
            }

        }
    }while (hasnew==1);
    int cnt = 0;
    for(unsigned i=0;i<COMPLETE;i++){
        if(wcslen(records[i])==0){
            continue;
        }
        wprintf(L"%s %04u\n",records[i],int_to_int(i));
        cnt++;
    }
    wprintf(L"get %d, %d total\n",cnt,COMPLETE);
	if (cnt == COMPLETE) {
		wprintf(L"complete set\n");
	}
	else {
		wprintf(L"not complete set\n");
	}
}
