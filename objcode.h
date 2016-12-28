//
// Created by tony on 2016/11/27.
//


#ifndef COMPILE_OBJCODE_H_H
#define COMPILE_OBJCODE_H_H

#define TREGNUM 8
#define PAMAX 300
int mIdxCur = 0;
int btidCur = -1;
struct {
    int lastIn;//FIFO
    int tidx[TREGNUM];
    int dif[TREGNUM];
    int busy[TREGNUM];
    int regId[TREGNUM];
} tReg;
char calopStr[][4] = {"slt", "sle", "sgt", "sge", "seq", "sne", "add", "sub", "mul", "div"};

struct {
    int cnt;
    union {
        int tidx;
        int value;
    } para[PAMAX];
    int isTid[PAMAX];
} paraQue;

void mathToObj(int op);

void storeGlobal();

void conToObj();

void varToObj();

void arrToObj();

void funToObj();

void endFunToObj();

void retToObj();

void paraToObj();

void brfToObj();

void callToObj();

void liToObj();

void calPaToObj();

void getArrToObj();

void setArrToObj();

void jmpToObj();

void bcomToObj();

void genToObj();

void rdToObj();

void wrToObj();

#endif //COMPILE_OBJCODE_H_H
