#include "err.h"
#include "global.h"

int err[ERRMAX][3];
int ecnt=0;//err cnt;
char* emsg[ERRMAX]={
    "incomplete source file",           //0
    "invalid character"                 //1
    "0 before unsigned or num too long" //2
    "should be \' "                     //3
    "should be ; "                      //4
    "syn error"                         //5
};


void error(int n){
    ecnt=ecnt+1;
    fprintf(fout,"line:%d col:%d errno:%d %s\n",symBuf[symBufIdx].lin,symBuf[symBufIdx].col,n,emsg[n]);
    switch(n){
    case 0://"incomplete source file",
        fprintf(fout,"errno:%d %s\n",n,emsg[n]);
        fclose(fin);
        fclose(fout);
        exit(1);
        break;
    case 1://"invalid character"
        fprintf(fout,"line:%d col:%d errno:%d %s: %c\n",lcnt,ccnt,n,emsg[n],ch);
        break;
    case 2://"0 before unsigned or num too long"
        fprintf(fout,"line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
        while(isdigit(ch)){//todo skip
            getch();
        }
        break;
    case 3://"should be \' "
    case 4://"should be ; "
        fprintf(fout,"line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
        break;
    }
}

