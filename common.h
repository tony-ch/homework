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
#include <locale.h>

// define

// util
#define DEBUG
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERR 3
#define LOG_LEVEL LOG_DEBUG
#define MSGLEN 100
// lex
enum SYMBOL{
    EOFSYM,DEFSYM,LPARENNTSYM,RPARENTSYM,COMMASYM,
    EQUSYM,IMPSYM,DISJSYM,CONJSYM,XORSYM,NOTSYM,
    IDENTSYM,LOGNUMSYM,NOLOGNSYM,
    NULSYM
};

// var
// util
extern wchar_t MSG[];
// lex
extern FILE *fin;//源文件
extern FILE *fout;//结果文件
extern FILE *codefile; //中间代码文件

// function

// util
void LOG(int level, const char* source, const wchar_t* msg);
void endProc(int n);
FILE* getFile(const char* mode,const char* name, const char* default_path);

// lex
void getch(FILE* fin);

#endif //LOGIC_COMMON_H
