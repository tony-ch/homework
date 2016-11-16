#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SYMNUM 40
#define PATHLEN 100//
#define KEYNO 14//number of key words
#define LLEN 200//line len limit
#define SPSN 16//punc num
#define ERRMAX 30// max err num
#define ALENMAX 15//id len max
#define STRMAX 100//max str len
#define TMAX 300// tab max
#define NMAX 10//num max len

void getsym();
void error(int n);

extern char* symbolstr[SYMNUM];
extern FILE* fin;//源文件
extern FILE* fout;//结果文件
extern char ch;//最新读入的字符!需要初始化
extern char line[LLEN];
extern int lcnt;//line num
extern int lleng;//line index
extern int ccnt;//ch cnt;
extern int ecnt;//err cnt;

extern char* key[KEYNO];//保留字
extern char punc[SPSN];//分隔符号
extern char* emsg[ERRMAX];
extern int err[ERRMAX][3];
extern char token[STRMAX];

extern int num;//lasr num read in

enum symbol{constsy=0,intsy,charsy,voidsy,mainsy,ifsy,elsesy,whilesy,switchsy,casesy,defaultsy,scanfsy,printfsy,returnsy,
plus,minus,mul,divd,les,gtr,become,semicolon,comma,lparent,rparent,lbrack,rbrack,lbrace,rbrace,colon,
leq,geq,neq,eql,
ident,unsignum,zero,
charcon,strcon,
nul};
enum symbol sym;
enum symbol ksym[KEYNO];//保留字
/*
enum kinds{vara,consta,func,arr};
enum types{inte,chara};
struct tabrecord{
    char name[ALENMAX];
    enum kinds kind;
    enum types typ;
    int ref;
};
struct tabrecord tab[TMAX];
extern int tidx;//tab index
*/

void program();
void decConst();
void constDef();
void decVar();
void varDef();
void numDef();
void retFuncDef();
void paraList();
void mainDef();
void valueParaList();
void complexStat();
void stat();
void statList();
void expr();
void term();
void factor();
void ifStat();
void whileStat();
void condition();
void switchStat();
void caseStat();
void oneCase();
void defaultCase();
void readStat();
void writeStat();
void retStat();
#endif // GLOBAL_H_INCLUDED
