#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED
#define ERRMAX 40// max err num

extern char *emsg[ERRMAX];
extern char *wmsg[ERRMAX];
extern int ecnt;//err cnt;
extern char errPlace;

void testAfterStat(char pos);

void showPos();

void warn(int n);

void error(int n);

void printErr(int n);

#endif // ERR_H_INCLUDED
