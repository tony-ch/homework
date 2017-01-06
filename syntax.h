//
// Created by tony on 2016/12/29.
//

#ifndef COMPILE_SYNTAX_H_H
#define COMPILE_SYNTAX_H_H

#define CASEMAX 30

extern int adrOffset;

struct CASTAB {
    int caseCnt;
    int labIdx[CASEMAX];
    int midx[CASEMAX];
    int cValue[CASEMAX];
};

void decConst();

void constDef();

//void decVar();
void varDef();

int numDef(char);       //return num value ; param:pos

void retFuncDef();

void voidFuncDef();

int paraList();     //return para cnt

void mainDef();

int call(int needRet);

int valueParaList(int funcid);

void complexStat();

void stat(char pos);

void statList();

int expr();

int term();

int factor();

void assignment();

void ifStat();

void whileStat();

int condition();

void switchStat();

void caseStat(struct CASTAB *tab);

void oneCase(struct CASTAB *tab);

void defaultCase(struct CASTAB *tab);

void readStat();

void writeStat();

void retStat();

#endif //COMPILE_SYNTAX_H_H
