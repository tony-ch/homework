#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED
#define ERRMAX 30// max err num

extern char* emsg[ERRMAX];
extern int ecnt;//err cnt;

void error(int n);

#endif // ERR_H_INCLUDED
