#include "global.h"
#include "err.h"

struct TABREC tab[TMAX];
int tidx=0;//tab top index
struct BTABREC btab[TMAX];
int btidx=0;//func tab top index

char kindstr[5][10]={"var","const","func","arr","para"};//todo debug
char typestr[3][10]={"void","int","char"};//todo debug

int adrOffset=0;
int midx=0;
struct MIDCODE mCode[CODEMAX];

int temVarCnt=0;
int labCnt=0;
int lab[LABMAX];
/*case record
struct caseRecord caseTab[CASEMAX];
int caseCnt=0;
 */


void program(){
    if(symBuf[symBufIdx].id==constsy){//!头符号为const
        decConst();
    }
    int hasVarDef=0;
    while((symBuf[symBufIdx].id==intsy || symBuf[symBufIdx].id==charsy)&&symBuf[(symBufIdx+2)%3].id!=lparent){
        hasVarDef=1;
        varDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是分号(F)
        }else{
            updateSymBuf();//!read one more sym
        }
    }
    if(hasVarDef){
        fprintf(fout,"\t\tthis is dec of var.\n");
    }
    while((symBuf[symBufIdx].id==intsy || symBuf[symBufIdx].id==charsy || symBuf[symBufIdx].id==voidsy)&&symBuf[(symBufIdx+1)%3].id!=mainsy){
        if(symBuf[symBufIdx].id==voidsy){
            voidFuncDef();
        }else{
            retFuncDef();
        }
    }
    mainDef();
    fprintf(fout,"\t\tthis is a program.\n");

    int i;
    fprintf(fout,"\n");
    for(i=0;i<tidx;i++){
        fprintf(fout,"\t\ttab index: %d,\tname: %s,\tkind: %s,\ttype: %s,\tvalue: %d,\tadr: %d\n",
        i,tab[i].name,kindstr[tab[i].kind],typestr[tab[i].typ],tab[i].value,tab[i].adr);
    }
    for(i=0;i<btidx;i++){
        fprintf(fout,"\t\tbtab index: %d,\tname: %s,\ttidx: %d\n",
                i,btab[i].name,btab[i].tidx);
    }
    printCode();

}
//const 开头
void decConst(){
    if(symBuf[symBufIdx].id!=constsy){//!可稍改写文法
        error(5);//应是const
        return;
    }
    updateSymBuf();
    constDef();
    if(symBuf[symBufIdx].id!=semicolon){
        error(4);//!应是;(F)
    }else{
        updateSymBuf();
    }
    while(symBuf[symBufIdx].id==constsy){//! first集合为{const}
        updateSymBuf();
        constDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是;(F)
        }else{
            updateSymBuf();
        }
    }
    fprintf(fout,"\t\tthis is dec of const.\n");
}

void constDef(){
    enum KINDS kind=conkind;
    enum TYPES type;
    char name[ALENMAX];
    int value;
    if(symBuf[symBufIdx].id==intsy){
        type=inttyp;
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo 重复代码
            error(5);//应是标识符
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(5);//应是=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){//!first集合
            error(5);//应是数字
            return;
        }
        value=numDef();
        mCode[midx].op=conOp;
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,value);
        adrOffset++;
        while(symBuf[symBufIdx].id==comma){//可选项
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//应是标识符
                return;
            }
            strcpy(name,symBuf[symBufIdx].token);
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(5);//应是=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){
                error(5);//应是数字
                return;
            }
            value=numDef();
            mCode[midx].op=conOp;
            mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
            mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
            mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
            midx++;
            enter(name,kind,type,value);
            adrOffset++;
        }
    }else if(symBuf[symBufIdx].id==charsy){
        type=chtyp;
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo 重复代码
            error(5);//应是标识符
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(5);//应是=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=charcon){
            error(5);//应是字符
            return;
        }
        value=symBuf[symBufIdx].token[1];
        mCode[midx].op=conOp;
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,value);
        adrOffset++;
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//可选项
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//应是标识符
                return;
            }
            strcpy(name,symBuf[symBufIdx].token);
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(5);//应是=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=charcon){
                error(5);//应是字符
                return;
            }
            value=symBuf[symBufIdx].token[1];
            mCode[midx].op=conOp;
            mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
            mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
            mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
            midx++;
            enter(name,kind,type,value);
            adrOffset++;
            updateSymBuf();
        }
    }else{
        error(5);//todp 应为int或char
        return;
    }
    fprintf(fout,"\t\tthis is const def.\n");
}
/*
void decVar(){//＜变量说明＞::=＜变量定义＞;{＜变量定义＞;}
}
*/
void varDef(){//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ }
    //enum symbol typ=sym;
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    if(symBuf[symBufIdx].id==intsy){
        type=inttyp;
    } else if(symBuf[symBufIdx].id==charsy){
        type=chtyp;
    }else{
        error(5);//todo 应为int或char
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo 应为标识符
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    kind=varkind;
    value=1;
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){//!可选项
        updateSymBuf();
        if(symBuf[symBufIdx].id!=unsignum){
            error(5);//todo 应为无符号整数
            return;
        }
        kind=arrkind;
        value=atoi(symBuf[symBufIdx].token);
        updateSymBuf();
        if(symBuf[symBufIdx].id!=rbrack){
            error(5);//todo )
            return;
        }
        updateSymBuf();
    }
    mCode[midx].op=(kind==arrkind?arrOp:varOp);
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=value;mCode[midx].arg2Typ=(kind==arrkind?varg:earg);
    mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
    midx++;
    enter(name,kind,type,value);
    adrOffset=adrOffset+value;
    while(symBuf[symBufIdx].id==comma){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!不应有类型标识符
            error(5);//todo 应为标识符
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        kind=varkind;
        value=1;
        updateSymBuf();
        if(symBuf[symBufIdx].id==lbrack){//!可选项
            updateSymBuf();
            if(symBuf[symBufIdx].id!=unsignum){
                error(5);//todo 应为无符号整数
                return;
            }
            kind=arrkind;
            value=atoi(symBuf[symBufIdx].token);
            updateSymBuf();
            if(symBuf[symBufIdx].id!=rbrack){
                error(5);//todo )
                return;
            }
            updateSymBuf();
        }
        mCode[midx].op=(kind==arrkind?arrOp:varOp);
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2.value=value;mCode[midx].arg2Typ=(kind==arrkind?varg:earg);
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,value);
        adrOffset=adrOffset+value;
    }
    fprintf(fout,"\t\tthis is var def.\n");
}

int numDef(){//＜整数＞::= ［＋｜－］＜无符号整数＞｜０
    int flag=0;
    int num=0;
    if(symBuf[symBufIdx].id==zero){
        updateSymBuf();
        num=0;
    }else{
        if(symBuf[symBufIdx].id==plus){//!可选项
            updateSymBuf();
        }else if(symBuf[symBufIdx].id==minus){
            updateSymBuf();
            flag=1;
        }
        if(symBuf[symBufIdx].id==unsignum){
            num=atoi(symBuf[symBufIdx].token);
            if(flag==1){
                num=-num;
            }
            updateSymBuf();
        }else{
            error(5);//todo 应是unsignnum
            return 0;
        }
    }
    fprintf(fout,"\t\tthis is a num:%d.\n",num);
    return num;
}

void retFuncDef(){//＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur,btidCur;
    int mIdxCur;
    kind=funkind;
    tIdxCur=tidx;btidCur=btidx;
    if(symBuf[symBufIdx].id==charsy){
        type=chtyp;
    }else if(symBuf[symBufIdx].id==intsy){
        type=inttyp;
    } else{
        error(5);//todo 应是类型标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo 应是标识符
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    mIdxCur=midx;
    mCode[midx].op=funOp;
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=0;mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=btidx;mCode[midx].rTyp=btiarg;
    midx++;
    enter(name,kind,type,0);
    adrOffset=0;
    //adrOffset++;//for $fp; todo
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tIdxCur].value=value;
    mCode[mIdxCur].arg2.value=value;
    adrOffset=adrOffset+10;//todo for $ra and $s0-$s7
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tIdxCur].name,tab[tIdxCur].value);
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo 应是{
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo 应是}
        return;
    }
    mCode[midx].op=endFunOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.btid=btidCur;mCode[midx].rTyp=btiarg;
    midx++;
    btab[btidCur].spacesz=adrOffset;
    btab[btidCur].paraN=value;
    updateSymBuf();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur,btIdxCur;
    int mIdxCur;
    kind=funkind;
    tIdxCur=tidx;btIdxCur=btidx;
    if(symBuf[symBufIdx].id!=voidsy){
        error(5);//todo 应是类型标识符
        return;
    }
    type=voidtyp;
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo 应是标识符
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    mIdxCur=midx;
    mCode[midx].op=funOp;
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=0;mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=btidx;mCode[midx].rTyp=btiarg;
    midx++;
    enter(name,kind,type,0);
    adrOffset=0;
    //adrOffset++;//todo for $fp
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tIdxCur].value=value;
    mCode[mIdxCur].arg2.value=value;
    adrOffset=adrOffset+10;//todo for $fp($sp+16) $ra($sp+20) and $s0-$s7
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tIdxCur].name,tab[tIdxCur].value);
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo 应是{
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace) {
        error(5);//todo 应是}
        return;
    }
    mCode[midx].op=endFunOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.btid=btIdxCur;mCode[midx].rTyp=btiarg;
    midx++;
    btab[btIdxCur].spacesz=adrOffset;
    btab[btIdxCur].paraN=value;
    updateSymBuf();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

int paraList(){//＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind=parakind;
    int paraCnt=0;
    if(symBuf[symBufIdx].id==charsy||symBuf[symBufIdx].id==intsy){//!可以为空
        if(symBuf[symBufIdx].id==charsy){
            type=chtyp;
        } else{
            type=inttyp;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){
            error(5);//todo
            return 0;
        }
        strcpy(name,symBuf[symBufIdx].token);
        mCode[midx].op=paraOp;
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,0);
        paraCnt=paraCnt+1;
        adrOffset++;
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){
            updateSymBuf();
            if(symBuf[symBufIdx].id!=charsy && symBuf[symBufIdx].id!=intsy){
                error(5);//todo
                return 0;
            }
            if(symBuf[symBufIdx].id==charsy){
                type=chtyp;
            } else{
                type=inttyp;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//todo
                return 0;
            }
            strcpy(name,symBuf[symBufIdx].token);
            mCode[midx].op=paraOp;
            mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
            mCode[midx].arg2Typ=earg;
            mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
            midx++;
            enter(name,kind,type,0);
            adrOffset++;
            paraCnt=paraCnt+1;
            updateSymBuf();
        }
    }
    adrOffset+=(4-paraCnt);
    fprintf(fout,"\t\tthis is para list.\n");
    return paraCnt;
}

void mainDef(){//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind=funkind;
    int tidCur=tidx;
    int btidCur=btidx;
    if(symBuf[symBufIdx].id!=voidsy){
        error(5);//todo 应是类型标识符
        return;
    }
    type=voidtyp;
    updateSymBuf();
    if(symBuf[symBufIdx].id!=mainsy){
        error(5);//todo 应是标识符
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    mCode[midx].op=funOp;
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=0; mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=btidx;mCode[midx].rTyp=btiarg;
    midx++;
    enter(name,kind,type,0);
    adrOffset=0;
    adrOffset++;//for fp;
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    complexStat();
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo 应是(
        return;
    }//!!todo
    mCode[midx].op=endFunOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.tidx=btidCur;mCode[midx].rTyp=tiarg;
    midx++;
    btab[btidCur].spacesz=adrOffset;
    fprintf(fout,"\t\tthis is main func dec.\n");
}

int call(int hasRet){
    int funcId,resTid=0,paraCnt,funcBtid=0;
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo
        return -1;
    }
    funcId=lookup(symBuf[symBufIdx].token,1);
    while (funcBtid<btidx && strcmp(btab[funcBtid].name,tab[funcId].name)!=0){
        funcBtid++;
    }
    if(funcId==-1){
        error(99);// todo 函数未定义
        return -1;
    }
    if(tab[funcId].typ==voidtyp && hasRet==1){
        error(99);//todo 应是又返回值函数
        return -1;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo
        return -1;
    }
    if(hasRet){
        resTid=getTemVar();
        tab[resTid].typ=tab[funcId].typ;
    }
    updateSymBuf();//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
    paraCnt=valueParaList();//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
    mCode[midx].op=callOp;
    mCode[midx].arg1.tidx=resTid;mCode[midx].arg1Typ=hasRet==1?tiarg:earg;//const type value name
    mCode[midx].arg2.value=paraCnt;mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=funcBtid;mCode[midx].rTyp=btiarg;
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return -1;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a call stat.\n");
    return resTid;
}


int valueParaList(){//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
    int paraCnt=0;
    int resTid;
    if(symBuf[symBufIdx].id==rparent){//!空
        //getsym();//!
    }else{
        resTid=expr();//!至少一个
        mCode[midx].op=calPaOp;
        mCode[midx].arg1Typ=earg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=resTid;mCode[midx].rTyp=tiarg;
        midx++;
        paraCnt++;
        while(symBuf[symBufIdx].id==comma){
            updateSymBuf();
            resTid=expr();
            mCode[midx].op=calPaOp;
            mCode[midx].arg1Typ=earg;//const type value name
            mCode[midx].arg2Typ=earg;
            mCode[midx].res.tidx=resTid;mCode[midx].rTyp=tiarg;
            midx++;
            paraCnt++;
        }
    }
    fprintf(fout,"\t\tthis is value para list.\n");
    return paraCnt;
}

void complexStat(){//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
    if(symBuf[symBufIdx].id==constsy){//!头符号为const
        decConst();
    }
    int hasVarDef=0;
    while(symBuf[symBufIdx].id==charsy || symBuf[symBufIdx].id==intsy){
        varDef();
        hasVarDef=1;
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是;(F)
        }else{
            updateSymBuf();
        }
    }
    if(hasVarDef){
        fprintf(fout,"\t\tthis is dec of var.\n");
    }
    statList();
    fprintf(fout,"\t\tthis is complex stat.\n");
}

void stat(){//＜语句＞::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
    if(symBuf[symBufIdx].id==ifsy){//!头符号集合
        ifStat();
    }else if(symBuf[symBufIdx].id==whilesy){
        whileStat();
    }else if(symBuf[symBufIdx].id==lbrace){//语句列
        updateSymBuf();
        statList();
        if(symBuf[symBufIdx].id!=rbrace){
            error(5);//应是}
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==scanfsy){
        readStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是;(F)
        }else{
            updateSymBuf();
        }
    }else if(symBuf[symBufIdx].id==printfsy){
        writeStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是;(F)
        }else{
            updateSymBuf();
        }
    }else if(symBuf[symBufIdx].id==returnsy){
        retStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是;(F)
        }else{
            updateSymBuf();
        }
    }else if(symBuf[symBufIdx].id==switchsy){
        switchStat();
    }else if(symBuf[symBufIdx].id==ident){//!first集合相交
        enum SYMBOL nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==become || nextSym==lbrack){
            assignment();
        }else if(nextSym==lparent){
            call(0);//todo 可能是又返回值的
        }else{
            error(5);//非法语句
            return;
        }
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!应是;(F)
        }else{
            updateSymBuf();
        }
    }else{
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!!应是;(F)
        }else{
            updateSymBuf();
        }
    }//!空
    fprintf(fout,"\t\tthis is a stat.\n");
}

void statList(){
    //!可以没有一条
    while(symBuf[symBufIdx].id==ifsy     ||  symBuf[symBufIdx].id==whilesy    ||  symBuf[symBufIdx].id==lbrace ||
          symBuf[symBufIdx].id==scanfsy  ||  symBuf[symBufIdx].id==printfsy   ||  symBuf[symBufIdx].id==returnsy ||
          symBuf[symBufIdx].id==switchsy ||  symBuf[symBufIdx].id==ident      ||  symBuf[symBufIdx].id==semicolon){//!first集合
        stat();
    }
    fprintf(fout,"\t\tthis is a stat list.\n");
}

int expr(){//＜表达式＞::=［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    int ti1,ti2,resTi,negflag;
    negflag=0;
    if(symBuf[symBufIdx].id==plus){//!可选项
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==minus){
        updateSymBuf();
        negflag=1;
    }
    ti1=term();
    if(negflag){
        resTi=getTemVar();
        mCode[midx].op=subOp;
        mCode[midx].arg1.value=0; mCode[midx].arg1Typ=varg;//const type value name
        mCode[midx].arg2.tidx=ti1; mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    }else{
        resTi=ti1;
    }
    while(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus){//!以+-分隔
        enum MOP op=symBuf[symBufIdx].id==plus?addOp:subOp;
        updateSymBuf();
        ti1=resTi;
        ti2=term();
        resTi=getTemVar();
        mCode[midx].op=op;
        mCode[midx].arg1.tidx=ti1; mCode[midx].arg1Typ=tiarg;//const type value name
        mCode[midx].arg2.tidx=ti2; mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    }
    fprintf(fout,"\t\tthis is an expr.\n");
    return resTi;
}

int term(){//＜项＞::=＜因子＞{＜乘法运算符＞＜因子＞}
    int ti1,ti2,resTi;
    ti1=factor();
    resTi=ti1;
    while(symBuf[symBufIdx].id==mul || symBuf[symBufIdx].id==divd){
        enum MOP op=symBuf[symBufIdx].id==mul?mulOp:divOp;
        updateSymBuf();
        ti1=resTi;
        ti2=factor();
        resTi=getTemVar();
        mCode[midx].op=op;
        mCode[midx].arg1.tidx=ti1; mCode[midx].arg1Typ=tiarg;//const type value name
        mCode[midx].arg2.tidx=ti2; mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;

    }
    fprintf(fout,"\t\tthis is a term.\n");
    return resTi;
}

int factor(){//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
    int resTi;
    if(symBuf[symBufIdx].id==ident){
        enum SYMBOL nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==lparent){
            return call(1);
        }else{
            resTi=lookup(symBuf[symBufIdx].token,0);//标识符
            updateSymBuf();
            if(symBuf[symBufIdx].id==lbrack){//!可选项 ＜标识符＞‘[’＜表达式＞‘]’
                int ti1,ti2;
                if(tab[resTi].kind!=arrkind){
                    error(99);//todo 不为数组
                    return -1;
                }
                ti1=resTi;
                updateSymBuf();
                ti2=expr();
                resTi=getTemVar();
                tab[resTi].typ=tab[ti1].typ;//!类型设为数组元素的类型
                mCode[midx].op=getArrOp;
                mCode[midx].arg1.tidx=ti1; mCode[midx].arg1Typ=tiarg;//const type value name
                mCode[midx].arg2.tidx=ti2; mCode[midx].arg2Typ=tiarg;
                mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
                midx++;
                if(symBuf[symBufIdx].id!=rbrack){
                    error(5);//todo 应是]
                    return -1;
                }
                updateSymBuf();
            }
        }
    }else if(symBuf[symBufIdx].id==charcon){
        resTi=getTemVar();
        tab[resTi].typ=chtyp;
        mCode[midx].op=liop;//todo 改为 li
        mCode[midx].arg1.value=symBuf[symBufIdx].token[1]; mCode[midx].arg1Typ=varg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus || symBuf[symBufIdx].id==zero || symBuf[symBufIdx].id==unsignum){//!整数 first 集合
        resTi=getTemVar();
        mCode[midx].op=liop;
        mCode[midx].arg1.value=numDef(); mCode[midx].arg1Typ=varg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    }else if(symBuf[symBufIdx].id==lparent){//‘(’＜表达式＞‘)’
        updateSymBuf();
        resTi=expr();
        if(symBuf[symBufIdx].id!=rparent){
            error(5);//todo 应是])
            return -1;
        }
        updateSymBuf();
    }else{
        error(5);//todo 非法因子
        return -1;
    }
    fprintf(fout,"\t\tthis is a factor.\n");
    return resTi;
}

void assignment(){//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
    int resTid=-1,ti1=-1,ti2=-1;
    int isArr=0;
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo 应是标识符
        return;
    }
    resTid=lookup(symBuf[symBufIdx].token,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){
        updateSymBuf();//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
        ti2=expr();isArr=1;
        if(symBuf[symBufIdx].id!=rbrack){
            error(5);//todo 应是]
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(5);//todo 应是=
            return;
        }
        updateSymBuf();
        ti1=expr();
    }else if(symBuf[symBufIdx].id==become){//变量赋值＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
        updateSymBuf();
        ti1=expr();//!直接调用
    }else{
        error(5);//todo 非法语句
        return;
    }
    mCode[midx].op=isArr?setArrOp:becomeOp;
    mCode[midx].arg1.tidx=ti1;mCode[midx].arg1Typ=tiarg;//const type value name
    mCode[midx].arg2.tidx=ti2;mCode[midx].arg2Typ=isArr?tiarg:earg;
    mCode[midx].res.tidx=resTid;mCode[midx].rTyp=tiarg;
    midx++;
    fprintf(fout,"\t\tthis is a assignment.\n");
}
/*
 * if cond
 * (true)->
 * (false)-> goto labElse(BP)
 * then
 *  ...
 *  goto labNext(BP)
 * else
 * labElse:
 *  ...
 *  goto labNext
 * end if
 * labNext:
 */


//if开头
void ifStat(){//＜条件语句＞::=if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
    int midxElse=0,midxNext=0,brTi=-1;
    if(symBuf[symBufIdx].id!=ifsy){
        error(5);//应是if
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//应是(
        return;
    }
    updateSymBuf();
    brTi=condition();//!直接调用
    midxElse=midx;
    mCode[midx].op=brfOp;
    mCode[midx].arg1.tidx=brTi;mCode[midx].arg1Typ=tiarg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//应是)
        return;
    }
	updateSymBuf();//!read one more sym
    stat();//!直接调用
    midxNext=midx;
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    mCode[midxElse].res.labIdx=getLab();
    if(symBuf[symBufIdx].id==elsesy){//!可选项
		updateSymBuf();//!one more sym
        stat();
    }
    mCode[midxNext].res.labIdx=getLab();
    fprintf(fout,"\t\tthis is an if stat.\n");
}

int condition(){//＜条件＞::=＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
    int ti1,ti2=-1,resTi;
    int hasOp=0;
    enum MOP op=sltOp;
    ti1=expr();//!直接调用
    if(symBuf[symBufIdx].id==les){//!可选项
        hasOp=1;op=sltOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==leq){
        hasOp=1;op=sleOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==gtr){
        hasOp=1;op=sgtOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==geq){
        hasOp=1;op=sgeOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==eql){
        hasOp=1;op=seqOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==neq){//!漏写
        hasOp=1;op=sneOp;
        updateSymBuf();
        ti2=expr();
    }
    if(hasOp==1){
        resTi=getTemVar();
        mCode[midx].op=op;
        mCode[midx].arg1.tidx=ti1;mCode[midx].arg1Typ=tiarg;//const type value name
        mCode[midx].arg2.tidx=ti2;mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    } else{
        resTi=ti1;
    }
    fprintf(fout,"\t\tthis is a condition.\n");
    return resTi;
}
/*
 * /*
 * labLoop:
 * while cond
 * (true)->
 * (false)-> goto labNext(BP)
 * do
 *  ...
 *  goto labLoop
 * end if
 * labNext:
 */
//while开头
void  whileStat(){//＜循环语句＞::=while ‘(’＜条件＞‘)’＜语句＞
    int loopLabIdx=0,endLabIdx=0,brTi=-1,loopMidx=-1;
    if(symBuf[symBufIdx].id!=whilesy){
        error(5);//todo 应是while
        return;
    }
    loopLabIdx=getLab();
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    brTi=condition();//!直接调用
    loopMidx=midx;
    mCode[midx].op=brfOp;
    mCode[midx].arg1.tidx=brTi;mCode[midx].arg1Typ=tiarg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    stat();//!直接调用
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=loopLabIdx;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    endLabIdx=getLab();
    mCode[loopMidx].res.labIdx=endLabIdx;
    fprintf(fout,"\t\tthis is a while stat.\n");
}
//printf开头
void writeStat(){//＜写语句＞::=printf‘(’ ＜字符串＞,＜表达式＞ ‘)’|printf ‘(’＜字符串＞ ‘)’|printf ‘(’＜表达式＞‘)’
    char str[STRMAX]="";
    int expTid=-1;
    int hasStr=0,hasExp=0;
    if(symBuf[symBufIdx].id!=printfsy){
        error(5);//todo 应是printf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==strcon){
        strcpy(str,symBuf[symBufIdx].token);hasStr=1;
        updateSymBuf();
        if(symBuf[symBufIdx].id==comma){//!可选项
            updateSymBuf();
            expTid=expr();hasExp=1;
        }
    }else{
        expTid=expr();//!直接调用 todo expr first集合
        hasExp=1;
    }
    mCode[midx].op=writeOp;
    mCode[midx].arg1Typ=earg;//const type value name
    if(hasStr){
        mCode[midx].arg2.stridx=enterStr(str);
    }
    mCode[midx].arg2Typ=hasStr?siarg:earg;
    mCode[midx].res.tidx=expTid;mCode[midx].rTyp=hasExp?tiarg:earg;
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack){
            updateSymBuf();//todo
        }
        return;
    }
	updateSymBuf();//!one more sym
    fprintf(fout,"\t\tthis is a write stat.\n");
}
//scanf开头
void readStat(){//＜读语句＞::=scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
    if(symBuf[symBufIdx].id!=scanfsy){
        error(5);//todo 应是scanf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//应是(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==ident){
        int ti=lookup(symBuf[symBufIdx].token,0);
        if(ti==-1){
            error(99);//todo 标识符未定义
            return;
        }
        mCode[midx].op=readOp;
        mCode[midx].arg1Typ=earg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=ti;mCode[midx].rTyp=tiarg;
        midx++;
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//!循环以,分隔
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//todo 应是标识符
                return;
            }
            ti=lookup(symBuf[symBufIdx].token,0);
            if(ti==-1){
                error(99);//todo 标识符未定义
                return;
            }
            mCode[midx].op=readOp;
            mCode[midx].arg1Typ=earg;//const type value name
            mCode[midx].arg2Typ=earg;
            mCode[midx].res.tidx=ti;mCode[midx].rTyp=tiarg;
            midx++;
            updateSymBuf();
        }
        if(symBuf[symBufIdx].id!=rparent){
            error(5);//todo 应是)
            if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack){
                updateSymBuf();//todo
            }
            return;
        }
        updateSymBuf();
    }else{
        error(5);//todo 应为标识符
        return;
    }
    fprintf(fout,"\t\tthis is a read stat.\n");
}
/*
 * swtich E
 * goto labTest(BP)
 * labCase1:
 *      goto labNext(BP)
 * labCase2:
 *      goto labNext(BP)
 * labDefault:
 *      goto labNext(BP)
 * labTest:
 *      if E=e1 goto labCase1(already exits)
 *      if E=e2 goto labCase2(already exits)
 *      goto labDefault      (already exits)
 * labNext:
 */
//switch:    cal E; gen labTest; gen labNext; goto labTest; BP labNext
//case:      cmp E; gen labCase; goto labNext;
//default:   gen labDefault;     goto labNext;
//switch开头
void switchStat(){//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞［＜缺省＞］‘}’
    int eva=-1;
    int swtMidx=-1;
    int hasDefault=0;
    int nextLabIdx=0;int i,brTi;
    /*case record
    int caseCntCur;
    caseCnt++;
    caseCntCur=caseCnt;
     */
    struct CASTAB casetb;
    casetb.caseCnt=0;
    if(symBuf[symBufIdx].id!=switchsy){ error(5);       return; }//todo 应是switch
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){        error(5);        return;    }//todo 应是(
    updateSymBuf();
    eva=expr();//!直接调用
    swtMidx=midx;//FOR BACK PATCH goto TEST
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//goto test
    midx++;
    if(symBuf[symBufIdx].id!=rparent){        error(5);        return;    }//todo 应是)
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){ error(5);return; }//todo 应是{
    updateSymBuf();
    caseStat(&casetb);//!直接调用
    if(symBuf[symBufIdx].id==defaultsy){//!first集合为{default}
        hasDefault=1;
        defaultCase(&casetb);//!可选项
    }
    mCode[swtMidx].res.labIdx=getLab();//gen labTest
    //todo labTest code
    for(i=0;i<casetb.caseCnt-hasDefault;i++){
        brTi=getTemVar();
        mCode[midx].op=sneOp;
        mCode[midx].arg1.value=casetb.cValue[i];mCode[midx].arg1Typ=varg;
        mCode[midx].arg2.tidx=eva;mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=brTi;mCode[midx].rTyp=tiarg;//goto test
        midx++;
        mCode[midx].op=brfOp;
        mCode[midx].arg1.tidx=brTi;mCode[midx].arg1Typ=tiarg;
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.labIdx=casetb.labIdx[i];mCode[midx].rTyp=liarg;//goto test
        midx++;
    }
    if(hasDefault==1){
        mCode[midx].op=jOp;
        mCode[midx].arg1Typ=earg;
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.labIdx=casetb.labIdx[casetb.caseCnt-1];mCode[midx].rTyp=liarg;//goto test
        midx++;
    }
    //gen nextLab
    nextLabIdx=getLab();
    //todo BP nextLab in case and default
    for(i=0;i<casetb.caseCnt;i++){
        mCode[casetb.midx[i]].res.labIdx=nextLabIdx;
    }
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo 应是}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a switch stat.\n");
}
//case开头
void caseStat(struct CASTAB* casetb){//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    if(symBuf[symBufIdx].id!=casesy){//!至少一个case语句
        error(5);//todo 应是case
        return;
    }
    while(symBuf[symBufIdx].id==casesy){//! first集合为{case}
        oneCase(casetb);
    }
    fprintf(fout,"\t\tthis is a case table.\n");
}
//case开头
void oneCase(struct CASTAB* casetb){//＜情况子语句＞::=case＜常量＞：＜语句＞
    if(casetb->caseCnt==CASEMAX){
        error(99);
        return;//case标签过多
    }
    if(symBuf[symBufIdx].id!=casesy){
        error(5);//todo 应是case
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==charcon){
        casetb->cValue[casetb->caseCnt]=symBuf[symBufIdx].token[1];
        //caseTab[caseCnt].cValue=symBuf[symBufIdx].token[1];
        updateSymBuf();
    }else {
        casetb->cValue[casetb->caseCnt]=numDef();
        //caseTab[caseCnt].cValue=numDef();//!直接调用
    }
    if(symBuf[symBufIdx].id!=colon){
        error(5);//todo 应是:
        return;
    }
    updateSymBuf();
    casetb->labIdx[casetb->caseCnt]=getLab();
    //caseTab[caseCnt].labIdx=getLab();
    stat();//!直接调用
    casetb->midx[casetb->caseCnt]=midx;
    //caseTab[caseCnt].midx=midx;
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//goto test
    midx++;
    casetb->caseCnt=casetb->caseCnt+1;
    //caseCnt++;
    fprintf(fout,"\t\tthis is a one case.\n");
}
//default开头
void defaultCase(struct CASTAB* casetb){//＜缺省＞::=default : ＜语句＞
    if(symBuf[symBufIdx].id!=defaultsy){
        error(5);//todo 应是default
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=colon){
        error(5);//todo 应是:
        return;
    }
    updateSymBuf();
    casetb->labIdx[casetb->caseCnt]=getLab();
    //caseTab[caseCnt].labIdx=getLab();
    stat();//!直接调用
    mCode[midx].op=jOp;
    casetb->midx[casetb->caseCnt]=midx;
    //caseTab[caseCnt].midx=midx;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//goto test
    midx++;
    //caseCnt++;
    casetb->caseCnt=casetb->caseCnt+1;
    fprintf(fout,"\t\tthis is a default case.\n");
}
//return开头
void retStat(){//＜返回语句＞::=return[‘(’＜表达式＞‘)’]
    int hasRet=0;
    int expTid=-1;
    if(symBuf[symBufIdx].id!=returnsy){
        error(5);//todo 应是return
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lparent){//!可选项
        updateSymBuf();
        expTid=expr();//!直接调用
        hasRet=1;
        if(symBuf[symBufIdx].id==rparent){
            updateSymBuf();
        }else{
            error(5);//todo 应是)
            if(symBuf[symBufIdx].id==rbrack || symBuf[symBufIdx].id==rbrace){//todo
                updateSymBuf();
            }
            return;
        }
    }
    if(btidx==0){
        error(99);//todo 非法语句
        return;
    }
    if(tab[btab[btidx-1].tidx].typ==voidtyp && hasRet==1){
        error(99);//todo 应为无返回值ret
        return;
    }
    if(tab[btab[btidx-1].tidx].typ==inttyp && (hasRet==0||tab[expTid].typ!=inttyp)){
        error(99);//todo 应为int返回值
    }
    if(tab[btab[btidx-1].tidx].typ==chtyp && (hasRet==0||tab[expTid].typ!=chtyp)){
        error(99);//todo 应为char返回值
    }
    mCode[midx].op=retOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.tidx=expTid;mCode[midx].rTyp=hasRet?tiarg:earg;
    midx++;
    fprintf(fout,"\t\tthis is a return stat.\n");
}
