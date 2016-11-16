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
    if(symBuf[symBufIdx].id==constsy){//!ͷ����Ϊconst
        decConst();
    }
    int hasVarDef=0;
    while((symBuf[symBufIdx].id==intsy || symBuf[symBufIdx].id==charsy)&&symBuf[(symBufIdx+2)%3].id!=lparent){
        hasVarDef=1;
        varDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ�Ƿֺ�
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
//const ��ͷ
void decConst(){
    if(symBuf[symBufIdx].id!=constsy){//!���Ը�д�ķ�
        error(2);//Ӧ��const
        return;
    }
    updateSymBuf();
    constDef();
    if(symBuf[symBufIdx].id!=semicolon){
        error(2);//Ӧ��;
        return;
    }
    updateSymBuf();
    while(symBuf[symBufIdx].id==constsy){//! first����Ϊ{const}
        updateSymBuf();
        constDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        updateSymBuf();
    }
    fprintf(fout,"\t\tthis is dec of const.\n");
}

void constDef(){
    if(symBuf[symBufIdx].id==intsy){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo �ظ�����
            error(2);//Ӧ�Ǳ�ʶ��
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(2);//Ӧ��=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){//!first����
            error(2);//Ӧ������
            return;
        }
        numDef();
        while(symBuf[symBufIdx].id==comma){//��ѡ��
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//Ӧ�Ǳ�ʶ��
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(2);//Ӧ��=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){
                error(2);//Ӧ������
                return;
            }
            numDef();
        }
    }else if(symBuf[symBufIdx].id==charsy){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo �ظ�����
            error(2);//Ӧ�Ǳ�ʶ��
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(2);//Ӧ��=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=charcon){
            error(2);//Ӧ������
            return;
        }
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//��ѡ��
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//Ӧ�Ǳ�ʶ��
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(2);//Ӧ��=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=charcon){
                error(2);//Ӧ������
                return;
            }
            updateSymBuf();
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
    if(symBuf[symBufIdx].id!=intsy && symBuf[symBufIdx].id!=charsy){
        error(2);//todo ӦΪint��char
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(2);//todo ӦΪ��ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){//!��ѡ��
        updateSymBuf();
        if(symBuf[symBufIdx].id!=unsignum){
            error(2);//todo ӦΪ�޷�������
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
        if(symBuf[symBufIdx].id!=ident){//!��Ӧ�����ͱ�ʶ��
            error(2);//todo ӦΪ��ʶ��
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id==lbrack){//!��ѡ��
            updateSymBuf();
            if(symBuf[symBufIdx].id!=unsignum){
                error(2);//todo ӦΪ�޷�������
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

void numDef(){//��������::= �ۣ������ݣ��޷�������������
    int flag=0;
    if(symBuf[symBufIdx].id==zero){
        updateSymBuf();
    }else{
        if(symBuf[symBufIdx].id==plus){//!��ѡ��
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
            error(2);//todo Ӧ��unsignnum
            return;
        }
    }
    if(flag){
        fprintf(fout,"\t\tthis is a num.\n");
    }else{
        fprintf(fout,"\t\tthis is a num.\n");
    }
}

void retFuncDef(){//���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
    if(symBuf[symBufIdx].id!=charsy&&symBuf[symBufIdx].id!=intsy){
        error(2);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(2);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    paraList();
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
    if(symBuf[symBufIdx].id!=voidsy){
        error(2);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(2);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    paraList();
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

void paraList(){//�����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}|���գ�
    if(symBuf[symBufIdx].id==charsy||symBuf[symBufIdx].id==intsy){//!����Ϊ��
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

void mainDef(){//����������    ::= void main��(����)�� ��{����������䣾��}��
    if(symBuf[symBufIdx].id!=voidsy){
        error(2);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=mainsy){
        error(2);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    complexStat();
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo Ӧ��(
        return;
    }//!!todo
    fprintf(fout,"\t\tthis is main func dec.\n");
}

/*void call(){
}
*/

void valueParaList(){//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
    if(symBuf[symBufIdx].id==rparent){//!��
        //getsym();//!
    }else{
        expr();//!����һ��
        while(symBuf[symBufIdx].id==comma){
            updateSymBuf();
            expr();
        }
    }
    fprintf(fout,"\t\tthis is value para list.\n");
}

void complexStat(){//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
    if(symBuf[symBufIdx].id==constsy){//!ͷ����Ϊconst
        decConst();
    }
    int hasVarDef=0;
    while(symBuf[symBufIdx].id==charsy || symBuf[symBufIdx].id==intsy){
        varDef();
        hasVarDef=1;
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
        }
        updateSymBuf();
    }
    if(hasVarDef){
        fprintf(fout,"\t\tthis is dec of var.\n");
    }
    statList();
    fprintf(fout,"\t\tthis is complex stat.\n");
}

void stat(){//����䣾::= ��������䣾����ѭ����䣾| ��{��������У���}�������з���ֵ����������䣾; | ���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;|�������䣾����������䣾;
    if(symBuf[symBufIdx].id==ifsy){//!ͷ���ż���
        ifStat();
    }else if(symBuf[symBufIdx].id==whilesy){
        whileStat();
    }else if(symBuf[symBufIdx].id==lbrace){//�����
        updateSymBuf();
        statList();
        if(symBuf[symBufIdx].id!=rbrace){
            error(2);//Ӧ��}
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==scanfsy){
        readStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==printfsy){
        writeStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==returnsy){
        retStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==switchsy){
        switchStat();
    }else if(symBuf[symBufIdx].id==ident){//!first�����ཻ
        updateSymBuf();
        if(symBuf[symBufIdx].id==become){//������ֵ����ֵ��䣾   ::=  ����ʶ�����������ʽ��
            updateSymBuf();
            expr();//!ֱ�ӵ���
            fprintf(fout,"\t\tthis is a assignment.\n");
        }else if(symBuf[symBufIdx].id==lbrack){//����Ԫ�ظ�ֵ
            updateSymBuf();//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
            expr();
            if(symBuf[symBufIdx].id!=rbrack){
                error(2);//todo Ӧ��]
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(2);//todo Ӧ��=
                return;
            }
            updateSymBuf();
            expr();
            fprintf(fout,"\t\tthis is a assignment.\n");
        }else if(symBuf[symBufIdx].id==lparent){//��������
            updateSymBuf();//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
            valueParaList();//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
            if(symBuf[symBufIdx].id!=rparent){
                error(2);//todo Ӧ��)
                return;
            }
            updateSymBuf();
            fprintf(fout,"\t\tthis is a call stat.\n");
        }else{
            error(2);//todo  �Ƿ����
            return;
        }
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        updateSymBuf();
    }else{
        if(symBuf[symBufIdx].id!=semicolon){
            error(2);//Ӧ��;
            return;
        }
        updateSymBuf();
    }//!��
    fprintf(fout,"\t\tthis is a stat.\n");
}

void statList(){
    //!����û��һ��
    while(symBuf[symBufIdx].id==ifsy     ||  symBuf[symBufIdx].id==whilesy    ||  symBuf[symBufIdx].id==lbrace ||
          symBuf[symBufIdx].id==scanfsy  ||  symBuf[symBufIdx].id==printfsy   ||  symBuf[symBufIdx].id==returnsy ||
          symBuf[symBufIdx].id==switchsy ||  symBuf[symBufIdx].id==ident      ||  symBuf[symBufIdx].id==semicolon){//!first����
        stat();
    }
    fprintf(fout,"\t\tthis is a stat list.\n");
}

void expr(){//�����ʽ��::=�ۣ������ݣ��{���ӷ�����������}
    if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus){//!��ѡ��
        updateSymBuf();
    }
    term();
    while(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus){//!��+-�ָ�
        updateSymBuf();
        term();
    }
    fprintf(fout,"\t\tthis is an expr.\n");
}

void term(){//���::=�����ӣ�{���˷�������������ӣ�}
    factor();
    while(symBuf[symBufIdx].id==mul || symBuf[symBufIdx].id==divd){
        updateSymBuf();
        factor();
    }
    fprintf(fout,"\t\tthis is a term.\n");
}

void factor(){//�����ӣ�::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
    if(symBuf[symBufIdx].id==ident){
        updateSymBuf();
        if(symBuf[symBufIdx].id==lbrack){//!��ѡ�� ����ʶ������[�������ʽ����]��
            updateSymBuf();
            expr();
            if(symBuf[symBufIdx].id!=rbrack){
                error(2);//todo Ӧ��]
                return;
            }
            updateSymBuf();
        }else if(symBuf[symBufIdx].id==lparent){//���з���ֵ����������䣾
            updateSymBuf();//!��Ҫ�ж��Ƿ�Ϊ�з���ֵ
            valueParaList();
            if(symBuf[symBufIdx].id!=rparent){
                error(2);//Ӧ��)
                return;
            }
            updateSymBuf();
            fprintf(fout,"\t\tthis is a call stat.\n");
        }
    }else if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus || symBuf[symBufIdx].id==zero || symBuf[symBufIdx].id==unsignum){//!���� first ����
        numDef();
    }else if(symBuf[symBufIdx].id==lparent){//��(�������ʽ����)��
        updateSymBuf();
        expr();
        if(symBuf[symBufIdx].id!=rparent){
            error(2);//todo Ӧ��])
            return;
        }
        updateSymBuf();
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
    if(symBuf[symBufIdx].id!=ifsy){
        error(2);//Ӧ��if
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//Ӧ��(
        return;
    }
    updateSymBuf();
    condition();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//Ӧ��)
        return;
    }
	updateSymBuf();//!read one more sym
    stat();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id==elsesy){//!��ѡ��
		updateSymBuf();//!one more sym
        stat();
    }
    fprintf(fout,"\t\tthis is an if stat.\n");
}

void condition(){//��������::=�����ʽ������ϵ������������ʽ���������ʽ��
    expr();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id==les){//!��ѡ��
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
    }else if(symBuf[symBufIdx].id==neq){//!©д
        updateSymBuf();
        expr();
    }
    fprintf(fout,"\t\tthis is a condition.\n");
}
//while��ͷ
void whileStat(){//��ѭ����䣾::=while ��(������������)������䣾
    if(symBuf[symBufIdx].id!=whilesy){
        error(2);//todo Ӧ��while
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    condition();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    fprintf(fout,"\t\tthis is a while stat.\n");
}
//printf��ͷ
void writeStat(){//��д��䣾::=printf��(�� ���ַ�����,�����ʽ�� ��)��|printf ��(�����ַ����� ��)��|printf ��(�������ʽ����)��
    if(symBuf[symBufIdx].id!=printfsy){
        error(2);//todo Ӧ��printf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==strcon){
        updateSymBuf();
        if(symBuf[symBufIdx].id==comma){//!��ѡ��
            updateSymBuf();
            expr();
        }
    }else{
        expr();//!ֱ�ӵ��� todo expr first����
    }
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo Ӧ��)
        if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack){
            updateSymBuf();//todo
        }
        return;
    }
	updateSymBuf();//!one more sym
    fprintf(fout,"\t\tthis is a write stat.\n");
}
//scanf��ͷ
void readStat(){//������䣾::=scanf ��(������ʶ����{,����ʶ����}��)��
    if(symBuf[symBufIdx].id!=scanfsy){
        error(2);//todo Ӧ��scanf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//Ӧ��(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==ident){
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//!ѭ����,�ָ�
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(2);//todo Ӧ�Ǳ�ʶ��
                return;
            }
            updateSymBuf();
        }
        if(symBuf[symBufIdx].id!=rparent){
            error(2);//todo Ӧ��)
            if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack){
                updateSymBuf();//todo
            }
            return;
        }
        updateSymBuf();
    }else{
        error(2);//todo ӦΪ��ʶ��
        return;
    }
    fprintf(fout,"\t\tthis is a read stat.\n");
}
//switch��ͷ
void switchStat(){//�������䣾  ::=  switch ��(�������ʽ����)�� ��{����������ۣ�ȱʡ���ݡ�}��
    if(symBuf[symBufIdx].id!=switchsy){
        error(2);//todo Ӧ��switch
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(2);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    expr();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id!=rparent){
        error(2);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(2);//todo Ӧ��switch
        return;
    }
    updateSymBuf();
    caseStat();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id==defaultsy){//!first����Ϊ{default}
        defaultCase();//!��ѡ��
    }
    if(symBuf[symBufIdx].id!=rbrace){
        error(2);//todo Ӧ��}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a switch stat.\n");
}
//case��ͷ
void caseStat(){//�������   ::=  ���������䣾{���������䣾}
    if(symBuf[symBufIdx].id!=casesy){//!����һ��case���
        error(2);//todo Ӧ��case
        return;
    }
    while(symBuf[symBufIdx].id==casesy){//! first����Ϊ{case}
        oneCase();
    }
    fprintf(fout,"\t\tthis is a case table.\n");
}
//case��ͷ
void oneCase(){//���������䣾::=case��������������䣾
    if(symBuf[symBufIdx].id!=casesy){
        error(2);//todo Ӧ��case
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else {
        numDef();//!ֱ�ӵ���
    }
    if(symBuf[symBufIdx].id!=colon){
        error(2);//todo Ӧ��:
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    fprintf(fout,"\t\tthis is a one case.\n");
}
//default��ͷ
void defaultCase(){//��ȱʡ��::=default : ����䣾
    if(symBuf[symBufIdx].id!=defaultsy){
        error(2);//todo Ӧ��default
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=colon){
        error(2);//todo Ӧ��:
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    fprintf(fout,"\t\tthis is a default case.\n");
}
//return��ͷ
void retStat(){//��������䣾::=return[��(�������ʽ����)��]
    if(symBuf[symBufIdx].id!=returnsy){
        error(2);//todo Ӧ��return
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lparent){//!��ѡ��
        updateSymBuf();
        expr();//!ֱ�ӵ���
        if(symBuf[symBufIdx].id==rparent){
            updateSymBuf();
        }else{
            error(2);//todo Ӧ��)
            if(symBuf[symBufIdx].id==rbrack || symBuf[symBufIdx].id==rbrace){//todo
                updateSymBuf();
            }
            return;
        }
    }
    fprintf(fout,"\t\tthis is a return stat.\n");
}
