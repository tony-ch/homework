#include "global.h"
#include "err.h"
struct tabrecord tab[TMAX];
int tidx=0;//tab top index
struct btabrecord btab[TMAX];
int btidx=0;//func tab top index

char kindstr[5][10]={"var","const","func","arr","para"};
char typestr[3][10]={"void","int","char"};
void enter(char *name, enum kinds k, enum types t, int value){
    if(tidx==TMAX){
        error(99);//TODO
        exit(1);
    }
    if(k==funkind){
        int i;
        for(i=0;i<btidx;i++){
            if(strcmp(btab[i].name,name)==0){
                error(99);//todo redef func
                return;
            }
        }
    } else{
        int i;
        i=(btidx==0)?0:btab[btidx-1].tidx+1;// if global field, search from 0; if func field, search after func name.
        for(;i<tidx;i++){
            if(strcmp(tab[i].name,name)==0){
                error(99);//todo redef iden
                return;
            }
        }
    }
    strcpy(tab[tidx].name,name);
    tab[tidx].kind=k;
    tab[tidx].typ=t;
    tab[tidx].value=value;
    fprintf(fout,"\t\tenter tab index: %d, name: %s, kind: %s, type: %s, value: %d\n",
            tidx,tab[tidx].name,kindstr[tab[tidx].kind],typestr[tab[tidx].typ],tab[tidx].value);
    if(k==funkind){
        strcpy(btab[btidx].name,name);
        btab[btidx].tidx=tidx;
        fprintf(fout,"\t\tenter btab index: %d, name: %s, tidx: %d\n",
                btidx,btab[btidx].name,btab[btidx].tidx);
        btidx=btidx+1;
    }
    tidx=tidx+1;
}


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
        fprintf(fout,"\t\ttab index: %d,\tname: %s,\tkind: %s,\ttype: %s,\tvalue: %d\n",
        i,tab[i].name,kindstr[tab[i].kind],typestr[tab[i].typ],tab[i].value);
    }
    for(i=0;i<btidx;i++){
        fprintf(fout,"\t\tbtab index: %d,\tname: %s,\ttidx: %d\n",
                i,btab[i].name,btab[i].tidx);
    }

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
    enum kinds kind=conkind;
    enum types type;
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
        enter(name,kind,type,value);
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
            enter(name,kind,type,value);
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
        enter(name,kind,type,value);
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
            enter(name,kind,type,value);
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
    enum kinds kind;
    enum types type;
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
    value=0;
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
    enter(name,kind,type,value);
    while(symBuf[symBufIdx].id==comma){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!不应有类型标识符
            error(5);//todo 应为标识符
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        kind=varkind;
        value=0;
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
        enter(name,kind,type,value);
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
    enum kinds kind;
    enum types type;
    int value;
    int tabIdxToEnter;
    kind=funkind;
    tabIdxToEnter=tidx;
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
    enter(name,kind,type,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tabIdxToEnter].value=value;
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tabIdxToEnter].name,tab[tabIdxToEnter].value);
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
    updateSymBuf();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum kinds kind;
    enum types type;
    int value;
    int tabIdxToEnter;
    kind=funkind;
    tabIdxToEnter=tidx;
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
    enter(name,kind,type,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tabIdxToEnter].value=value;
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tabIdxToEnter].name,tab[tabIdxToEnter].value);
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
    updateSymBuf();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

int paraList(){//＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
    char name[ALENMAX];
    enum kinds kind;
    enum types type;
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
        enter(name,kind,type,0);
        paraCnt=paraCnt+1;
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
            enter(name,kind,type,0);
            paraCnt=paraCnt+1;
            updateSymBuf();
        }
    }
    fprintf(fout,"\t\tthis is para list.\n");
    return paraCnt;
}

void mainDef(){//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum kinds kind;
    enum types type;
    kind=funkind;
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
    enter(name,kind,type,0);
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
    fprintf(fout,"\t\tthis is main func dec.\n");
}

void call(){
    if(symBuf[symBufIdx].id!=ident){
        error(5);//tofo
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//tofo
        return;
    }
    updateSymBuf();//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
    valueParaList();//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a call stat.\n");
}


void valueParaList(){//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
    if(symBuf[symBufIdx].id==rparent){//!空
        //getsym();//!
    }else{
        expr();//!至少一个
        while(symBuf[symBufIdx].id==comma){
            updateSymBuf();
            expr();
        }
    }
    fprintf(fout,"\t\tthis is value para list.\n");
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
        enum symbol nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==become || nextSym==lbrack){
            assignment();
        }else if(nextSym==lparent){
            call();
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

void expr(){//＜表达式＞::=［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus){//!可选项
        updateSymBuf();
    }
    term();
    while(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus){//!以+-分隔
        updateSymBuf();
        term();
    }
    fprintf(fout,"\t\tthis is an expr.\n");
}

void term(){//＜项＞::=＜因子＞{＜乘法运算符＞＜因子＞}
    factor();
    while(symBuf[symBufIdx].id==mul || symBuf[symBufIdx].id==divd){
        updateSymBuf();
        factor();
    }
    fprintf(fout,"\t\tthis is a term.\n");
}

void factor(){//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
    if(symBuf[symBufIdx].id==ident){
        enum symbol nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==lparent){
            call();
        }else{
            updateSymBuf();
            if(symBuf[symBufIdx].id==lbrack){//!可选项 ＜标识符＞‘[’＜表达式＞‘]’
                updateSymBuf();
                expr();
                if(symBuf[symBufIdx].id!=rbrack){
                    error(5);//todo 应是]
                    return;
                }
                updateSymBuf();
            }
        }
    }else if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus || symBuf[symBufIdx].id==zero || symBuf[symBufIdx].id==unsignum){//!整数 first 集合
        numDef();
    }else if(symBuf[symBufIdx].id==lparent){//‘(’＜表达式＞‘)’
        updateSymBuf();
        expr();
        if(symBuf[symBufIdx].id!=rparent){
            error(5);//todo 应是])
            return;
        }
        updateSymBuf();
    }else{
        error(5);//todo 非法因子
        return;
    }
    fprintf(fout,"\t\tthis is a factor.\n");
}

void assignment(){//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo 应是标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){
        updateSymBuf();//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
        expr();
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
        expr();
    }else if(symBuf[symBufIdx].id==become){//变量赋值＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
        updateSymBuf();
        expr();//!直接调用
    }else{
        error(5);//todo 非法语句
        return;
    }
    fprintf(fout,"\t\tthis is a assignment.\n");
}
//if开头
void ifStat(){//＜条件语句＞::=if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
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
    condition();//!直接调用
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//应是)
        return;
    }
	updateSymBuf();//!read one more sym
    stat();//!直接调用
    if(symBuf[symBufIdx].id==elsesy){//!可选项
		updateSymBuf();//!one more sym
        stat();
    }
    fprintf(fout,"\t\tthis is an if stat.\n");
}

void condition(){//＜条件＞::=＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
    expr();//!直接调用
    if(symBuf[symBufIdx].id==les){//!可选项
        updateSymBuf();
        expr();
    }else if(symBuf[symBufIdx].id==leq){
        updateSymBuf();
        expr();
    }else if(symBuf[symBufIdx].id==gtr){
        updateSymBuf();
        expr();
    }else if(symBuf[symBufIdx].id==geq){
        updateSymBuf();
        expr();
    }else if(symBuf[symBufIdx].id==eql){
        updateSymBuf();
        expr();
    }else if(symBuf[symBufIdx].id==neq){//!漏写
        updateSymBuf();
        expr();
    }
    fprintf(fout,"\t\tthis is a condition.\n");
}
//while开头
void whileStat(){//＜循环语句＞::=while ‘(’＜条件＞‘)’＜语句＞
    if(symBuf[symBufIdx].id!=whilesy){
        error(5);//todo 应是while
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    condition();//!直接调用
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    stat();//!直接调用
    fprintf(fout,"\t\tthis is a while stat.\n");
}
//printf开头
void writeStat(){//＜写语句＞::=printf‘(’ ＜字符串＞,＜表达式＞ ‘)’|printf ‘(’＜字符串＞ ‘)’|printf ‘(’＜表达式＞‘)’
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
        updateSymBuf();
        if(symBuf[symBufIdx].id==comma){//!可选项
            updateSymBuf();
            expr();
        }
    }else{
        expr();//!直接调用 todo expr first集合
    }
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
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//!循环以,分隔
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//todo 应是标识符
                return;
            }
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
//switch开头
void switchStat(){//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞［＜缺省＞］‘}’
    if(symBuf[symBufIdx].id!=switchsy){
        error(5);//todo 应是switch
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo 应是(
        return;
    }
    updateSymBuf();
    expr();//!直接调用
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo 应是switch
        return;
    }
    updateSymBuf();
    caseStat();//!直接调用
    if(symBuf[symBufIdx].id==defaultsy){//!first集合为{default}
        defaultCase();//!可选项
    }
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo 应是}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a switch stat.\n");
}
//case开头
void caseStat(){//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    if(symBuf[symBufIdx].id!=casesy){//!至少一个case语句
        error(5);//todo 应是case
        return;
    }
    while(symBuf[symBufIdx].id==casesy){//! first集合为{case}
        oneCase();
    }
    fprintf(fout,"\t\tthis is a case table.\n");
}
//case开头
void oneCase(){//＜情况子语句＞::=case＜常量＞：＜语句＞
    if(symBuf[symBufIdx].id!=casesy){
        error(5);//todo 应是case
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else {
        numDef();//!直接调用
    }
    if(symBuf[symBufIdx].id!=colon){
        error(5);//todo 应是:
        return;
    }
    updateSymBuf();
    stat();//!直接调用
    fprintf(fout,"\t\tthis is a one case.\n");
}
//default开头
void defaultCase(){//＜缺省＞::=default : ＜语句＞
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
    stat();//!直接调用
    fprintf(fout,"\t\tthis is a default case.\n");
}
//return开头
void retStat(){//＜返回语句＞::=return[‘(’＜表达式＞‘)’]
    if(symBuf[symBufIdx].id!=returnsy){
        error(5);//todo 应是return
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lparent){//!可选项
        updateSymBuf();
        expr();//!直接调用
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
    fprintf(fout,"\t\tthis is a return stat.\n");
}
