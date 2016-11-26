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
    if(symBuf[symBufIdx].id==constsy){//!ͷ����Ϊconst
        decConst();
    }
    int hasVarDef=0;
    while((symBuf[symBufIdx].id==intsy || symBuf[symBufIdx].id==charsy)&&symBuf[(symBufIdx+2)%3].id!=lparent){
        hasVarDef=1;
        varDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!Ӧ�Ƿֺ�(F)
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
//const ��ͷ
void decConst(){
    if(symBuf[symBufIdx].id!=constsy){//!���Ը�д�ķ�
        error(5);//Ӧ��const
        return;
    }
    updateSymBuf();
    constDef();
    if(symBuf[symBufIdx].id!=semicolon){
        error(4);//!Ӧ��;(F)
    }else{
        updateSymBuf();
    }
    while(symBuf[symBufIdx].id==constsy){//! first����Ϊ{const}
        updateSymBuf();
        constDef();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!Ӧ��;(F)
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
        if(symBuf[symBufIdx].id!=ident){//!todo �ظ�����
            error(5);//Ӧ�Ǳ�ʶ��
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(5);//Ӧ��=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){//!first����
            error(5);//Ӧ������
            return;
        }
        value=numDef();
        enter(name,kind,type,value);
        while(symBuf[symBufIdx].id==comma){//��ѡ��
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//Ӧ�Ǳ�ʶ��
                return;
            }
            strcpy(name,symBuf[symBufIdx].token);
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(5);//Ӧ��=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=minus &&symBuf[symBufIdx].id!=plus && symBuf[symBufIdx].id!=unsignum && symBuf[symBufIdx].id!=zero){
                error(5);//Ӧ������
                return;
            }
            value=numDef();
            enter(name,kind,type,value);
        }
    }else if(symBuf[symBufIdx].id==charsy){
        type=chtyp;
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!todo �ظ�����
            error(5);//Ӧ�Ǳ�ʶ��
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(5);//Ӧ��=
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=charcon){
            error(5);//Ӧ���ַ�
            return;
        }
        value=symBuf[symBufIdx].token[1];
        enter(name,kind,type,value);
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//��ѡ��
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//Ӧ�Ǳ�ʶ��
                return;
            }
            strcpy(name,symBuf[symBufIdx].token);
            updateSymBuf();
            if(symBuf[symBufIdx].id!=become){
                error(5);//Ӧ��=
                return;
            }
            updateSymBuf();
            if(symBuf[symBufIdx].id!=charcon){
                error(5);//Ӧ���ַ�
                return;
            }
            value=symBuf[symBufIdx].token[1];
            enter(name,kind,type,value);
            updateSymBuf();
        }
    }else{
        error(5);//todp ӦΪint��char
        return;
    }
    fprintf(fout,"\t\tthis is const def.\n");
}
/*
void decVar(){//������˵����::=���������壾;{���������壾;}
}
*/
void varDef(){//���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,����ʶ����|����ʶ������[�����޷�����������]�� }
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
        error(5);//todo ӦΪint��char
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo ӦΪ��ʶ��
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    kind=varkind;
    value=0;
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){//!��ѡ��
        updateSymBuf();
        if(symBuf[symBufIdx].id!=unsignum){
            error(5);//todo ӦΪ�޷�������
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
        if(symBuf[symBufIdx].id!=ident){//!��Ӧ�����ͱ�ʶ��
            error(5);//todo ӦΪ��ʶ��
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        kind=varkind;
        value=0;
        updateSymBuf();
        if(symBuf[symBufIdx].id==lbrack){//!��ѡ��
            updateSymBuf();
            if(symBuf[symBufIdx].id!=unsignum){
                error(5);//todo ӦΪ�޷�������
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

int numDef(){//��������::= �ۣ������ݣ��޷�������������
    int flag=0;
    int num=0;
    if(symBuf[symBufIdx].id==zero){
        updateSymBuf();
        num=0;
    }else{
        if(symBuf[symBufIdx].id==plus){//!��ѡ��
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
            error(5);//todo Ӧ��unsignnum
            return 0;
        }
    }
    fprintf(fout,"\t\tthis is a num:%d.\n",num);
    return num;
}

void retFuncDef(){//���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
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
        error(5);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    enter(name,kind,type,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tabIdxToEnter].value=value;
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tabIdxToEnter].name,tab[tabIdxToEnter].value);
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo Ӧ��{
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo Ӧ��}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
    char name[ALENMAX];
    enum kinds kind;
    enum types type;
    int value;
    int tabIdxToEnter;
    kind=funkind;
    tabIdxToEnter=tidx;
    if(symBuf[symBufIdx].id!=voidsy){
        error(5);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    type=voidtyp;
    updateSymBuf();
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    enter(name,kind,type,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tabIdxToEnter].value=value;
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tabIdxToEnter].name,tab[tabIdxToEnter].value);
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo Ӧ��{
        return;
    }
    updateSymBuf();
    complexStat();
    if(symBuf[symBufIdx].id!=rbrace) {
        error(5);//todo Ӧ��}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

int paraList(){//�����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}|���գ�
    char name[ALENMAX];
    enum kinds kind;
    enum types type;
    kind=parakind;
    int paraCnt=0;
    if(symBuf[symBufIdx].id==charsy||symBuf[symBufIdx].id==intsy){//!����Ϊ��
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

void mainDef(){//����������    ::= void main��(����)�� ��{����������䣾��}��
    char name[ALENMAX];
    enum kinds kind;
    enum types type;
    kind=funkind;
    if(symBuf[symBufIdx].id!=voidsy){
        error(5);//todo Ӧ�����ͱ�ʶ��
        return;
    }
    type=voidtyp;
    updateSymBuf();
    if(symBuf[symBufIdx].id!=mainsy){
        error(5);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    strcpy(name,symBuf[symBufIdx].token);
    enter(name,kind,type,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    complexStat();
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo Ӧ��(
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
    updateSymBuf();//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
    valueParaList();//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a call stat.\n");
}


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
            error(4);//!Ӧ��;(F)
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

void stat(){//����䣾::= ��������䣾����ѭ����䣾| ��{��������У���}�������з���ֵ����������䣾; | ���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;|�������䣾����������䣾;
    if(symBuf[symBufIdx].id==ifsy){//!ͷ���ż���
        ifStat();
    }else if(symBuf[symBufIdx].id==whilesy){
        whileStat();
    }else if(symBuf[symBufIdx].id==lbrace){//�����
        updateSymBuf();
        statList();
        if(symBuf[symBufIdx].id!=rbrace){
            error(5);//Ӧ��}
            return;
        }
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==scanfsy){
        readStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!Ӧ��;(F)
        }else{
            updateSymBuf();
        }
    }else if(symBuf[symBufIdx].id==printfsy){
        writeStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!Ӧ��;(F)
        }else{
            updateSymBuf();
        }
    }else if(symBuf[symBufIdx].id==returnsy){
        retStat();
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!Ӧ��;(F)
        }else{
            updateSymBuf();
        }
    }else if(symBuf[symBufIdx].id==switchsy){
        switchStat();
    }else if(symBuf[symBufIdx].id==ident){//!first�����ཻ
        enum symbol nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==become || nextSym==lbrack){
            assignment();
        }else if(nextSym==lparent){
            call();
        }else{
            error(5);//�Ƿ����
            return;
        }
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!Ӧ��;(F)
        }else{
            updateSymBuf();
        }
    }else{
        if(symBuf[symBufIdx].id!=semicolon){
            error(4);//!!Ӧ��;(F)
        }else{
            updateSymBuf();
        }
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
        enum symbol nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==lparent){
            call();
        }else{
            updateSymBuf();
            if(symBuf[symBufIdx].id==lbrack){//!��ѡ�� ����ʶ������[�������ʽ����]��
                updateSymBuf();
                expr();
                if(symBuf[symBufIdx].id!=rbrack){
                    error(5);//todo Ӧ��]
                    return;
                }
                updateSymBuf();
            }
        }
    }else if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus || symBuf[symBufIdx].id==zero || symBuf[symBufIdx].id==unsignum){//!���� first ����
        numDef();
    }else if(symBuf[symBufIdx].id==lparent){//��(�������ʽ����)��
        updateSymBuf();
        expr();
        if(symBuf[symBufIdx].id!=rparent){
            error(5);//todo Ӧ��])
            return;
        }
        updateSymBuf();
    }else{
        error(5);//todo �Ƿ�����
        return;
    }
    fprintf(fout,"\t\tthis is a factor.\n");
}

void assignment(){//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){
        updateSymBuf();//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
        expr();
        if(symBuf[symBufIdx].id!=rbrack){
            error(5);//todo Ӧ��]
            return;
        }
        updateSymBuf();
        if(symBuf[symBufIdx].id!=become){
            error(5);//todo Ӧ��=
            return;
        }
        updateSymBuf();
        expr();
    }else if(symBuf[symBufIdx].id==become){//������ֵ����ֵ��䣾   ::=  ����ʶ�����������ʽ��
        updateSymBuf();
        expr();//!ֱ�ӵ���
    }else{
        error(5);//todo �Ƿ����
        return;
    }
    fprintf(fout,"\t\tthis is a assignment.\n");
}
//if��ͷ
void ifStat(){//��������䣾::=if ��(������������)������䣾��else����䣾��
    if(symBuf[symBufIdx].id!=ifsy){
        error(5);//Ӧ��if
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//Ӧ��(
        return;
    }
    updateSymBuf();
    condition();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//Ӧ��)
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
        error(5);//todo Ӧ��while
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    condition();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    fprintf(fout,"\t\tthis is a while stat.\n");
}
//printf��ͷ
void writeStat(){//��д��䣾::=printf��(�� ���ַ�����,�����ʽ�� ��)��|printf ��(�����ַ����� ��)��|printf ��(�������ʽ����)��
    if(symBuf[symBufIdx].id!=printfsy){
        error(5);//todo Ӧ��printf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
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
        error(5);//todo Ӧ��)
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
        error(5);//todo Ӧ��scanf
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//Ӧ��(
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==ident){
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//!ѭ����,�ָ�
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//todo Ӧ�Ǳ�ʶ��
                return;
            }
            updateSymBuf();
        }
        if(symBuf[symBufIdx].id!=rparent){
            error(5);//todo Ӧ��)
            if(symBuf[symBufIdx].id==lbrace || symBuf[symBufIdx].id==lbrack){
                updateSymBuf();//todo
            }
            return;
        }
        updateSymBuf();
    }else{
        error(5);//todo ӦΪ��ʶ��
        return;
    }
    fprintf(fout,"\t\tthis is a read stat.\n");
}
//switch��ͷ
void switchStat(){//�������䣾  ::=  switch ��(�������ʽ����)�� ��{����������ۣ�ȱʡ���ݡ�}��
    if(symBuf[symBufIdx].id!=switchsy){
        error(5);//todo Ӧ��switch
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    expr();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){
        error(5);//todo Ӧ��switch
        return;
    }
    updateSymBuf();
    caseStat();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id==defaultsy){//!first����Ϊ{default}
        defaultCase();//!��ѡ��
    }
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo Ӧ��}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a switch stat.\n");
}
//case��ͷ
void caseStat(){//�������   ::=  ���������䣾{���������䣾}
    if(symBuf[symBufIdx].id!=casesy){//!����һ��case���
        error(5);//todo Ӧ��case
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
        error(5);//todo Ӧ��case
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==charcon){
        updateSymBuf();
    }else {
        numDef();//!ֱ�ӵ���
    }
    if(symBuf[symBufIdx].id!=colon){
        error(5);//todo Ӧ��:
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    fprintf(fout,"\t\tthis is a one case.\n");
}
//default��ͷ
void defaultCase(){//��ȱʡ��::=default : ����䣾
    if(symBuf[symBufIdx].id!=defaultsy){
        error(5);//todo Ӧ��default
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=colon){
        error(5);//todo Ӧ��:
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    fprintf(fout,"\t\tthis is a default case.\n");
}
//return��ͷ
void retStat(){//��������䣾::=return[��(�������ʽ����)��]
    if(symBuf[symBufIdx].id!=returnsy){
        error(5);//todo Ӧ��return
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lparent){//!��ѡ��
        updateSymBuf();
        expr();//!ֱ�ӵ���
        if(symBuf[symBufIdx].id==rparent){
            updateSymBuf();
        }else{
            error(5);//todo Ӧ��)
            if(symBuf[symBufIdx].id==rbrack || symBuf[symBufIdx].id==rbrace){//todo
                updateSymBuf();
            }
            return;
        }
    }
    fprintf(fout,"\t\tthis is a return stat.\n");
}
