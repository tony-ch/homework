#include "global.h"

void program(){
    if(sym==constsy){
        decConst();
    }

}
//const ��ͷ
void decConst(){
    if(sym!=constsy){//!���Ը�д�ķ�
        error(2);//Ӧ��const
        return;
    }
    getsym();
    constDef();
    if(sym!=semicolon){
        error(2);//Ӧ��;
        return;
    }
    getsym();
    while(sym==constsy){//! first����Ϊ{const}
        getsym();
        constDef();
        if(sym!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        getsym();
    }
    printf("this is dec of const.\n");
}

void constDef(){
    if(sym==intsy){
        getsym();
        if(sym!=ident){//!todo �ظ�����
            error(2);//Ӧ�Ǳ�ʶ��
            return;
        }
        getsym();
        if(sym!=become){
            error(2);//Ӧ��=
            return;
        }
        getsym();
        if(sym!=minus &&sym!=plus && sym!=unsignum && sym!=zero){
            error(2);//Ӧ������
            return;
        }
        numDef();
        while(sym==comma){//��ѡ��
            getsym();
            if(sym!=ident){
                error(2);//Ӧ�Ǳ�ʶ��
                return;
            }
            getsym();
            if(sym!=become){
                error(2);//Ӧ��=
                return;
            }
            getsym();
            if(sym!=minus &&sym!=plus && sym!=unsignum && sym!=zero){
                error(2);//Ӧ������
                return;
            }
            numDef();
        }
    }else if(sym==charsy){
        getsym();
        if(sym!=ident){//!todo �ظ�����
            error(2);//Ӧ�Ǳ�ʶ��
            return;
        }
        getsym();
        if(sym!=become){
            error(2);//Ӧ��=
            return;
        }
        getsym();
        if(sym!=charcon){
            error(2);//Ӧ������
            return;
        }
        getsym();
        while(sym==comma){//��ѡ��
            getsym();
            if(sym!=ident){
                error(2);//Ӧ�Ǳ�ʶ��
                return;
            }
            getsym();
            if(sym!=become){
                error(2);//Ӧ��=
                return;
            }
            getsym();
            if(sym!=charcon){
                error(2);//Ӧ������
                return;
            }
            getsym();
        }
    }else{
        error(2);//todp ӦΪint��char
        return;
    }
    printf("this is const def.\n");
}

void decVar(){
}

void varDef(){

}

void numDef(){//��������::= �ۣ������ݣ��޷�������������
    int flag=0;
    if(sym==zero){
        getsym();
    }else{
        if(sym==plus){//!��ѡ��
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
            error(2);//todo Ӧ��unsignnum
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

void valueParaList(){//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
    if(sym==rparent){//!��
        getsym();
    }else{
        expr();//!����һ��
        while(sym==comma){
            getsym();
            expr();
        }
    }
    printf("this is value para list.\n");
}

void complexStat(){
}

void stat(){//����䣾::= ��������䣾����ѭ����䣾| ��{��������У���}�������з���ֵ����������䣾; | ���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;|�������䣾����������䣾;
    if(sym==ifsy){//!ͷ���ż���
        ifStat();
    }else if(sym==whilesy){
        whileStat();
    }else if(sym==lbrace){//�����
        getsym();
        statList();
        if(sym!=rbrace){
            error(2);//Ӧ��}
            return;
        }
        getsym();
    }else if(sym==scanfsy){
        readStat();
        if(sym!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        getsym();
    }else if(sym==printfsy){
        writeStat();
        if(sym!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        getsym();
    }else if(sym==returnsy){
        retStat();
        if(sym!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        getsym();
    }else if(sym==switchsy){
        switchStat();
    }else if(sym==ident){//!first�����ཻ
        getsym();
        if(sym==become){//������ֵ����ֵ��䣾   ::=  ����ʶ�����������ʽ��
            getsym();
            expr();//!ֱ�ӵ���
            printf("this is a assignment.\n");
        }else if(sym==lbrack){//����Ԫ�ظ�ֵ
            getsym();//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
            expr();
            if(sym!=rbrack){
                error(2);//todo Ӧ��]
                return;
            }
            getsym();
            if(sym!=become){
                error(2);//todo Ӧ��=
                return;
            }
            getsym();
            expr();
            printf("this is a assignment.\n");
        }else if(sym==lparent){//��������
            getsym();//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
            valueParaList();//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
            if(sym!=rparent){
                error(2);//todo Ӧ��)
                return;
            }
            getsym();
            printf("this is a call stat.\n");
        }else{
            error(2);//todo  �Ƿ����
            return;
        }
        if(sym!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        getsym();
    }else{
        if(sym!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        getsym();
    }//!��
    printf("this is a stat.\n");
}

void statList(){
    //!����û��һ��
    while(sym==ifsy     ||  sym==whilesy    ||  sym==lbrace ||
          sym==scanfsy  ||  sym==printfsy   ||  sym==returnsy ||
          sym==switchsy ||  sym==ident      ||  sym==semicolon){//!first����
        stat();
    }
    printf("this is a stat list.\n");
}

void expr(){//�����ʽ��::=�ۣ������ݣ��{���ӷ�����������}
    if(sym==plus || sym==minus){//!��ѡ��
        getsym();
    }
    term();
    while(sym==plus || sym==minus){//!��+-�ָ�
        getsym();
        term();
    }
    printf("this is an expr.\n");
}

void term(){//���::=�����ӣ�{���˷�������������ӣ�}
    factor();
    while(sym==mul || sym==divd){
        getsym();
        factor();
    }
    printf("this is a term.\n");
}

void factor(){//�����ӣ�::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
    if(sym==ident){
        getsym();
        if(sym==lbrack){//!��ѡ�� ����ʶ������[�������ʽ����]��
            getsym();
            expr();
            if(sym!=rbrack){
                error(2);//todo Ӧ��]
                return;
            }
            getsym();
        }else if(sym==lparent){//���з���ֵ����������䣾
            getsym();//!��Ҫ�ж��Ƿ�Ϊ�з���ֵ
            valueParaList();
            if(sym!=rparent){
                error(2);//Ӧ��)
                return;
            }
            getsym();
            printf("this is a call stat.\n");
        }
    }else if(sym==charcon){
        getsym();
    }else if(sym==plus || sym==minus || sym==zero || sym==unsignum){//!���� first ����
        numDef();
    }else if(sym==lparent){//��(�������ʽ����)��
        getsym();
        expr();
        if(sym!=rparent){
            error(2);//todo Ӧ��])
            return;
        }
        getsym();
    }else{
        error(2);//todo �Ƿ�����
        return;
    }
    printf("this is a factor.\n");
}
/*
void assignment(){
}
*/
//if��ͷ
void ifStat(){//��������䣾::=if ��(������������)������䣾��else����䣾��
    if(sym!=ifsy){
        error(2);//Ӧ��if
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//Ӧ��(
        return;
    }
    getsym();
    condition();//!ֱ�ӵ���
    if(sym!=rparent){
        error(2);//Ӧ��)
        return;
    }
    stat();//!ֱ�ӵ���
    if(sym==elsesy){//!��ѡ��
        stat();
    }
    printf("this is an if stat.\n");
}

void condition(){//��������::=�����ʽ������ϵ������������ʽ���������ʽ��
    expr();//!ֱ�ӵ���
    if(sym==les){//!��ѡ��
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
//while��ͷ
void whileStat(){//��ѭ����䣾::=while ��(������������)������䣾
    if(sym!=whilesy){
        error(2);//todo Ӧ��while
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    condition();//!ֱ�ӵ���
    if(sym!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    getsym();
    stat();//!ֱ�ӵ���
    printf("this is a while stat.\n");
}
//printf��ͷ
void writeStat(){//��д��䣾::=printf��(�� ���ַ�����,�����ʽ�� ��)��|printf ��(�����ַ����� ��)��|printf ��(�������ʽ����)��
    if(sym!=printfsy){
        error(2);//todo Ӧ��printf
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    if(sym==strcon){
        getsym();
        if(sym==comma){//!��ѡ��
            getsym();
            expr();
        }
    }else{
        expr();//!ֱ�ӵ��� todo expr first����
    }
    if(sym!=rparent){
        error(2);//todo Ӧ��)
        if(sym==lbrace || sym==lbrack){
            getsym();//todo
        }
        return;
    }
    printf("this is a write stat.\n");
}
//scanf��ͷ
void readStat(){//������䣾::=scanf ��(������ʶ����{,����ʶ����}��)��
    if(sym!=scanfsy){
        error(2);//todo Ӧ��scanf
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//Ӧ��(
        return;
    }
    getsym();
    if(sym==ident){
        getsym();
        while(sym==comma){//!ѭ����,�ָ�
            getsym();
            if(sym!=ident){
                error(2);//todo Ӧ�Ǳ�ʶ��
                return;
            }
            getsym();
        }
        if(sym!=rparent){
            error(2);//todo Ӧ��)
            if(sym==lbrace || sym==lbrack){
                getsym();//todo
            }
            return;
        }
        getsym();
    }else{
        error(2);//todo ӦΪ��ʶ��
        return;
    }
    printf("this is a read stat.\n");
}
//switch��ͷ
void switchStat(){//�������䣾  ::=  switch ��(�������ʽ����)�� ��{����������ۣ�ȱʡ���ݡ�}��
    if(sym!=switchsy){
        error(2);//todo Ӧ��switch
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    expr();//!ֱ�ӵ���
    if(sym!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    getsym();
    if(sym!=lbrace){
        error(2);//todo Ӧ��switch
        return;
    }
    getsym();
    caseStat();//!ֱ�ӵ���
    if(sym==defaultsy){//!first����Ϊ{default}
        defaultCase();//!��ѡ��
    }
    if(sym!=rbrace){
        error(2);//todo Ӧ��}
        return;
    }
    getsym();
    printf("this is a switch stat.\n");
}
//case��ͷ
void caseStat(){//�������   ::=  ���������䣾{���������䣾}
    if(sym!=casesy){//!����һ��case���
        error(2);//todo Ӧ��case
        return;
    }
    while(sym==casesy){//! first����Ϊ{case}
        oneCase();
    }
    printf("this is a case table.\n");
}
//case��ͷ
void oneCase(){//���������䣾::=case��������������䣾
    if(sym!=casesy){
        error(2);//todo Ӧ��case
        return;
    }
    getsym();
    if(sym==charcon){
        getsym();
    }else {
        numDef();//!ֱ�ӵ���
    }
    if(sym!=colon){
        error(2);//todo Ӧ��:
        return;
    }
    getsym();
    stat();//!ֱ�ӵ���
    printf("this is a one case.\n");
}
//default��ͷ
void defaultCase(){//��ȱʡ��::=default : ����䣾
    if(sym!=defaultsy){
        error(2);//todo Ӧ��default
        return;
    }
    getsym();
    if(sym!=colon){
        error(2);//todo Ӧ��:
        return;
    }
    getsym();
    stat();//!ֱ�ӵ���
    printf("this is a default case.\n");
}
//return��ͷ
void retStat(){//��������䣾::=return[��(�������ʽ����)��]
    if(sym!=returnsy){
        error(2);//todo Ӧ��return
        return;
    }
    getsym();
    if(sym==lparent){//!��ѡ��
        getsym();
        expr();//!ֱ�ӵ���
        if(sym==rparent){
            getsym();
        }else{
            error(2);//todo Ӧ��)
            if(sym==rbrack || sym==rbrace){//todo
                getsym();
            }
            return;
        }
    }
    printf("this is a return stat.\n");
}
