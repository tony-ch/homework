//
// Created by tony on 2016/12/29.
//

#ifndef COMPILE_MIDCODE_H
#define COMPILE_MIDCODE_H

#define LABMAX 512
#define STRNUMMAX 100
#define CODEMAX 4096

char mopStr[][10] = {"conOp", "varOp", "funOp", "arrOp", "paraOp", "retOp", "endFunOp", "callOp", "calPaOp", "readOp",
                     "writeOp",
                     "jOp", "brfOp", "sltOp", "sleOp", "sgtOp", "sgeOp", "seqOp", "sneOp",
                     "liop", "addOp", "subOp", "mulOp", "divOp", "setArrOp", "getArrOp", "becomeOp", "genOp",
                     "optedOp"};
char kindstr[5][10] = {"var", "const", "func", "arr", "para"};
char typestr[3][10] = {"void", "int", "char"};

char strtab[STRNUMMAX][STRMAX];
int strCnt = 0;

int enterStr(char *str) {
    strcpy(strtab[strCnt], str);
    return (strCnt++);
}

//struct TABREC tab[TMAX];
int tabCnt = 0;//tab top index
//struct BTABREC btab[TMAX];
int btabCnt = 0;//func tab top index

int adrOffset = 0;
int mcodeCnt = 0;
struct MIDCODE mCode[CODEMAX];

int temVarCnt = 0;
int labCnt = 0;
int lab[LABMAX];

#endif //COMPILE_MIDCODE_H
