#include "err.h"
#include "global.h"

int err[ERRMAX][3];
char errPlace[30];
int ecnt=0;//err cnt;
char* emsg[ERRMAX]={
        "源文件不完整",//0 updatesymBuf
        "非法字符",//1 getsym
        "数有前导0或整数太长",//2 getsym
        "应是 \' ",//3 getsym
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
        "函数未定义,call()中出现,因子和语句分别调用",//16
        "标识符未定义,read中使用",//17
        "常量必须有初始值",//18
        "int常量的初始值应为整数，char常量的初始值应为字符",//19
        "数组的大小应为非0无符号整数",//20
        "非法的整数 numdef中遇到",//21
        "main函数返回类型应为void",//22
        "必须有main函数",//23
        "非法因子",//24
        "非法语句",//25
        "switch至少有一个case",//26
        "case标签过多",//27
        "[]只用于访问数组",//28
        "return语句与函数定义不一致",//29
        "缺少return语句",//30
        "函数调用参数个数不正确",//31
        "函数调用参数类型不正确",//32
        "不能对数组直接赋值"//33
};


void error(int n){
    ecnt=ecnt+1;
    switch(n){
        case 0://"incomplete source file",
            fprintf(fout,"errno:%d %s\n",n,emsg[n]);
            printf("errno:%d %s\n",n,emsg[n]);
            fclose(fin);
            fclose(fout);
            exit(1);
        case 1://"invalid character"
            fprintf(fout,"line:%d col:%d errno:%d %s: %c\n",lcnt,ccnt,n,emsg[n],ch);
            printf("line:%d col:%d errno:%d %s: %c\n",lcnt,ccnt,n,emsg[n],ch);
            break;
        case 2://"0 before unsigned or num too long"
            fprintf(fout,"line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
            printf("line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
            while(isdigit(ch)){
                getch();
            }
            break;
        case 3://"should be \' "
            fprintf(fout,"line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
            printf("line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
            break;
        case 4://"should be ; "
        case 5://redef func
        case 6://redef iden
            printErr(n);
            break;
        case 7://未知的常量类型标识 const def中遇到
            printErr(n);
            while (symBuf[symBufIdx].id!=semicolon){
                updateSymBuf();
            }
            break;
        case 8://未知的参数类型
            printErr(n);
            while (symBuf[symBufIdx].id!=comma && symBuf[symBufIdx].id!=rparent){
                updateSymBuf();
            }
            break;
        case 9://应是标识符
            printErr(n);
            if(strcmp(errPlace,"condef")==0){
                while (symBuf[symBufIdx].id!=semicolon){
                    updateSymBuf();
                }
            }else if(strcmp(errPlace,"vardef")==0){
                while (symBuf[symBufIdx].id!=semicolon && symBuf[symBufIdx].id!=comma){
                    updateSymBuf();
                }
            }else if(strcmp(errPlace,"fundef")==0){
                while (symBuf[symBufIdx].id!=lparent){
                    updateSymBuf();
                }
            } else if(strcmp(errPlace,"paralist")==0){
                while (symBuf[symBufIdx].id!=rparent && symBuf[symBufIdx].id!=comma){
                    updateSymBuf();
                }
            } else if(strcmp(errPlace,"read")==0){
                while (symBuf[symBufIdx].id!=rparent && symBuf[symBufIdx].id!=comma){
                    updateSymBuf();
                }
            }
            errPlace[0]=0;
            break;
        case 10://应是(
            printErr(n);
            if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack)
                updateSymBuf();
            break;
        case 11://应是)
            printErr(n);
            if(symBuf[symBufIdx].id==rbrace || symBuf[symBufIdx].id==rbrack)
                updateSymBuf();
            break;
        case 12://应是{
            printErr(n);
            if(symBuf[symBufIdx].id==lparent || symBuf[symBufIdx].id==lbrack)
                updateSymBuf();
            break;
        case 13://应是}
            printErr(n);
            if(symBuf[symBufIdx].id==rparent || symBuf[symBufIdx].id==rbrack)
                updateSymBuf();
            break;
        case 14://应是]
            printErr(n);
            if(symBuf[symBufIdx].id==rbrace || symBuf[symBufIdx].id==rparent)
                updateSymBuf();
            break;
        case 15://应是:
            printErr(n);
            while(symBuf[symBufIdx].id!=ifsy     &&  symBuf[symBufIdx].id!=whilesy    &&  symBuf[symBufIdx].id!=lbrace &&
                  symBuf[symBufIdx].id!=scanfsy  &&  symBuf[symBufIdx].id!=printfsy   &&  symBuf[symBufIdx].id!=returnsy &&
                  symBuf[symBufIdx].id!=switchsy &&  symBuf[symBufIdx].id!=ident      &&  symBuf[symBufIdx].id!=semicolon){//!first集合
                updateSymBuf();
            }
            break;
        case 16://函数未定义,call()中出现,因子和语句分别调用
            printErr(n);//到语句的结束位置
            if(strcmp(errPlace,"0")==0){//语句中调用
                while(symBuf[symBufIdx].id!=rbrace && symBuf[symBufIdx].id!=semicolon)
                    updateSymBuf();
            }//else//因子中调用//do nothing
            errPlace[0]=0;
            break;
        case 17://标识符未定义 赋值语句 read语句 因子
            printErr(n);//赋值语句的结束位置
            if(strcmp(errPlace,"assign")==0){//赋值语句中调用
                while(symBuf[symBufIdx].id!=semicolon)
                    updateSymBuf();
            }//else//因子中调用//do nothing
            errPlace[0]=0;
            break;
        case 18://常量必须有初始值
        case 19://int常量的初始值应为整数，char常量的初始值应为字符
            printErr(n);//
            while(symBuf[symBufIdx].id!=semicolon)
                updateSymBuf();
            break;
        case 20://数组的大小应为非0无符号整数
            printErr(n);//
            while(symBuf[symBufIdx].id!=rbrack)
                updateSymBuf();
            break;
        case 21://非法的整数 numdef中遇到, constdef 0, factor 1, onecase 2
            printErr(n);//到表达式或者语句的结束位置
            if(strcmp(errPlace,"0")==0){
                while(symBuf[symBufIdx].id!=semicolon)
                    updateSymBuf();
            }else if(strcmp(errPlace,"1")==0){
                while(symBuf[symBufIdx].id!=semicolon && symBuf[symBufIdx].id!=rbrack && symBuf[symBufIdx].id!=rparent)
                    updateSymBuf();
            } else{//one case
                while(symBuf[symBufIdx].id!=semicolon && symBuf[symBufIdx].id!=rbrace)
                    updateSymBuf();
            }
            break;
        case 22://main函数返回类型应为void
            printErr(n);//do nothing
            break;
        case 23://必须有main函数
            printErr(n);//do nothing
            break;
        case 24://非法因子
            printErr(n);
            while(symBuf[symBufIdx].id!=semicolon && symBuf[symBufIdx].id!=rbrack && symBuf[symBufIdx].id!=rparent)
                updateSymBuf();
            break;
        case 25://非法语句
            printErr(n);
            while(symBuf[symBufIdx].id!=semicolon && symBuf[symBufIdx].id!=rbrace)
                updateSymBuf();
            break;
        case 26://switch至少有一个case
        case 27://case标签过多
            printErr(n);//do nothing
            break;
        case 28://[]只用于访问数组
        case 29://return语句与函数定义不一致
        case 30://缺少return语句
        case 31://函数调用参数个数不正确
        case 32://函数调用参数类型不正确
        case 33://不能对数组直接赋值
            printErr(n);//do nothing
            break;
        case 99:
            fprintf(fout,"fatal error: sym tab is full.\n");
            printf("fatal error: sym tab is full.\n");
            exit(1);
        default:
            fprintf(fout,"run time error\n");
            printf("run time error\n");
            exit(1);
        }
}

void printErr(int n){
    fprintf(fout,"error: line:%d col:%d errno:%d %s\n",
            symBuf[symBufIdx].lin,symBuf[symBufIdx].col,n,emsg[n]);
    printf("error: line:%d col:%d errno:%d %s\n",
           symBuf[symBufIdx].lin,symBuf[symBufIdx].col,n,emsg[n]);
}
