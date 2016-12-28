//
// Created by tony on 2016/12/27.
//

#ifndef COMPILE_OPT_H
#define COMPILE_OPT_H

void optLab();

void delLab(int toDel, int rep);

void optJmp();

void optConst();

void delConst(int tid, int value);

void optExp();

int calExp(int codeIdx);

void dag();

#endif //COMPILE_OPT_H
