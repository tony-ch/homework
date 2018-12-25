//
// Created by tony on 2016/12/27.
//

#ifndef COMPILE_OPT_H
#define COMPILE_OPT_H

void optLab();

void delLab(int toDel, int rep);

void optJmp();

void delConst(int tid, int value);

void optExp();

int calExp(int codeIdx);

void passConst(int codeIdx);

void divdBlk();

void dag();

void dataflow();

int canDelTemVar(int codeIdx);

void delDeadCode();

void globalReg();
#endif //COMPILE_OPT_H
