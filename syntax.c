//
// Created by tony on 11/6/18.
//
#include "common.h"

struct FUNCITEM functab[FUNCMAX];
struct EXPITEM exptab[EXPMAX];
#define SYMID (symBuf[symBufIdx].id)
#define NSYMID (symBuf[(symBufIdx+1)%SYMBUFSZ].id)

void program(){
    while(SYMID==IDENTSYM || ID==NOTSYM || ID==LPARENNTSYM || ID==DEFSYM){

    }
}

void sentence(){

}