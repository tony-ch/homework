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
#define ALENMAX 15//id len max
#define STRMAX 100//max str len
#define TMAX 512// tab max
#define NMAX 10//num max len

extern char* symbolstr[SYMNUM];
extern FILE* fin;//Դ�ļ�
extern FILE* fout;//����ļ�
extern char ch;//���¶�����ַ�!��Ҫ��ʼ��
extern char line[LLEN];
extern int lcnt;//line num
extern int lleng;
extern int ccnt;

extern char* key[KEYNO];//������
extern char punc[SPSN];//�ָ�����



enum symbol{eofs=-1,constsy=0,intsy,charsy,voidsy,mainsy,ifsy,elsesy,whilesy,switchsy,casesy,defaultsy,scanfsy,printfsy,returnsy,
plus,minus,mul,divd,les,gtr,become,semicolon,comma,lparent,rparent,lbrack,rbrack,lbrace,rbrace,colon,
leq,geq,neq,eql,
ident,unsignum,zero,
charcon,strcon,
nul};
struct symStru{
    enum symbol id;
    char token[STRMAX];
    int lin;
    int col;
};
extern struct symStru symBuf[3];
extern int symBufIdx;

enum symbol ksym[KEYNO];//������
void getch();
void getsym();
void initSymBuf();
void updateSymBuf();
int reachEof();
void program();
void decConst();
void constDef();
void decVar();
void varDef();
int numDef();       //return num value
void retFuncDef();
void voidFuncDef();
int paraList();     //return para cnt
void mainDef();
void call();
void valueParaList();
void complexStat();
void stat();
void statList();
void expr();
void term();
void factor();
void assignment();
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



enum kinds{varkind,conkind,funkind,arrkind,parakind};
enum types{voidtyp,inttyp,chtyp};
struct tabrecord{
    char name[ALENMAX];
    enum kinds kind;
    enum types typ;
    int value;//value for const; para num for funkind; arr element num for arr
};
extern struct tabrecord tab[TMAX];
extern int tidx;//tab index

struct btabrecord{
    char name[ALENMAX];
    int tidx;//index in tab
};
extern struct btabrecord btab[TMAX];
extern int btidx;//block tab index
extern char kindstr[5][10];
extern char typestr[3][10];

#endif // GLOBAL_H_INCLUDED
