#include "global.h"
int tempBufSel;
int tempBufIdx;
int tempLcnt;
int tempLidx;
char tempCh;
enum symbol tempSym;
int tempNum;
char tempToken[STRMAX];
void program(){
    if(sym==constsy){//!ͷ����Ϊconst
        decConst();
    }

    while(sym==intsy || sym==charsy){
        saveState();//!in while body
        getsym();
        if(sym!=ident){
            error(2);//todoӦ�Ǳ�ʶ��
            return;
        }
        getsym();
        if(sym==lparent){//!�Ǻ�������
            recState();
            break;
        }else{
            recState();
            varDef();
            if(sym!=semicolon){
                error(2);//Ӧ�Ƿֺ�
                return;
            }
            getsym();//!read one more sym
        }
    }
    while(1){
        saveState();//!in while body
        if(sym==voidsy){
            getsym();
            if(sym==mainsy){//!��main����
                recState();
                break;
            }else{
                recState();
                voidFuncDef();
            }
        }else if(sym==charsy || sym==intsy){//!ͷ����Ϊ{char int}
            retFuncDef();
        }else{
            error(2);//todo
            return;
        }
    }
    mainDef();
    fprintf(fout,"\t\tthis is a program.\n");
}

void saveState(){
    fprintf(fout,"\t\tsave stat.\n");
    /*fprintf(fout,"\t\tbufsel:%d\n",bufsel);
    fprintf(fout,"\t\tbufidx:%d\n",bufidx);
    fprintf(fout,"\t\tlcnt:%d\n",lcnt);
    fprintf(fout,"\t\tlidx:%d\n",lidx);
    fprintf(fout,"\t\tch:%c\n",ch);
    fprintf(fout,"\t\tsym:%s\n",symbolstr[sym]);*/
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
    /*fprintf(fout,"\t\tbufsel:%d\n",bufsel);
    fprintf(fout,"\t\tbufidx:%d\n",bufidx);
    fprintf(fout,"\t\tlcnt:%d\n",lcnt);
    fprintf(fout,"\t\tlidx:%d\n",lidx);
    fprintf(fout,"\t\tch:%c\n",ch);
    fprintf(fout,"\t\tsym:%s\n",symbolstr[sym]);*/
    bufsel=tempBufSel;
    bufidx=tempBufIdx;
    lcnt=tempLcnt;
    lidx=tempLidx;
    ch=tempCh;
    sym=tempSym;
    num=tempNum;
    strcpy(token,tempToken);
    /*fprintf(fout,"\t\tafter rec.\n");
    fprintf(fout,"\t\tbufsel:%d\n",bufsel);
    fprintf(fout,"\t\tbufidx:%d\n",bufidx);
    fprintf(fout,"\t\tlcnt:%d\n",lcnt);
    fprintf(fout,"\t\tlidx:%d\n",lidx);
    fprintf(fout,"\t\tch:%c\n",ch);
    fprintf(fout,"\t\tsym:%s\n",symbolstr[sym]);*/
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
    fprintf(fout,"\t\tthis is dec of const.\n");
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
        if(sym!=minus &&sym!=plus && sym!=unsignum && sym!=zero){//!first����
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
    fprintf(fout,"\t\tthis is const def.\n");
}
/*
void decVar(){//������˵����::=���������壾;{���������壾;}

    while(sym==intsy || sym==charsy){
        getsym();
        if(sym!=ident){
            error(2);//todo ӦΪ��ʶ��
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
                error(2);//todo Ӧ�Ƿֺ�
                return;
            }
            getsym();
        }
    }
}
*/
void varDef(){
    //enum symbol typ=sym;
    if(sym!=intsy && sym!=charsy){
        error(2);//todo ӦΪint��char
        return;
    }
    getsym();
    if(sym!=ident){
        error(2);//todo ӦΪ��ʶ��
        return;
    }
    getsym();
    if(sym==lbrack){//!��ѡ��
        getsym();
        if(sym!=unsignum){
            error(2);//todo ӦΪ�޷�������
            return;
        }
        getsym();
        if(sym!=rbrack){
            error(2);//todo )
            return;
        }
        getsym();
    }
    while(sym==comma){
        getsym();
        if(sym!=ident){//!��Ӧ�����ͱ�ʶ��
            error(2);//todo ӦΪ��ʶ��
            return;
        }
        getsym();
        if(sym==lbrack){//!��ѡ��
            getsym();
            if(sym!=unsignum){
                error(2);//todo ӦΪ�޷�������
                return;
            }
            getsym();
            if(sym!=rbrack){
                error(2);//todo )
                return;
            }
            getsym();
        }
    }
    fprintf(fout,"\t\tthis is var def.\n");
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
    fprintf(fout,"\t\tthis is a num:%d\n",num);
}

void retFuncDef(){//���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
    if(sym!=charsy&&sym!=intsy){
        error(2);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    getsym();
    if(sym!=ident){
        error(2);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    paraList();
    if(sym!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    getsym();
    if(sym!=lbrace){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    complexStat();
    if(sym!=rbrace){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
    if(sym!=voidsy){
        error(2);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    getsym();
    if(sym!=ident){
        error(2);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    paraList();
    if(sym!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    getsym();
    if(sym!=lbrace){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    complexStat();
    if(sym!=rbrace){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

void paraList(){//�����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}|���գ�
    if(sym==charsy||sym==intsy){//!����Ϊ��
        getsym();
        if(sym!=ident){
            error(2);//todo
            return;
        }
        getsym();
        while(sym==comma){
            getsym();
            if(sym!=charsy && sym!=intsy){
                error(2);//todo
                return;
            }
            getsym();
            if(sym!=ident){
                error(2);//todo
                return;
            }
            getsym();
        }
    }
    fprintf(fout,"\t\tthis is para list.\n");
}

void mainDef(){//����������    ::= void main��(����)�� ��{����������䣾��}��
    if(sym!=voidsy){
        error(2);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    getsym();
    if(sym!=mainsy){
        error(2);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    getsym();
    if(sym!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    if(sym!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    getsym();
    if(sym!=lbrace){
        error(2);//todo Ӧ��(
        return;
    }
    getsym();
    complexStat();
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[sym],token);
    if(sym!=rbrace){
        error(2);//todo Ӧ��(
        return;
    }//!!todo
    fprintf(fout,"\t\tthis is main func dec.\n");
}

/*void call(){
}
*/

void valueParaList(){//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
    if(sym==rparent){//!��
        //getsym();//!
    }else{
        expr();//!����һ��
        while(sym==comma){
            getsym();
            expr();
        }
    }
    fprintf(fout,"\t\tthis is value para list.\n");
}

void complexStat(){//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
    if(sym==constsy){//!ͷ����Ϊconst
        decConst();
    }
    while(sym==charsy || sym==intsy){
        varDef();
        if(sym!=semicolon){
            error(2);//Ӧ��;
        }
        getsym();
    }
    statList();
    fprintf(fout,"\t\tthis is complex stat.\n");
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
            fprintf(fout,"\t\tthis is a assignment.\n");
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
            fprintf(fout,"\t\tthis is a assignment.\n");
        }else if(sym==lparent){//��������
            getsym();//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
            valueParaList();//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
            if(sym!=rparent){
                error(2);//todo Ӧ��)
                return;
            }
            getsym();
            fprintf(fout,"\t\tthis is a call stat.\n");
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
    fprintf(fout,"\t\tthis is a stat.\n");
}

void statList(){
    //!����û��һ��
    while(sym==ifsy     ||  sym==whilesy    ||  sym==lbrace ||
          sym==scanfsy  ||  sym==printfsy   ||  sym==returnsy ||
          sym==switchsy ||  sym==ident      ||  sym==semicolon){//!first����
        stat();
    }
    fprintf(fout,"\t\tthis is a stat list.\n");
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
    fprintf(fout,"\t\tthis is an expr.\n");
}

void term(){//���::=�����ӣ�{���˷�������������ӣ�}
    factor();
    while(sym==mul || sym==divd){
        getsym();
        factor();
    }
    fprintf(fout,"\t\tthis is a term.\n");
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
            fprintf(fout,"\t\tthis is a call stat.\n");
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
    fprintf(fout,"\t\tthis is a factor.\n");
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
	getsym();//!read one more sym
    stat();//!ֱ�ӵ���
    if(sym==elsesy){//!��ѡ��
		getsym();//!one more sym
        stat();
    }
    fprintf(fout,"\t\tthis is an if stat.\n");
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
    }else if(sym==neq){//!©д
        getsym();
        expr();
    }
    fprintf(fout,"\t\tthis is a condition.\n");
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
    fprintf(fout,"\t\tthis is a while stat.\n");
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
	getsym();//!one more sym
    fprintf(fout,"\t\tthis is a write stat.\n");
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
    fprintf(fout,"\t\tthis is a read stat.\n");
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
    fprintf(fout,"\t\tthis is a switch stat.\n");
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
    fprintf(fout,"\t\tthis is a case table.\n");
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
    fprintf(fout,"\t\tthis is a one case.\n");
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
    fprintf(fout,"\t\tthis is a default case.\n");
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
    fprintf(fout,"\t\tthis is a return stat.\n");
}
