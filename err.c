#include "err.h"
#include "global.h"

//int err[ERRMAX][3];
char errPlace[30];
int ecnt = 0;//err cnt;
char *emsg[ERRMAX] = {
        "Դ�ļ�������",//0 updatesymBuf
        "�Ƿ��ַ�",//1 getsym
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
        "return����뺯�����岻һ��",//29
        "ȱ��return���",//30
        "�������ò�����������ȷ",//31
        "�������ò������Ͳ���ȷ",//32
        "���ܶ�����ֱ�Ӹ�ֵ"//33
        //todo ��ֵ������Ͳ�һ��
};


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
            break;
        case 8://δ֪�Ĳ�������
            printErr(n);
            while (symBuf[symBufIdx].id != comma && symBuf[symBufIdx].id != rparent) {
                updateSymBuf();
            }
            break;
        case 9://Ӧ�Ǳ�ʶ��
            printErr(n);
            if (strcmp(errPlace, "condef") == 0) {
                while (symBuf[symBufIdx].id != semicolon) {
                    updateSymBuf();
                }
            } else if (strcmp(errPlace, "vardef") == 0) {
                while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != comma) {
                    updateSymBuf();
                }
            } else if (strcmp(errPlace, "fundef") == 0) {
                while (symBuf[symBufIdx].id != lparent) {
                    updateSymBuf();
                }
            } else if (strcmp(errPlace, "paralist") == 0) {
                while (symBuf[symBufIdx].id != rparent && symBuf[symBufIdx].id != comma) {
                    updateSymBuf();
                }
            } else if (strcmp(errPlace, "read") == 0) {
                while (symBuf[symBufIdx].id != rparent && symBuf[symBufIdx].id != comma) {
                    updateSymBuf();
                }
            }
            errPlace[0] = 0;
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
            break;
        case 16://����δ����,call()�г���,���Ӻ����ֱ����
            printErr(n);//�����Ľ���λ��
            if (strcmp(errPlace, "0") == 0) {//����е���
                while (symBuf[symBufIdx].id != rbrace && symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
            } else {
                while (symBuf[symBufIdx].id != rbrace)
                    updateSymBuf();
                updateSymBuf();
            }
            errPlace[0] = 0;
            break;
        case 17://��ʶ��δ���� ��ֵ��� read��� ����
            printErr(n);//��ֵ���Ľ���λ��
            if (strcmp(errPlace, "assign") == 0) {//��ֵ����е���
                while (symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
            }//else//�����е���//do nothing
            errPlace[0] = 0;
            break;
        case 18://���������г�ʼֵ
        case 19://int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�
            printErr(n);//
            while (symBuf[symBufIdx].id != semicolon)
                updateSymBuf();
            break;
        case 20://����Ĵ�СӦΪ��0�޷�������
            printErr(n);//
            while (symBuf[symBufIdx].id != rbrack)
                updateSymBuf();
            break;
        case 21://�Ƿ������� numdef������, constdef 0, factor 1, onecase 2
            printErr(n);//�����ʽ�������Ľ���λ��
            if (strcmp(errPlace, "0") == 0) {
                while (symBuf[symBufIdx].id != semicolon)
                    updateSymBuf();
            } else if (strcmp(errPlace, "1") == 0) {
                while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrack &&
                       symBuf[symBufIdx].id != rparent)
                    updateSymBuf();
            } else {//one case
                while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrace)
                    updateSymBuf();
            }
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
            break;
        case 25://�Ƿ����
            printErr(n);
            while (symBuf[symBufIdx].id != semicolon && symBuf[symBufIdx].id != rbrace)
                updateSymBuf();
            break;
        case 26://switch������һ��case
        case 27://case��ǩ����
            printErr(n);//do nothing
            break;
        case 28://[]ֻ���ڷ�������
        case 29://return����뺯�����岻һ��
        case 30://ȱ��return���
        case 31://�������ò�����������ȷ
        case 32://�������ò������Ͳ���ȷ
        case 33://���ܶ�����ֱ�Ӹ�ֵ
            printErr(n);//do nothing
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
