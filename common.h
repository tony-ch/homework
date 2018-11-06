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
#define NUMMAX 10
// lex
enum SYMBOL{
    EOFSYM,DEFSYM=L'#',LPARENNTSYM=L'(',RPARENTSYM=L')',COMMASYM=L',',
    EQUSYM=L'↔',IMPSYM=L'→',DISJSYM=L'∨',CONJSYM=L'∧',XORSYM=L'⊕',NOTSYM=L'¬',
    IDENTSYM,NUMSYM,LOGICNUMSYM,
    NULSYM
};

enum ERRORTYPE{
    INCOMPLETE_INPUT_ERR,INVAID_CHAR_ERR,INVAID_SENTENCE,ASSERT_ERROR,PARA_NUM_TOO_LARGE_ERR,RUNTIME_ERR
};

#define SYMBUFSZ 3
struct SYMITEM{
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
extern struct SYMITEM symBuf[SYMBUFSZ];
extern int symBufIdx;


// function
// common

// util
void LOG(int level, const char* source, const wchar_t* format, ...);
void endProc(int n);
FILE* getFile(const char* mode,const char* name, const char* default_path);
inline const char * getSymStr(enum SYMBOL sym);
inline const char * getErrStr(enum ERRORTYPE e);


// lex
void getch(FILE* fin);
void getsym(FILE* fin);
void initSymBuf(FILE *fin);
void updateSymBuf(FILE *fin);
int readEOF();

// syntax
void program();

// error
void error(enum ERRORTYPE e, const char *source,int,int);
void assert(int value);
#endif //LOGIC_COMMON_H
