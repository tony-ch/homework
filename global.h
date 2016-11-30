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
#define STRNUMMAX 100
#define TMAX 512// tab max
#define NMAX 10//num max len
#define CODEMAX 4096
#define LABMAX 512
#define CASEMAX 30
//lex
extern char* symbolstr[SYMNUM];
extern FILE* fin;//源文件
extern FILE* fout;//结果文件
extern char ch;//最新读入的字符!需要初始化
extern char line[LLEN];
extern int lcnt;//line num
extern int lleng;
extern int ccnt;
extern char* key[KEYNO];//保留字
extern char punc[SPSN];//分隔符号
enum SYMBOL{eofs=-1,constsy=0,intsy,charsy,voidsy,mainsy,ifsy,elsesy,whilesy,switchsy,casesy,defaultsy,scanfsy,printfsy,returnsy,
plus,minus,mul,divd,les,gtr,become,semicolon,comma,lparent,rparent,lbrack,rbrack,lbrace,rbrace,colon,
leq,geq,neq,eql,
ident,unsignum,zero,
charcon,strcon,
nul};
struct SYMSTRC{
    enum SYMBOL id;
    char token[STRMAX];
    int lin;
    int col;
};
extern struct SYMSTRC symBuf[3];
extern int symBufIdx;
enum SYMBOL ksym[KEYNO];//保留字
//tab
enum KINDS{varkind,conkind,funkind,arrkind,parakind};
enum TYPES{voidtyp,inttyp,chtyp};
struct TABREC{
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES typ;
    int value;//value for const; para num for funkind; arr element num for arr
    int adr;// for not para adr relative to b base$fp
};
extern int adrOffset;
extern struct TABREC tab[TMAX];
extern int tidx;//tab index
struct BTABREC{
    char name[ALENMAX];
    int tidx;//index in tab
    int hasCall;
    int spacesz;
    int paraN;
};
extern struct BTABREC btab[TMAX];
extern int btidx;//block tab index
extern char kindstr[5][10];
extern char typestr[3][10];
//midcode
enum MOP{
    conOp,varOp,funOp,arrOp,paraOp,retOp,endFunOp,callOp,calPaOp,readOp,writeOp,
    jOp,brfOp,sltOp,sleOp,sgtOp,sgeOp,seqOp,sneOp,
    liop,addOp,subOp,mulOp,divOp,setArrOp,getArrOp,becomeOp,genOp
};
extern char mopStr[][10];
enum ARGTYP{
    earg,targ,varg,tiarg,liarg,siarg,btiarg//empty type value idx str btidx
};
struct MIDCODE{
    enum MOP op;
    union {
        enum TYPES typ;
        int value;
        int tidx;
    } arg1;
    union {
        int value;
        int stridx;
        int tidx;
    } arg2;
    union{
        int tidx;
        int btid;
        int labIdx;
        //int value;
    } res;
    enum ARGTYP arg1Typ;
    enum ARGTYP arg2Typ;
    enum ARGTYP rTyp;
};
extern char strtab[STRNUMMAX][STRMAX];
extern int strCnt;
extern struct MIDCODE mCode[CODEMAX];
extern int midx;
extern int temVarCnt;
extern int labCnt;
extern int lab[LABMAX];
struct CASTAB{
    int caseCnt;
    int labIdx[CASEMAX];
    int midx[CASEMAX];
    int cValue[CASEMAX];
};
extern int labCnt;
//objcode
extern FILE* codefile;//结果文件



//lex
void getch();
void getsym();
void initSymBuf();
void updateSymBuf();
int reachEof();
//syn
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
int call(int hasRet);
int valueParaList();
void complexStat();
void stat();
void statList();
int expr();
int term();
int factor();
void assignment();
void ifStat();
void whileStat();
int condition();
void switchStat();
void caseStat(struct CASTAB* tab);
void oneCase(struct CASTAB* tab);
void defaultCase(struct CASTAB* tab);
void readStat();
void writeStat();
void retStat();
//midcode
void enter(char *name, enum KINDS k, enum TYPES t, int value);
int enterStr(char *str);
int lookup(char *name,int isfunc);
int getTemVar();
int getLab();
void printCode();
void genDeccode(enum MOP op,enum TYPES type,int value,int idx);//const var int func para
void genRetCode(enum MOP op, int idx);// endfunc ret
void genCallcode(enum MOP op, int value,int idx);//call callpara ret endfunc
void genRW(enum MOP op,char* str,int idx);//read write
void genMathcode(enum MOP op,int arg1,int arg2,int res);//read write
void genBrcode(enum MOP op, int arg1,int arg2, int labidx);//br j genlab
//objcode
void generate();
#endif // GLOBAL_H_INCLUDED
