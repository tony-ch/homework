//
// Created by tony on 2016/12/27.
//
#include "global.h"
#include "opt.h"

//t1 opt jmp
//t2 opt lab
//t3 opt const
//t3.1 li
//t3.2 con
//t3.3 exp
void opt() {
    if (!OPT) {
        return;
    }
    optLab();
    optJmp();
    optConst();
    optExp();
}

void optLab() {
    for (int i = 0; i < mcodeCnt - 1; i++) {//0-midx-2 第二条到倒数第二条
        if (mCode[i].op == genOp) {
            int labId = mCode[i].res.labIdx;
            int j = i + 1;
            while (mCode[j].op == genOp) {
                mCode[j].op = optedOp;
                delLab(mCode[j].res.labIdx, labId);
                j++;
            }
        }
    }
}

void delLab(int toDel, int replaceLabIdx) {
    for (int i = 0; i < mcodeCnt; i++) {
        if (mCode[i].rTyp == liarg && mCode[i].res.labIdx == toDel) {
            mCode[i].res.labIdx = replaceLabIdx;
        }
    }
}

void optJmp() {
    for (int i = 1; i < mcodeCnt - 1; i++) {//0-midx-2 第一条到倒数第二条
        if (mCode[i].op != jOp && mCode[i].op != brfOp)
            continue;
        if (mCode[i + 1].op != genOp && mCode[i - 1].op != genOp)
            continue;
        int labIdx = mCode[i].res.labIdx;
        if (labIdx == mCode[i + 1].res.labIdx || labIdx == mCode[i - 1].res.labIdx) {
            mCode[i].op = optedOp;
        }
    }
}

void optConst() {
    for (int i = 0; i < mcodeCnt; i++) {//li,v,_,des
        if (mCode[i].op == liop) {
            mCode[i].op = optedOp;
            delConst(mCode[i].res.tidx, mCode[i].arg1.value);
        }
        if (mCode[i].op == conOp) {//const,_,v,tid
            mCode[i].op = optedOp;
            delConst(mCode[i].res.tidx, mCode[i].arg2.value);
        }
    }
}

void delConst(int tid, int value) {
    for (int i = 0; i < mcodeCnt; i++) {
        if (mCode[i].arg1Typ == tiarg && mCode[i].arg1.tidx == tid) {
            mCode[i].arg1Typ = varg;
            mCode[i].arg1.value = value;
        }
        if (mCode[i].arg2Typ == tiarg && mCode[i].arg2.tidx == tid) {
            mCode[i].arg2Typ = varg;
            mCode[i].arg2.value = value;
        }
        if (mCode[i].rTyp == tiarg && mCode[i].res.tidx == tid) {
            mCode[i].rTyp = varg;
            mCode[i].res.value = value;
            if (mCode[i].op == writeOp) {
                mCode[i].arg1Typ = targ;
                mCode[i].arg1.typ = tab[tid].typ;
            }
        }
    }
    //更改tab中的地址偏移
    for (int i = tid + 1; i < tabCnt && tab[i].kind != funkind; i++) {
        tab[i].adr -= 1;
    }
    if (tid >= btab[0].tidx) {//不是全局变量
        for (int i = 0; i < btabCnt; i++) {
            if (tid < btab[i].tidx) {
                btab[i - 1].spacesz -= 1;
                break;
            }
        }
    }
}

void optExp() {
    for (int i = 0; i < mcodeCnt; i++) {
        enum MOP op = mCode[i].op;
        if (mCode[i].arg1Typ != varg || mCode[i].arg2Typ != varg)
            continue;
        if (op == addOp || op == subOp || op == mulOp || op == divOp
            || op == sltOp || op == sleOp || op == seqOp || op == sneOp || op == sgtOp || op == sgeOp) {
            mCode[i].arg1.value = calExp(i);
            mCode[i].op = becomeOp;
            mCode[i].arg1Typ = varg;
            mCode[i].arg2Typ = earg;
        }
    }
}

int calExp(int codeIdx) {
    enum MOP op = mCode[codeIdx].op;
    int arg1 = mCode[codeIdx].arg1.value;
    int arg2 = mCode[codeIdx].arg2.value;
    if (arg2 == 0 && op == divOp) {
        printf("fetal error: div 0\n");
        endProc(-1);
    }
    switch (op) {
        case addOp:
            return arg1 + arg2;
        case subOp:
            return arg1 - arg2;
        case mulOp:
            return arg1 * arg2;
        case divOp:
            return arg1 / arg2;
        case sltOp:
            return arg1 < arg2;
        case sleOp:
            return arg1 <= arg2;
        case seqOp:
            return arg1 == arg2;
        case sneOp:
            return arg1 != arg2;
        case sgtOp:
            return arg1 > arg2;
        case sgeOp:
            return arg1 >= arg2;
        default:
            break;
    }

}

void dag() {

}