#include "err.h"
#include "global.h"

//int err[ERRMAX][3];
char errPlace;
int ecnt = 0;//err cnt;
char *emsg[ERRMAX] = {
        "源文件不完整",//0 updatesymBuf
        "非法字符, 将之跳过",//1 getsym
        "数有前导0或整数太长",//2 getsym
        "应是 \' ",//3 getsym todo 跳读太多
        "应是 ; ",//4
        "函数重定义",//5 enter
        "标识符重定义",//6 enter
        "未知的常量类型标识",//7
        "未知的参数类型",//8
        "应是标识符",//9
        "应是 (",//10
        "应是 )",//11
        "应是{",//12
        "应是}",//13
        "应是]",//14
        "应是:",//15
        "函数未定义",//16
        "标识符未定义",//17
        "常量必须有初始值",//18
        "int常量的初始值应为整数，char常量的初始值应为字符",//19
        "数组的大小应为非0无符号整数",//20
        "非法的整数 ",//21
        "main函数返回类型应为void",//22
        "必须有main函数",//23
        "非法因子",//24
        "非法语句",//25
        "switch至少有一个case",//26
        "case标签过多",//27
        "[]只用于访问数组",//28
        "应为无返回值return",//29
        "缺少return语句",//30
        "函数调用参数个数不正确",//31
        "不能对数组、常量或函数赋值",//32
        "不合法的变量定义",//33
        "函数应以void, int, 或char开始",//34
        "字符串非正常结束",//35
        "语句后的内容不合法",//36
        "位置不合法的常量定义",//37
        "位置不合法的变量定义"//38
};

char *wmsg[ERRMAX] = {
        "赋值语句两端类型不一致",//0
        "返回值类型应为int",//1
        "返回值类型应为char",//2
        "参数类型不匹配",//3
        "常量定义应该在变量定义之前"//4
};

void warn(int n) {
    fprintf(fout, "warn: line:%d col:%d %s\n",
            symBuf[symBufIdx].lin, symBuf[symBufIdx].col, wmsg[n]);
    printf("warn: line:%d col:%d %s\n",
           symBuf[symBufIdx].lin, symBuf[symBufIdx].col, wmsg[n]);
}


void showPos() {
    fprintf(fout, "跳读到 (line: %d, col: %d): %s\n", symBuf[symBufIdx].lin, symBuf[symBufIdx].col,
            symBuf[symBufIdx].token);
    printf("跳读到 (line: %d, col: %d): %s\n", symBuf[symBufIdx].lin, symBuf[symBufIdx].col, symBuf[symBufIdx].token);
}

void error(int n) {
    ecnt = ecnt + 1;
    switch (n) {
        case 0://"incomplete source file",
            fprintf(fout, "errno:%d %s\n", n, emsg[n]);
            printf("errno:%d %s\n", n, emsg[n]);
            endProc(1);
        case 1://"invalid character"
            fprintf(fout, "line:%d col:%d errno:%d %s: %c\n", lcnt, ccnt, n, emsg[n], ch);
            printf("line:%d col:%d errno:%d %s: %c\n", lcnt, ccnt, n, emsg[n], ch);
            break;
        case 2://"0 before unsigned or num too long"
            fprintf(fout, "line:%d col:%d errno:%d %s\n", lcnt, ccnt, n, emsg[n]);
            printf("line:%d col:%d errno:%d %s\n", lcnt, ccnt, n, emsg[n]);
            while (isdigit(ch)) {
                getch();
            }
            break;
        case 3://"should be \' "
            fprintf(fout, "line:%d col:%d errno:%d %s\n", lcnt, ccnt, n, emsg[n]);
            printf("line:%d col:%d errno:%d %s\n", lcnt, ccnt, n, emsg[n]);
            break;
        case 4://"should be ; "
        case 5://redef func
        case 6://redef iden
            printErr(n);
            break;
        case 7://未知的常量类型标识 const def中遇到
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon) {
                updateSymBuf();
            }
            showPos();
            break;
        case 8://未知的参数类型
            printErr(n);
            while (symBuf[symBufIdx].id != comma && symBuf[symBufIdx].id != rparent) {
                updateSymBuf();
            }
            showPos();
            break;
        case 9://应是标识符
            printErr(n);
            if (errPlace == 'c') {//condef
                while (symBuf[symBufIdx].id != semicolon) {
                    updateSymBuf();
                }
            } else if (errPlace == 'v') {//vardef
                while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != comma) {
                    updateSymBuf();
                }
            } else if (errPlace == 'f') {//funcdef
                while (symBuf[symBufIdx].id != lparent) {
                    updateSymBuf();
                }
            } else if (errPlace == 'p') {//paralist
                while (symBuf[symBufIdx].id != rparent && symBuf[symBufIdx].id != comma) {
                    updateSymBuf();
                }
            } else if (errPlace == 'r') {//read
                while (symBuf[symBufIdx].id != rparent && symBuf[symBufIdx].id != comma) {
                    updateSymBuf();
                }
            }
            showPos();
            errPlace = 0;
            break;
        case 10://应是(
            printErr(n);
            if (symBuf[symBufIdx].id == lbrace || symBuf[symBufIdx].id == lbrack)
                updateSymBuf();
            break;
        case 11://应是)
            printErr(n);
            if (symBuf[symBufIdx].id == rbrace || symBuf[symBufIdx].id == rbrack)
                updateSymBuf();
            break;
        case 12://应是{
            printErr(n);
            if (symBuf[symBufIdx].id == lparent || symBuf[symBufIdx].id == lbrack)
                updateSymBuf();
            break;
        case 13://应是}
            printErr(n);
            if (symBuf[symBufIdx].id == rparent || symBuf[symBufIdx].id == rbrack)
                updateSymBuf();
            break;
        case 14://应是]
            printErr(n);
            if (symBuf[symBufIdx].id == rbrace || symBuf[symBufIdx].id == rparent)
                updateSymBuf();
            break;
        case 15://应是:
            printErr(n);
            while (symBuf[symBufIdx].id != ifsy && symBuf[symBufIdx].id != whilesy && symBuf[symBufIdx].id != lbrace &&
                   symBuf[symBufIdx].id != scanfsy && symBuf[symBufIdx].id != printfsy &&
                   symBuf[symBufIdx].id != returnsy &&
                   symBuf[symBufIdx].id != switchsy && symBuf[symBufIdx].id != ident &&
                   symBuf[symBufIdx].id != semicolon) {//!first集合
                updateSymBuf();
            }
            showPos();
            break;
        case 16://函数未定义,call()中出现,因子和语句分别调用
            printErr(n);//到语句的结束位置
            if (errPlace == 's') {//语句中调用
                while (symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
            } else {//factor f
                while (symBuf[symBufIdx].id != rparent)
                    updateSymBuf();
                updateSymBuf();
            }
            showPos();
            errPlace = 0;
            break;
        case 17://标识符未定义 赋值语句 read语句 因子
            printErr(n);//赋值语句的结束位置
            if (errPlace == 'a') {//赋值语句中调用 assign
                while (symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
                showPos();
            }//else//因子中调用 f //do nothing
            errPlace = 0;
            break;
        case 18://常量必须有初始值
        case 19://int常量的初始值应为整数，char常量的初始值应为字符
            printErr(n);//
            while (symBuf[symBufIdx].id != semicolon)
                updateSymBuf();
            showPos();
            break;
        case 20://数组的大小应为非0无符号整数
            printErr(n);//
            while (symBuf[symBufIdx].id != rbrack)
                updateSymBuf();
            showPos();
            break;
        case 21://非法的整数 numdef中遇到, constdef c, factor f, onecase o
            printErr(n);//到表达式或者语句的结束位置
            if (errPlace == 'c') {//constdef
                while (symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
            } else if (errPlace == 'f') {//factor
                while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrack &&
                       symBuf[symBufIdx].id != rparent)
                    updateSymBuf();
            } else {//one case
                while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrace)
                    updateSymBuf();
            }
            showPos();
            break;
        case 22://main函数返回类型应为void
            printErr(n);//do nothing
            break;
        case 23://必须有main函数
            printErr(n);//do nothing
            break;
        case 24://非法因子
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrack &&
                   symBuf[symBufIdx].id != rparent)
                updateSymBuf();
            showPos();
            break;
        case 25://非法语句
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrace)
                updateSymBuf();
            showPos();
            break;
        case 26://switch至少有一个case
        case 27://case标签过多
            printErr(n);//do nothing
            break;
        case 28://[]只用于访问数组
        case 29://应为无返回值return
        case 30://缺少return语句
        case 31://函数调用参数个数不正确
        case 32://不能对数组函数和常量赋值
            printErr(n);//do nothing
            break;
        case 33://不合法的变量定义
            printErr(n);
            while (symBuf[symBufIdx].id != comma && symBuf[symBufIdx].id != semicolon)
                updateSymBuf();
            showPos();
            break;
        case 34://函数应以void, int, 或char开始
            printErr(n);
            while (symBuf[symBufIdx].id != charsy && symBuf[symBufIdx].id != voidsy && symBuf[symBufIdx].id != intsy) {
                updateSymBuf();
            }
            showPos();
            break;
        case 35:
            printErr(n);
            break;
        case 36:
            printErr(n);
            while (symBuf[symBufIdx].id != ifsy && symBuf[symBufIdx].id != whilesy && symBuf[symBufIdx].id != lbrace &&
                   symBuf[symBufIdx].id != scanfsy && symBuf[symBufIdx].id != printfsy &&
                   symBuf[symBufIdx].id != returnsy &&
                   symBuf[symBufIdx].id != switchsy && symBuf[symBufIdx].id != ident &&
                   symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrace) {
                if (errPlace == 'i' && symBuf[symBufIdx].id == elsesy)//else after if is valid
                    break;
                if (errPlace == 'c' && (symBuf[symBufIdx].id == defaultsy ||
                                        symBuf[symBufIdx].id == casesy))// case and default after case is valid
                    break;
                updateSymBuf();
            }
            errPlace = 0;
            showPos();
            break;
        case 37:
        case 38:
            printErr(n);
            break;
        case 99:
            fprintf(fout, "fatal error: sym tab is full.\n");
            printf("fatal error: sym tab is full.\n");
            endProc(99);
        default:
            fprintf(fout, "run time error\n");
            printf("run time error\n");
            endProc(-1);
    }
}

void printErr(int n) {
    fprintf(fout, "error: line:%d col:%d errno:%d %s\n",
            symBuf[symBufIdx].lin, symBuf[symBufIdx].col, n, emsg[n]);
    printf("error: line:%d col:%d errno:%d %s\n",
           symBuf[symBufIdx].lin, symBuf[symBufIdx].col, n, emsg[n]);
}


void testAfterStat(char pos) {
    enum SYMBOL sym = symBuf[symBufIdx].id;
    if (pos == 'i' && sym == elsesy)
        return;
    if (pos == 'c' && (sym == casesy || sym == defaultsy))
        return;
    if (sym != ifsy && sym != whilesy && sym != lbrace && sym != scanfsy && sym != printfsy && sym != returnsy &&
        sym != switchsy && sym != ident && sym != semicolon && sym != rbrace) {
        errPlace = pos;// save errPos
        error(36);
//        switch (pos){
//            case 's'://statlist
//            case 'e'://else
//            case 'd'://default
//            case 'w'://while
//                error(36);
//                break;
//            case 'i'://if
//                if(sym!=elsesy)
//                    error(36);
//                break;
//            case 'c'://case
//                if(sym!=casesy && sym!=defaultsy)
//                    error(36);
//                break;
//            default:
//                break;
//        }
    }
}