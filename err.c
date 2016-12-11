#include "err.h"
#include "global.h"

//int err[ERRMAX][3];
char errPlace;
int ecnt = 0;//err cnt;
char *emsg[ERRMAX] = {
        "Դ�ļ�������",//0 updatesymBuf
        "�Ƿ��ַ�, ��֮����",//1 getsym
        "����ǰ��0������̫��",//2 getsym
        "Ӧ�� \' ",//3 getsym todo ����̫��
        "Ӧ�� ; ",//4
        "�����ض���",//5 enter
        "��ʶ���ض���",//6 enter
        "δ֪�ĳ������ͱ�ʶ",//7
        "δ֪�Ĳ�������",//8
        "Ӧ�Ǳ�ʶ��",//9
        "Ӧ�� (",//10
        "Ӧ�� )",//11
        "Ӧ��{",//12
        "Ӧ��}",//13
        "Ӧ��]",//14
        "Ӧ��:",//15
        "����δ����",//16
        "��ʶ��δ����",//17
        "���������г�ʼֵ",//18
        "int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�",//19
        "����Ĵ�СӦΪ��0�޷�������",//20
        "�Ƿ������� ",//21
        "main������������ӦΪvoid",//22
        "������main����",//23
        "�Ƿ�����",//24
        "�Ƿ����",//25
        "switch������һ��case",//26
        "case��ǩ����",//27
        "[]ֻ���ڷ�������",//28
        "ӦΪ�޷���ֵreturn",//29
        "ȱ��return���",//30
        "�������ò�����������ȷ",//31
        "���ܶ����顢����������ֵ",//32
        "���Ϸ��ı�������",//33
        "����Ӧ��void, int, ��char��ʼ",//34
        "�ַ�������������",//35
        "��������ݲ��Ϸ�",//36
        "λ�ò��Ϸ��ĳ�������",//37
        "λ�ò��Ϸ��ı�������"//38
};

char *wmsg[ERRMAX] = {
        "��ֵ����������Ͳ�һ��",//0
        "����ֵ����ӦΪint",//1
        "����ֵ����ӦΪchar",//2
        "�������Ͳ�ƥ��",//3
        "��������Ӧ���ڱ�������֮ǰ"//4
};

void warn(int n) {
    fprintf(fout, "warn: line:%d col:%d %s\n",
            symBuf[symBufIdx].lin, symBuf[symBufIdx].col, wmsg[n]);
    printf("warn: line:%d col:%d %s\n",
           symBuf[symBufIdx].lin, symBuf[symBufIdx].col, wmsg[n]);
}


void showPos() {
    fprintf(fout, "������ (line: %d, col: %d): %s\n", symBuf[symBufIdx].lin, symBuf[symBufIdx].col,
            symBuf[symBufIdx].token);
    printf("������ (line: %d, col: %d): %s\n", symBuf[symBufIdx].lin, symBuf[symBufIdx].col, symBuf[symBufIdx].token);
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
        case 7://δ֪�ĳ������ͱ�ʶ const def������
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon) {
                updateSymBuf();
            }
            showPos();
            break;
        case 8://δ֪�Ĳ�������
            printErr(n);
            while (symBuf[symBufIdx].id != comma && symBuf[symBufIdx].id != rparent) {
                updateSymBuf();
            }
            showPos();
            break;
        case 9://Ӧ�Ǳ�ʶ��
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
        case 10://Ӧ��(
            printErr(n);
            if (symBuf[symBufIdx].id == lbrace || symBuf[symBufIdx].id == lbrack)
                updateSymBuf();
            break;
        case 11://Ӧ��)
            printErr(n);
            if (symBuf[symBufIdx].id == rbrace || symBuf[symBufIdx].id == rbrack)
                updateSymBuf();
            break;
        case 12://Ӧ��{
            printErr(n);
            if (symBuf[symBufIdx].id == lparent || symBuf[symBufIdx].id == lbrack)
                updateSymBuf();
            break;
        case 13://Ӧ��}
            printErr(n);
            if (symBuf[symBufIdx].id == rparent || symBuf[symBufIdx].id == rbrack)
                updateSymBuf();
            break;
        case 14://Ӧ��]
            printErr(n);
            if (symBuf[symBufIdx].id == rbrace || symBuf[symBufIdx].id == rparent)
                updateSymBuf();
            break;
        case 15://Ӧ��:
            printErr(n);
            while (symBuf[symBufIdx].id != ifsy && symBuf[symBufIdx].id != whilesy && symBuf[symBufIdx].id != lbrace &&
                   symBuf[symBufIdx].id != scanfsy && symBuf[symBufIdx].id != printfsy &&
                   symBuf[symBufIdx].id != returnsy &&
                   symBuf[symBufIdx].id != switchsy && symBuf[symBufIdx].id != ident &&
                   symBuf[symBufIdx].id != semicolon) {//!first����
                updateSymBuf();
            }
            showPos();
            break;
        case 16://����δ����,call()�г���,���Ӻ����ֱ����
            printErr(n);//�����Ľ���λ��
            if (errPlace == 's') {//����е���
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
        case 17://��ʶ��δ���� ��ֵ��� read��� ����
            printErr(n);//��ֵ���Ľ���λ��
            if (errPlace == 'a') {//��ֵ����е��� assign
                while (symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
                showPos();
            }//else//�����е��� f //do nothing
            errPlace = 0;
            break;
        case 18://���������г�ʼֵ
        case 19://int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�
            printErr(n);//
            while (symBuf[symBufIdx].id != semicolon)
                updateSymBuf();
            showPos();
            break;
        case 20://����Ĵ�СӦΪ��0�޷�������
            printErr(n);//
            while (symBuf[symBufIdx].id != rbrack)
                updateSymBuf();
            showPos();
            break;
        case 21://�Ƿ������� numdef������, constdef c, factor f, onecase o
            printErr(n);//�����ʽ�������Ľ���λ��
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
        case 22://main������������ӦΪvoid
            printErr(n);//do nothing
            break;
        case 23://������main����
            printErr(n);//do nothing
            break;
        case 24://�Ƿ�����
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrack &&
                   symBuf[symBufIdx].id != rparent)
                updateSymBuf();
            showPos();
            break;
        case 25://�Ƿ����
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrace)
                updateSymBuf();
            showPos();
            break;
        case 26://switch������һ��case
        case 27://case��ǩ����
            printErr(n);//do nothing
            break;
        case 28://[]ֻ���ڷ�������
        case 29://ӦΪ�޷���ֵreturn
        case 30://ȱ��return���
        case 31://�������ò�����������ȷ
        case 32://���ܶ����麯���ͳ�����ֵ
            printErr(n);//do nothing
            break;
        case 33://���Ϸ��ı�������
            printErr(n);
            while (symBuf[symBufIdx].id != comma && symBuf[symBufIdx].id != semicolon)
                updateSymBuf();
            showPos();
            break;
        case 34://����Ӧ��void, int, ��char��ʼ
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