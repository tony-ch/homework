//
// Created by tony on 11/5/18.
//

#ifndef LOGIC_COMMON_H
#define LOGIC_COMMON_H

//include
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <locale.h>
#include <stdarg.h>

// define
// common
#define STRMAX 100 // max len of str
// util
#define DEBUG_LOG 0
#define INFO_LOG 1
#define WARN_LOG 2
#define ERR_LOG 3
#define LOG_LEVEL DEBUG_LOG
// lex
enum SYMBOL{
    EOFSYM,DEFSYM,LPARENNTSYM,RPARENTSYM,COMMASYM,
    EQUSYM,IMPSYM,DISJSYM,CONJSYM,XORSYM,NOTSYM,
    IDENTSYM,LOGNUMSYM,NOLOGNSYM,
    NULSYM
};

enum ERRORTYPE{
    INCOMPLETE_INPUT_ERR,INVAID_CHAR_ERR,RUNTIME_ERR
};

#define SYMBUFSZ 3
struct SYMBUF{
    enum SYMBOL id;
    wchar_t token[STRMAX];
    int line;
    int col;
};
// var
// common
extern FILE *fin;//源文件
extern FILE *fout;//结果文件
extern FILE *codefile; //中间代码文件
// util

// lex
extern struct SYMBUF symBuf[SYMBUFSZ];
extern int symBufIdx;

// function
// common

// util
void LOG(int level, const char* source, const wchar_t* format, ...);
void endProc(int n);
FILE* getFile(const char* mode,const char* name, const char* default_path);

// lex
void getch(FILE* fin);
void getsym(FILE* fin);
void initSymBuf(FILE *fin);
void updateSymBuf(FILE *fin);
int readEOF();

// error
void error(enum ERRORTYPE e, const char *source,int,int);
#endif //LOGIC_COMMON_H
