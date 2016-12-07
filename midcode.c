//
// Created by tony on 2016/11/27.
//
#include "global.h"
#include "err.h"

char mopStr[][10] = {"conOp", "varOp", "funOp", "arrOp", "paraOp", "retOp", "endFunOp", "callOp", "calPaOp", "readOp",
                     "writeOp",
                     "jOp", "brfOp", "sltOp", "sleOp", "sgtOp", "sgeOp", "seqOp", "sneOp",
                     "liop", "addOp", "subOp", "mulOp", "divOp", "setArrOp", "getArrOp", "becomeOp", "genOp"};
char kindstr[5][10] = {"var", "const", "func", "arr", "para"};
char typestr[3][10] = {"void", "int", "char"};

char strtab[STRNUMMAX][STRMAX];
int strCnt = 0;

int enterStr(char *str) {
    strcpy(strtab[strCnt], str);
    return (strCnt++);
}

//struct TABREC tab[TMAX];
int tidx = 0;//tab top index
//struct BTABREC btab[TMAX];
int btidx = 0;//func tab top index

int adrOffset = 0;
int midx = 0;
struct MIDCODE mCode[CODEMAX];

int temVarCnt = 0;
int labCnt = 0;
int lab[LABMAX];

void enter(char *name, enum KINDS k, enum TYPES t, int value) {
    if (tidx == TMAX) {
        error(99);//!fatal err ,exit
    }
    if (k == funkind) {
        int i;
        for (i = 0; i < btidx; i++) {
            if (strcmp(btab[i].name, name) == 0) {
                error(5);//!redef func
                sprintf(name, "#func_%d", btidx);
                //return;//!should't return
            }
        }
    } else {
        int i;
        i = (btidx == 0) ? 0 : btab[btidx - 1].tidx +
                               1;// if global field, search from 0; if func field, search after func name.
        for (; i < tidx; i++) {
            if (strcmp(tab[i].name, name) == 0) {
                error(6);//! redef iden
                return;
            }
        }
    }
    strcpy(tab[tidx].name, name);
    tab[tidx].kind = k;
    tab[tidx].typ = t;
    tab[tidx].value = value;
    tab[tidx].adr = adrOffset;
    tab[tidx].inMem = 0;
    tab[tidx].regIdx = -1;
    fprintf(fout, "\t\tenter tab index: %d, name: %s, kind: %s, type: %s, value: %d, adr:%d\n",
            tidx, tab[tidx].name, kindstr[tab[tidx].kind], typestr[tab[tidx].typ], tab[tidx].value, tab[tidx].adr);
    if (k == funkind) {
        strcpy(btab[btidx].name, name);
        btab[btidx].tidx = tidx;
        btab[btidx].paraN = value;
        btab[btidx].reted = 0;
        btab[btidx].callParaN = 0;
        fprintf(fout, "\t\tenter btab index: %d, name: %s, tidx: %d, spacesz:%d, paraN:%d\n",
                btidx, btab[btidx].name, btab[btidx].tidx, btab[btidx].spacesz, btab[btidx].paraN);
        btidx = btidx + 1;
    }
    tidx = tidx + 1;
}

int lookup(char *name, int isfunc) {
    int result = -1;
    int i;
    if (isfunc == 1) {
        for (i = 0; i < btidx; i++)
            if (strcmp(name, btab[i].name) == 0)
                return btab[i].tidx;
    } else if (btidx == 0) {
        for (i = 0; i < tidx; i++)
            if (strcmp(tab[i].name, name) == 0)
                return i;
    } else {
        for (i = btab[btidx - 1].tidx + 1; i < tidx; i++)
            if (strcmp(tab[i].name, name) == 0)
                return i;
        for (i = 0; i < btab[0].tidx; i++)
            if (strcmp(tab[i].name, name) == 0)
                return i;
    }
    return result;
};


int getTemVar() {
    int ti = tidx;
    char name[ALENMAX] = "&";
//    itoa(temVarCnt,name+1,10);
    sprintf(name + 1, "%d", temVarCnt);//!itoa 改为sprintf
    temVarCnt++;
    enter(name, varkind, inttyp, 0);//将临时变量加入符号表
    adrOffset++;//分配空间
    return ti;
}

int getLab() {
    mCode[midx].op = genOp;
    mCode[midx].arg1Typ = earg;
    mCode[midx].arg2Typ = earg;
    mCode[midx].res.labIdx = labCnt, mCode[midx].rTyp = liarg;
    midx++;
    lab[labCnt] = midx;
    return (labCnt++);
}

void emitMid(enum MOP op, int a1, int a2, int r, enum ARGTYP a1t, enum ARGTYP a2t, enum ARGTYP rt) {
    mCode[midx].op = op;
    mCode[midx].arg1.value = a1;
    mCode[midx].arg1Typ = a1t;
    mCode[midx].arg2.value = a2;
    mCode[midx].arg2Typ = a2t;
    mCode[midx].res.tidx = r;
    mCode[midx].rTyp = rt;
    midx++;
}

void printCode() {
    int i;
    for (i = 0; i < tidx; i++) {
        fprintf(fout, "\t\ttab index: %d,\tname: %s,\tkind: %s,\ttype: %s,\tvalue: %d,\tadr: %d\n",
                i, tab[i].name, kindstr[tab[i].kind], typestr[tab[i].typ], tab[i].value, tab[i].adr);
    }
    for (i = 0; i < btidx; i++) {
        fprintf(fout, "\t\tbtab index: %d,\tname: %20s,\t\tspacesz: %d,\t\tparaN: %d,\t\treted: %d,\t\ttidx: %d\n",
                i, btab[i].name, btab[i].spacesz, btab[i].paraN, btab[i].reted, btab[i].tidx);
    }
    for (i = 0; i < strCnt; i++) {
        fprintf(fout, "\t\tstr index: %d,\tstr: %s\n", i, strtab[i]);
    }
    for (i = 0; i < labCnt; i++) {
        fprintf(fout, "\t\tlab index: %d,\tmidx: %d\n", i, lab[i]);
    }
    for (i = 0; i < midx; i++) {
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