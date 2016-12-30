//
// Created by tony on 2016/11/27.
//


#ifndef COMPILE_OBJCODE_H_H
#define COMPILE_OBJCODE_H_H

#define TREGNUM 8
#define PAMAX 300

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
