#include "global.h"

void program(){
    if(sym==constsy){
        decConst();
    }

}
//const 开头
void decConst(){
    if(sym!=constsy){//!可稍改写文法
        error(2);//应是const
        return;
    }
    getsym();
    constDef();
    if(sym!=semicolon){
        error(2);//应是;
        return;
    }
    getsym();
    while(sym==constsy){//! first集合为{const}
        getsym();
        constDef();
        if(sym!=semicolon){
            error(2);//应是;
            return;
        }
        getsym();
    }
    printf("this is dec of const.\n");
}

void constDef(){
    if(sym==intsy){
        getsym();
        if(sym!=ident){//!todo 重复代码
            error(2);//应是标识符
            return;
        }
        getsym();
        if(sym!=become){
            error(2);//应是=
            return;
        }
        getsym();
        if(sym!=minus &&sym!=plus && sym!=unsignum && sym!=zero){
            error(2);//应是数字
            return;
        }
        numDef();
        while(sym==comma){//可选项
            getsym();
            if(sym!=ident){
                error(2);//应是标识符
                return;
            }
            getsym();
            if(sym!=become){
                error(2);//应是=
                return;
            }
            getsym();
            if(sym!=minus &&sym!=plus && sym!=unsignum && sym!=zero){
                error(2);//应是数字
                return;
            }
            numDef();
        }
    }else if(sym==charsy){
        getsym();
        if(sym!=ident){//!todo 重复代码
            error(2);//应是标识符
            return;
        }
        getsym();
        if(sym!=become){
            error(2);//应是=
            return;
        }
        getsym();
        if(sym!=charcon){
            error(2);//应是数字
            return;
        }
        getsym();
        while(sym==comma){//可选项
            getsym();
            if(sym!=ident){
                error(2);//应是标识符
                return;
            }
            getsym();
            if(sym!=become){
                error(2);//应是=
                return;
            }
            getsym();
            if(sym!=charcon){
                error(2);//应是数字
                return;
            }
            getsym();
        }
    }else{
        error(2);//todp 应为int或char
        return;
    }
    printf("this is const def.\n");
}

void decVar(){
}

void varDef(){

}

void numDef(){//＜整数＞::= ［＋｜－］＜无符号整数＞｜０
    int flag=0;
    if(sym==zero){
        getsym();
    }else{
        if(sym==plus){//!可选项
            getsym();
        }else if(sym==minus){
            getsym();
            flag=1;
        }
        if(sym==unsignum){
            if(flag==1){
                num=-num;
            }
            getsym();
        }else{
            error(2);//todo 应是unsignnum
            return;
        }
    }
    printf("this is a num:%d\n",num);
}

void retFuncDef(){
}

void voidFuncDef(){
}

void paraList(){

}

void mainDef(){
}

/*void call(){
}
*/

void valueParaList(){//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
    if(sym==rparent){//!空
        getsym();
    }else{
        expr();//!至少一个
        while(sym==comma){
            getsym();
            expr();
        }
    }
    printf("this is value para list.\n");
}

void complexStat(){
}

void stat(){//＜语句＞::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
    if(sym==ifsy){//!头符号集合
        ifStat();
    }else if(sym==whilesy){
        whileStat();
    }else if(sym==lbrace){//语句列
        getsym();
        statList();
        if(sym!=rbrace){
            error(2);//应是}
            return;
        }
        getsym();
    }else if(sym==scanfsy){
        readStat();
        if(sym!=semicolon){
            error(2);//应是;
            return;
        }
        getsym();
    }else if(sym==printfsy){
        writeStat();
        if(sym!=semicolon){
            error(2);//应是;
            return;
        }
        getsym();
    }else if(sym==returnsy){
        retStat();
        if(sym!=semicolon){
            error(2);//应是;
            return;
        }
        getsym();
    }else if(sym==switchsy){
        switchStat();
    }else if(sym==ident){//!first集合相交
        getsym();
        if(sym==become){//变量赋值＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
            getsym();
            expr();//!直接调用
            printf("this is a assignment.\n");
        }else if(sym==lbrack){//数组元素赋值
            getsym();//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
            expr();
            if(sym!=rbrack){
                error(2);//todo 应是]
                return;
            }
            getsym();
            if(sym!=become){
                error(2);//todo 应是=
                return;
            }
            getsym();
            expr();
            printf("this is a assignment.\n");
        }else if(sym==lparent){//函数调用
            getsym();//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
            valueParaList();//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
            if(sym!=rparent){
                error(2);//todo 应是)
                return;
            }
            getsym();
            printf("this is a call stat.\n");
        }else{
            error(2);//todo  非法语句
            return;
        }
        if(sym!=semicolon){
            error(2);//应是;
            return;
        }
        getsym();
    }else{
        if(sym!=semicolon){
            error(2);//应是;
            return;
        }
        getsym();
    }//!空
    printf("this is a stat.\n");
}

void statList(){
    //!可以没有一条
    while(sym==ifsy     ||  sym==whilesy    ||  sym==lbrace ||
          sym==scanfsy  ||  sym==printfsy   ||  sym==returnsy ||
          sym==switchsy ||  sym==ident      ||  sym==semicolon){//!first集合
        stat();
    }
    printf("this is a stat list.\n");
}

void expr(){//＜表达式＞::=［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    if(sym==plus || sym==minus){//!可选项
        getsym();
    }
    term();
    while(sym==plus || sym==minus){//!以+-分隔
        getsym();
        term();
    }
    printf("this is an expr.\n");
}

void term(){//＜项＞::=＜因子＞{＜乘法运算符＞＜因子＞}
    factor();
    while(sym==mul || sym==divd){
        getsym();
        factor();
    }
    printf("this is a term.\n");
}

void factor(){//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
    if(sym==ident){
        getsym();
        if(sym==lbrack){//!可选项 ＜标识符＞‘[’＜表达式＞‘]’
            getsym();
            expr();
            if(sym!=rbrack){
                error(2);//todo 应是]
                return;
            }
            getsym();
        }else if(sym==lparent){//＜有返回值函数调用语句＞
            getsym();//!需要判断是否为有返回值
            valueParaList();
            if(sym!=rparent){
                error(2);//应是)
                return;
            }
            getsym();
            printf("this is a call stat.\n");
        }
    }else if(sym==charcon){
        getsym();
    }else if(sym==plus || sym==minus || sym==zero || sym==unsignum){//!整数 first 集合
        numDef();
    }else if(sym==lparent){//‘(’＜表达式＞‘)’
        getsym();
        expr();
        if(sym!=rparent){
            error(2);//todo 应是])
            return;
        }
        getsym();
    }else{
        error(2);//todo 非法因子
        return;
    }
    printf("this is a factor.\n");
}
/*
void assignment(){
}
*/
//if开头
void ifStat(){//＜条件语句＞::=if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
    if(sym!=ifsy){
        error(2);//应是if
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//应是(
        return;
    }
    getsym();
    condition();//!直接调用
    if(sym!=rparent){
        error(2);//应是)
        return;
    }
    stat();//!直接调用
    if(sym==elsesy){//!可选项
        stat();
    }
    printf("this is an if stat.\n");
}

void condition(){//＜条件＞::=＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
    expr();//!直接调用
    if(sym==les){//!可选项
        getsym();
        expr();
    }else if(sym==leq){
        getsym();
        expr();
    }else if(sym==gtr){
        getsym();
        expr();
    }else if(sym==geq){
        getsym();
        expr();
    }else if(sym==eql){
        getsym();
        expr();
    }
    printf("this is a condition.\n");
}
//while开头
void whileStat(){//＜循环语句＞::=while ‘(’＜条件＞‘)’＜语句＞
    if(sym!=whilesy){
        error(2);//todo 应是while
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo 应是(
        return;
    }
    getsym();
    condition();//!直接调用
    if(sym!=rparent){
        error(2);//todo 应是)
        return;
    }
    getsym();
    stat();//!直接调用
    printf("this is a while stat.\n");
}
//printf开头
void writeStat(){//＜写语句＞::=printf‘(’ ＜字符串＞,＜表达式＞ ‘)’|printf ‘(’＜字符串＞ ‘)’|printf ‘(’＜表达式＞‘)’
    if(sym!=printfsy){
        error(2);//todo 应是printf
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo 应是(
        return;
    }
    getsym();
    if(sym==strcon){
        getsym();
        if(sym==comma){//!可选项
            getsym();
            expr();
        }
    }else{
        expr();//!直接调用 todo expr first集合
    }
    if(sym!=rparent){
        error(2);//todo 应是)
        if(sym==lbrace || sym==lbrack){
            getsym();//todo
        }
        return;
    }
    printf("this is a write stat.\n");
}
//scanf开头
void readStat(){//＜读语句＞::=scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
    if(sym!=scanfsy){
        error(2);//todo 应是scanf
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//应是(
        return;
    }
    getsym();
    if(sym==ident){
        getsym();
        while(sym==comma){//!循环以,分隔
            getsym();
            if(sym!=ident){
                error(2);//todo 应是标识符
                return;
            }
            getsym();
        }
        if(sym!=rparent){
            error(2);//todo 应是)
            if(sym==lbrace || sym==lbrack){
                getsym();//todo
            }
            return;
        }
        getsym();
    }else{
        error(2);//todo 应为标识符
        return;
    }
    printf("this is a read stat.\n");
}
//switch开头
void switchStat(){//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞［＜缺省＞］‘}’
    if(sym!=switchsy){
        error(2);//todo 应是switch
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo 应是(
        return;
    }
    getsym();
    expr();//!直接调用
    if(sym!=rparent){
        error(2);//todo 应是)
        return;
    }
    getsym();
    if(sym!=lbrace){
        error(2);//todo 应是switch
        return;
    }
    getsym();
    caseStat();//!直接调用
    if(sym==defaultsy){//!first集合为{default}
        defaultCase();//!可选项
    }
    if(sym!=rbrace){
        error(2);//todo 应是}
        return;
    }
    getsym();
    printf("this is a switch stat.\n");
}
//case开头
void caseStat(){//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    if(sym!=casesy){//!至少一个case语句
        error(2);//todo 应是case
        return;
    }
    while(sym==casesy){//! first集合为{case}
        oneCase();
    }
    printf("this is a case table.\n");
}
//case开头
void oneCase(){//＜情况子语句＞::=case＜常量＞：＜语句＞
    if(sym!=casesy){
        error(2);//todo 应是case
        return;
    }
    getsym();
    if(sym==charcon){
        getsym();
    }else {
        numDef();//!直接调用
    }
    if(sym!=colon){
        error(2);//todo 应是:
        return;
    }
    getsym();
    stat();//!直接调用
    printf("this is a one case.\n");
}
//default开头
void defaultCase(){//＜缺省＞::=default : ＜语句＞
    if(sym!=defaultsy){
        error(2);//todo 应是default
        return;
    }
    getsym();
    if(sym!=colon){
        error(2);//todo 应是:
        return;
    }
    getsym();
    stat();//!直接调用
    printf("this is a default case.\n");
}
//return开头
void retStat(){//＜返回语句＞::=return[‘(’＜表达式＞‘)’]
    if(sym!=returnsy){
        error(2);//todo 应是return
        return;
    }
    getsym();
    if(sym==lparent){//!可选项
        getsym();
        expr();//!直接调用
        if(sym==rparent){
            getsym();
        }else{
            error(2);//todo 应是)
            if(sym==rbrack || sym==rbrace){//todo
                getsym();
            }
            return;
        }
    }
    printf("this is a return stat.\n");
}
