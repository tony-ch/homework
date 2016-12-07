#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED
#define ERRMAX 40// max err num

extern char *emsg[ERRMAX];
extern int ecnt;//err cnt;
extern char errPlace[30];

void error(int n);

void printErr(int n);

#endif // ERR_H_INCLUDED
