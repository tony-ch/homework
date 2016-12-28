#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#define DEBUG

#define OPT 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALENMAX 15//id len max
#define STRMAX 100//max str len
#define TMAX 512// tab max

//err
extern char errPlace;
extern int ecnt;
//lex
extern FILE *fin;//Դ�ļ�
extern FILE *fout;//����ļ�
enum SYMBOL {
    eofsy, constsy, intsy, charsy, voidsy, mainsy,
    ifsy, elsesy, whilesy, switchsy, casesy, defaultsy,
    scanfsy, printfsy, returnsy,
    plussy, minussy, mulsy, divsy, lessy, gtrsy, leqsy, geqsy, neqsy, eqlsy,
    becomesy, semicolonsy, commasy, lparentsy, rparentsy, lbracksy, rbracksy, lbracesy, rbracesy, colonsy,
    identsy, unsignumsy, zerosy, charconsy, strconsy,
    nulsy
};
struct {
    enum SYMBOL id;
    char token[STRMAX];
    int lin;
    int col;
} symBuf[3];
extern int symBufIdx;
//tab
enum KINDS {
    varkind, conkind, funkind, arrkind, parakind
};
enum TYPES {
    voidtyp, inttyp, chtyp
};
struct {
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES typ;
    int value;//value for const; para num for funkind; arr element num for arr
    int adr;// for not para adr relative to b base$fp
    int inMem;
    int regIdx;
} tab[TMAX];

extern int tabCnt;//tab index
struct {
    char name[ALENMAX];
    int tidx;//index in tab
    int callParaN;
    int spacesz;
    int paraN;
    int reted;
} btab[TMAX];
extern int btabCnt;//block tab index
//midcode
enum MOP {
    conOp, varOp, funOp, arrOp, paraOp, retOp, endFunOp, callOp, calPaOp, readOp, writeOp,
    jOp, brfOp, sltOp, sleOp, sgtOp, sgeOp, seqOp, sneOp,
    liop, addOp, subOp, mulOp, divOp, setArrOp, getArrOp, becomeOp, genOp,
    optedOp
};
enum ARGTYP {
    earg, targ, varg, tiarg, liarg, siarg, btiarg//empty type value idx str btidx
};
struct MIDCODE {
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
    union {
        int tidx;
        int btid;
        int labIdx;
        int value;
    } res;
    enum ARGTYP arg1Typ;
    enum ARGTYP arg2Typ;
    enum ARGTYP rTyp;
};
extern char strtab[][STRMAX];
extern int strCnt;
extern struct MIDCODE mCode[];
extern int mcodeCnt;

//objcode
extern FILE *codefile;//����ļ�

void endProc(int n);//end program

//lex
void initSymBuf();

void updateSymBuf();

int reachEof();

void printSym();

//syn
void program();

//midcode
void enter(char *name, enum KINDS k, enum TYPES t, int value);

int enterStr(char *str);

int lookup(char *name, int isfunc);

int getTemVar();

int getLab();

void emitMid(enum MOP op, int a1, int a2, int r, enum ARGTYP a1t, enum ARGTYP a2t, enum ARGTYP rt);

void printCode();

//opt
void opt();

//objcode
void generate();

//err
void warn(int n);

void error(int n);

void testAfterStat(char pos);

#endif // GLOBAL_H_INCLUDED
