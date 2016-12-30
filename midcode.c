//
// Created by tony on 2016/11/27.
//
#include "global.h"
#include "midcode.h"

void enter(char *name, enum KINDS k, enum TYPES t, int value) {
    if (tabCnt == TMAX) {
        error(99);//!fatal err ,exit
    }
    if (k == funkind) {
        int i;
        for (i = 0; i < btabCnt; i++) {
            if (strcmp(btab[i].name, name) == 0) {
                error(5);//!redef func
                sprintf(name, "#func_%d", btabCnt);
                //return;//!should't return
            }
        }
    } else {
        int i;
        i = (btabCnt == 0) ? 0 : btab[btabCnt - 1].tidx +
                                 1;// if global field, search from 0; if func field, search after func name.
        for (; i < tabCnt; i++) {
            if (strcmp(tab[i].name, name) == 0) {
                error(6);//! redef iden
                return;
            }
        }
    }
    strcpy(tab[tabCnt].name, name);
    tab[tabCnt].kind = k;
    tab[tabCnt].typ = t;
    tab[tabCnt].value = value;
    tab[tabCnt].adr = adrOffset;
    tab[tabCnt].inMem = 1;//todo check
    tab[tabCnt].regIdx = -1;//todo check
    fprintf(fout, "\t\tenter tab index: %d, name: %s, kind: %s, type: %s, value: %d, adr:%d\n",
            tabCnt, tab[tabCnt].name, kindstr[tab[tabCnt].kind], typestr[tab[tabCnt].typ], tab[tabCnt].value,
            tab[tabCnt].adr);
    if (k == funkind) {
        strcpy(btab[btabCnt].name, name);
        btab[btabCnt].tidx = tabCnt;
        btab[btabCnt].paraN = value;
        btab[btabCnt].reted = 0;
        btab[btabCnt].callParaN = 0;
        fprintf(fout, "\t\tenter btab index: %d, name: %s, tidx: %d, spacesz:%d, paraN:%d\n",
                btabCnt, btab[btabCnt].name, btab[btabCnt].tidx, btab[btabCnt].spacesz, btab[btabCnt].paraN);
        btabCnt = btabCnt + 1;
    }
    tabCnt = tabCnt + 1;
}

int lookup(char *name, int isfunc) {//todo func and ident 分开
    int result = -1;//checked 此处未使用 检查调用者(all chceked)
    int i;
    if (isfunc == 1) {
        for (i = 0; i < btabCnt; i++)
            if (strcmp(name, btab[i].name) == 0)
                return btab[i].tidx;
    } else if (btabCnt == 0) {
        for (i = 0; i < tabCnt; i++)
            if (strcmp(tab[i].name, name) == 0)
                return i;
    } else {
        for (i = btab[btabCnt - 1].tidx + 1; i < tabCnt; i++)
            if (strcmp(tab[i].name, name) == 0)
                return i;
        for (i = 0; i < btab[0].tidx; i++)
            if (strcmp(tab[i].name, name) == 0)
                return i;
    }
    return result;
};


int getTemVar() {
    int ti = tabCnt;
    char name[ALENMAX] = "&";
//    itoa(temVarCnt,name+1,10);
    sprintf(name + 1, "%d", temVarCnt);//!itoa 改为sprintf
    temVarCnt++;
    enter(name, varkind, inttyp, 0);//将临时变量加入符号表
    adrOffset++;//分配空间
    return ti;
}

int getLab() {
    mCode[mcodeCnt].op = genOp;
    mCode[mcodeCnt].arg1Typ = earg;
    mCode[mcodeCnt].arg2Typ = earg;
    mCode[mcodeCnt].res.labIdx = labCnt, mCode[mcodeCnt].rTyp = liarg;
    mcodeCnt++;
    lab[labCnt] = mcodeCnt;
    return (labCnt++);
}

void emitMid(enum MOP op, int a1, int a2, int r, enum ARGTYP a1t, enum ARGTYP a2t, enum ARGTYP rt) {//todo check -1
    mCode[mcodeCnt].op = op;
    mCode[mcodeCnt].arg1.value = a1;
    mCode[mcodeCnt].arg1Typ = a1t;
    mCode[mcodeCnt].arg2.value = a2;
    mCode[mcodeCnt].arg2Typ = a2t;
    mCode[mcodeCnt].res.tidx = r;
    mCode[mcodeCnt].rTyp = rt;
    mcodeCnt++;
}

void printCode() {
    int i;
    for (i = 0; i < tabCnt; i++) {
        fprintf(fout, "\t\ttab index: %d,\tname: %s,\tkind: %s,\ttype: %s,\tvalue: %d,\tadr: %d\n",
                i, tab[i].name, kindstr[tab[i].kind], typestr[tab[i].typ], tab[i].value, tab[i].adr);
    }
    for (i = 0; i < btabCnt; i++) {
        fprintf(fout,
                "\t\tbtab index: %d,\tname: %20s,\t\tspacesz: %d,\t\tparaN: %d,\t\treted: %d,\t\ttidx: %d,\t\tcallParaN:%d\n",
                i, btab[i].name, btab[i].spacesz, btab[i].paraN, btab[i].reted, btab[i].tidx, btab[i].callParaN);
    }
    for (i = 0; i < strCnt; i++) {
        fprintf(fout, "\t\tstr index: %d,\tstr: %s\n", i, strtab[i]);
    }
    for (i = 0; i < labCnt; i++) {
        fprintf(fout, "\t\tlab index: %d,\tmidx: %d\n", i, lab[i]);
    }
    if (ecnt > 0) {
        return;//确保不会访问下标-1的元素
    }
    for (i = 0; i < mcodeCnt; i++) {
        fprintf(fout, "%5d%20s", i, mopStr[mCode[i].op]);
        switch (mCode[i].arg1Typ) {
            case targ:
                fprintf(fout, "%20s", typestr[mCode[i].arg1.typ]);
                break;
            case varg:
                fprintf(fout, "%20d", mCode[i].arg1.value);
                break;
            case tiarg:
                fprintf(fout, "%20s", tab[mCode[i].arg1.tidx].name);
                break;
            case btiarg:
                fprintf(fout, "%20s", btab[mCode[i].arg1.btid].name);
                break;
            default:
                fprintf(fout, "%20s", "");
                break;
        }
        switch (mCode[i].arg2Typ) {
            case varg:
                fprintf(fout, "%20d", mCode[i].arg2.value);
                break;
            case siarg:
                fprintf(fout, "%20s", strtab[mCode[i].arg2.stridx]);
                break;
            case tiarg:
                fprintf(fout, "%20s", tab[mCode[i].arg2.tidx].name);
                break;
            default:
                fprintf(fout, "%20s", "");
                break;
        }
        switch (mCode[i].rTyp) {
            case targ:
                fprintf(fout, "%20s", typestr[mCode[i].res.typ]);
                break;
            case tiarg:
                fprintf(fout, "%20s", tab[mCode[i].res.tidx].name);
                break;
            case liarg:
                fprintf(fout, "%15dlabel", mCode[i].res.labIdx);
                break;
            case btiarg:
                fprintf(fout, "%20s", btab[mCode[i].res.btid].name);
                break;
            default:
                fprintf(fout, "%20s", "");
                break;
        }
        fprintf(fout, "\n");
    }
}