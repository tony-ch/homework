#include "global.h"
/*int tempBufSel;
int tempBufIdx;
int tempLcnt;
int tempLidx;
char tempCh;
enum symbol tempSym;
int tempNum;
char tempToken[STRMAX];*/
void program(){
    if(symBuf[symBufIdx].id==constsy){//!头符号为const
        decConst();
    }
    int hasVarDef=0;
    while((symBuf[symBufIdx].id==intsy || symBuf[symBufIdx].id==charsy)&&symBuf[(symBufIdx+2)%3].id!=lparent){
        hasVarDef=1;
        varDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是分号
            return;
        }
        updateSymBuf();//!read one more sym
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
}
/*
void saveState(){
    fprintf(fout,"\t\tsave stat.\n");
    tempBufSel=bufsel;
    tempBufIdx=bufidx;
    tempLcnt=lcnt;
    tempLidx=lidx;
    tempCh=ch;
    tempSym=sym;
    tempNum=num;
    strcpy(tempToken,token);
}
void recState(){
    fprintf(fout,"\t\trec stat.\n");
    bufsel=tempBufSel;
    bufidx=tempBufIdx;
    lcnt=tempLcnt;
    lidx=tempLidx;
    ch=tempCh;
    sym=tempSym;
    num=tempNum;
    strcpy(token,tempToken);S
}
*/
//const 开头
void decConst(){
    if(symBuf[symBufIdx].id!=constsy){//!可稍改写文法
        error(2);//应是const
        return;
    }
    updateSymBuf();
    constDef();
    if(symBuf[symBufIdx].id!=semicolon){
        error(2);//应是;
        return;
    }
    updateSymBuf();
    while(symBuf[symBufIdx].id==constsy){//! first集合为{const}
        updateSymBuf();
        constDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是;
            return;
        }
        updateSymBuf();
    }
    fprintf(fout,"\t\tthis is dec of const.\n");
}

void constDef(){
    if(symBuf[symBufIdx].id==intsy){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo 重复代码
            error(2);//应是标识符
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(2);//应是=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){//!first集合
            error(2);//应是数字
            return;
        }
        numDef();
        while(symBuf[symBufIdx].id==comma){//可选项
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//应是标识符
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(2);//应是=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){
                error(2);//应是数字
                return;
            }
            numDef();
        }
    }else if(symBuf[symBufIdx].id==charsy){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo 重复代码
            error(2);//应是标识符
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(2);//应是=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=charcon){
            error(2);//应是数字
            return;
        }
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//可选项
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//应是标识符
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(2);//应是=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=charcon){
                error(2);//应是数字
                return;
            }
            updateSymBuf();
        }
    }else{
        error(2);//todp 应为int或char
        return;
    }
    fprintf(fout,"\t\tthis is const def.\n");
}
/*
void decVar(){//＜变量说明＞::=＜变量定义＞;{＜变量定义＞;}

    while(sym==intsy || sym==charsy){
        getsym();
        if(sym!=ident){
            error(2);//todo 应为标识符
            return;
        }
        getsym();
        bufsel=tempBufSel;
        bufidx=tempBufIdx;
        lcnt=tempLcnt;
        lidx=tempLidx;
        ch=tempCh;
        if(sym==lparent){
            sym=tempSym;
            break;
        }else{
            sym=tempSym;
            varDef();
            if(sym!=semicolon){
                error(2);//todo 应是分号
                return;
            }
            getsym();
        }
    }
}
*/
void varDef(){
    //enum symbol typ=sym;
    if(symBuf[symBufIdx].id!=intsy && symBuf[symBufIdx].id!=charsy){
        error(2);//todo 应为int或char
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(2);//todo 应为标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){//!可选项
        updateSymBuf();
        if(symBuf[symBufIdx].id!=unsignum){
            error(2);//todo 应为无符号整数
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=rbrack){
            error(2);//todo )
            return;
        }
        updateSymBuf();
    }
    while(symBuf[symBufIdx].id==comma){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!不应有类型标识符
            error(2);//todo 应为标识符
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id==lbrack){//!可选项
            updateSymBuf();
            if(symBuf[symBufIdx].id!=unsignum){
                error(2);//todo 应为无符号整数
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=rbrack){
                error(2);//todo )
                return;
            }
            updateSymBuf();
        }
    }
    fprintf(fout,"\t\tthis is var def.\n");
}

void numDef(){//＜整数＞::= ［＋｜－］＜无符号整数＞｜０
    int flag=0;
    if(symBuf[symBufIdx].id==zero){
        updateSymBuf();
    }else{
        if(symBuf[symBufIdx].id==plus){//!可选项
            updateSymBuf();
        }else if(symBuf[symBufIdx].id==minus){
            updateSymBuf();
            flag=1;
        }
        if(symBuf[symBufIdx].id==unsignum){
            //if(flag==1){
                //num=-num;
            //}
            updateSymBuf();
        }else{
            error(2);//todo 应是unsignnum
            return;
        }
    }
    if(flag){
        fprintf(fout,"\t\tthis is a num.\n");
    }else{
        fprintf(fout,"\t\tthis is a num.\n");
    }
}

void retFuncDef(){//＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
    if(symBuf[symBufIdx].id!=charsy&&symBuf[symBufIdx].id!=intsy){
        error(2);//todo 应是类型标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(2);//todo 应是标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    paraList();
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
    if(symBuf[symBufIdx].id!=voidsy){
        error(2);//todo 应是类型标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(2);//todo 应是标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    paraList();
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

void paraList(){//＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
    if(symBuf[symBufIdx].id==charsy||symBuf[symBufIdx].id==intsy){//!可以为空
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){
            error(2);//todo
            return;
        }
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){
            updateSymBuf();
            if(symBuf[symBufIdx].id!=charsy && symBuf[symBufIdx].id!=intsy){
                error(2);//todo
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//todo
                return;
            }
            updateSymBuf();
        }
    }
    fprintf(fout,"\t\tthis is para list.\n");
}

void mainDef(){//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    if(symBuf[symBufIdx].id!=voidsy){
        error(2);//todo 应是类型标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=mainsy){
        error(2);//todo 应是标识符
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    complexStat();
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo 应是(
        return;
    }//!!todo
    fprintf(fout,"\t\tthis is main func dec.\n");
}

/*void call(){
}
*/

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
            error(2);//应是;
        }
        updateSymBuf();
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
            error(2);//应是}
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==scanfsy){
        readStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是;
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==printfsy){
        writeStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是;
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==returnsy){
        retStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是;
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==switchsy){
        switchStat();
    }else if(symBuf[symBufIdx].id==ident){//!first集合相交
        updateSymBuf();
        if(symBuf[symBufIdx].id==become){//变量赋值＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
            updateSymBuf();
            expr();//!直接调用
            fprintf(fout,"\t\tthis is a assignment.\n");
        }else if(symBuf[symBufIdx].id==lbrack){//数组元素赋值
            updateSymBuf();//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
            expr();
            if(symBuf[symBufIdx].id!=rbrack){
                error(2);//todo 应是]
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(2);//todo 应是=
                return;
            }
            updateSymBuf();
            expr();
            fprintf(fout,"\t\tthis is a assignment.\n");
        }else if(symBuf[symBufIdx].id==lparent){//函数调用
            updateSymBuf();//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
            valueParaList();//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
            if(symBuf[symBufIdx].id!=rparent){
                error(2);//todo 应是)
                return;
            }
            updateSymBuf();
            fprintf(fout,"\t\tthis is a call stat.\n");
        }else{
            error(2);//todo  非法语句
            return;
        }
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是;
            return;
        }
        updateSymBuf();
    }else{
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//应是;
            return;
        }
        updateSymBuf();
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
        updateSymBuf();
        if(symBuf[symBufIdx].id==lbrack){//!可选项 ＜标识符＞‘[’＜表达式＞‘]’
            updateSymBuf();
            expr();
            if(symBuf[symBufIdx].id!=rbrack){
                error(2);//todo 应是]
                return;
            }
            updateSymBuf();
        }else if(symBuf[symBufIdx].id==lparent){//＜有返回值函数调用语句＞
            updateSymBuf();//!需要判断是否为有返回值
            valueParaList();
            if(symBuf[symBufIdx].id!=rparent){
                error(2);//应是)
                return;
            }
            updateSymBuf();
            fprintf(fout,"\t\tthis is a call stat.\n");
        }
    }else if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus || symBuf[symBufIdx].id==zero || symBuf[symBufIdx].id==unsignum){//!整数 first 集合
        numDef();
    }else if(symBuf[symBufIdx].id==lparent){//‘(’＜表达式＞‘)’
        updateSymBuf();
        expr();
        if(symBuf[symBufIdx].id!=rparent){
            error(2);//todo 应是])
            return;
        }
        updateSymBuf();
    }else{
        error(2);//todo 非法因子
        return;
    }
    fprintf(fout,"\t\tthis is a factor.\n");
}
/*
void assignment(){
}
*/
//if开头
void ifStat(){//＜条件语句＞::=if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
    if(symBuf[symBufIdx].id!=ifsy){
        error(2);//应是if
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//应是(
        return;
    }
    updateSymBuf();
    condition();//!直接调用
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//应是)
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
        error(2);//todo 应是while
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    condition();//!直接调用
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo 应是)
        return;
    }
    updateSymBuf();
    stat();//!直接调用
    fprintf(fout,"\t\tthis is a while stat.\n");
}
//printf开头
void writeStat(){//＜写语句＞::=printf‘(’ ＜字符串＞,＜表达式＞ ‘)’|printf ‘(’＜字符串＞ ‘)’|printf ‘(’＜表达式＞‘)’
    if(symBuf[symBufIdx].id!=printfsy){
        error(2);//todo 应是printf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo 应是(
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
        error(2);//todo 应是)
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
        error(2);//todo 应是scanf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//应是(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==ident){
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//!循环以,分隔
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//todo 应是标识符
                return;
            }
            updateSymBuf();
        }
        if(symBuf[symBufIdx].id!=rparent){
            error(2);//todo 应是)
            if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack){
                updateSymBuf();//todo
            }
            return;
        }
        updateSymBuf();
    }else{
        error(2);//todo 应为标识符
        return;
    }
    fprintf(fout,"\t\tthis is a read stat.\n");
}
//switch开头
void switchStat(){//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞［＜缺省＞］‘}’
    if(symBuf[symBufIdx].id!=switchsy){
        error(2);//todo 应是switch
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo 应是(
        return;
    }
    updateSymBuf();
    expr();//!直接调用
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo 应是)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo 应是switch
        return;
    }
    updateSymBuf();
    caseStat();//!直接调用
    if(symBuf[symBufIdx].id==defaultsy){//!first集合为{default}
        defaultCase();//!可选项
    }
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo 应是}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a switch stat.\n");
}
//case开头
void caseStat(){//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    if(symBuf[symBufIdx].id!=casesy){//!至少一个case语句
        error(2);//todo 应是case
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
        error(2);//todo 应是case
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else {
        numDef();//!直接调用
    }
    if(symBuf[symBufIdx].id!=colon){
        error(2);//todo 应是:
        return;
    }
    updateSymBuf();
    stat();//!直接调用
    fprintf(fout,"\t\tthis is a one case.\n");
}
//default开头
void defaultCase(){//＜缺省＞::=default : ＜语句＞
    if(symBuf[symBufIdx].id!=defaultsy){
        error(2);//todo 应是default
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=colon){
        error(2);//todo 应是:
        return;
    }
    updateSymBuf();
    stat();//!直接调用
    fprintf(fout,"\t\tthis is a default case.\n");
}
//return开头
void retStat(){//＜返回语句＞::=return[‘(’＜表达式＞‘)’]
    if(symBuf[symBufIdx].id!=returnsy){
        error(2);//todo 应是return
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lparent){//!可选项
        updateSymBuf();
        expr();//!直接调用
        if(symBuf[symBufIdx].id==rparent){
            updateSymBuf();
        }else{
            error(2);//todo 应是)
            if(symBuf[symBufIdx].id==rbrack || symBuf[symBufIdx].id==rbrace){//todo
                updateSymBuf();
            }
            return;
        }
    }
    fprintf(fout,"\t\tthis is a return stat.\n");
}
