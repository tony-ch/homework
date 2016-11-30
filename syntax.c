#include "global.h"
#include "err.h"

struct TABREC tab[TMAX];
int tidx=0;//tab top index
struct BTABREC btab[TMAX];
int btidx=0;//func tab top index

char kindstr[5][10]={"var","const","func","arr","para"};//todo debug
char typestr[3][10]={"void","int","char"};//todo debug

int adrOffset=0;
int midx=0;
struct MIDCODE mCode[CODEMAX];

int temVarCnt=0;
int labCnt=0;
int lab[LABMAX];
/*case record
struct caseRecord caseTab[CASEMAX];
int caseCnt=0;
 */


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
        fprintf(fout,"\t\ttab index: %d,\tname: %s,\tkind: %s,\ttype: %s,\tvalue: %d,\tadr: %d\n",
        i,tab[i].name,kindstr[tab[i].kind],typestr[tab[i].typ],tab[i].value,tab[i].adr);
    }
    for(i=0;i<btidx;i++){
        fprintf(fout,"\t\tbtab index: %d,\tname: %s,\ttidx: %d\n",
                i,btab[i].name,btab[i].tidx);
    }
    printCode();

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
    enum KINDS kind=conkind;
    enum TYPES type;
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
        mCode[midx].op=conOp;
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,value);
        adrOffset++;
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
            mCode[midx].op=conOp;
            mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
            mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
            mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
            midx++;
            enter(name,kind,type,value);
            adrOffset++;
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
        mCode[midx].op=conOp;
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,value);
        adrOffset++;
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
            mCode[midx].op=conOp;
            mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
            mCode[midx].arg2.value=value;mCode[midx].arg2Typ=varg;
            mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
            midx++;
            enter(name,kind,type,value);
            adrOffset++;
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
    enum KINDS kind;
    enum TYPES type;
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
    value=1;
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
    mCode[midx].op=(kind==arrkind?arrOp:varOp);
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=value;mCode[midx].arg2Typ=(kind==arrkind?varg:earg);
    mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
    midx++;
    enter(name,kind,type,value);
    adrOffset=adrOffset+value;
    while(symBuf[symBufIdx].id==comma){
        updateSymBuf();
        if(symBuf[symBufIdx].id!=ident){//!��Ӧ�����ͱ�ʶ��
            error(5);//todo ӦΪ��ʶ��
            return;
        }
        strcpy(name,symBuf[symBufIdx].token);
        kind=varkind;
        value=1;
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
        mCode[midx].op=(kind==arrkind?arrOp:varOp);
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2.value=value;mCode[midx].arg2Typ=(kind==arrkind?varg:earg);
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,value);
        adrOffset=adrOffset+value;
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
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur,btidCur;
    int mIdxCur;
    kind=funkind;
    tIdxCur=tidx;btidCur=btidx;
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
    mIdxCur=midx;
    mCode[midx].op=funOp;
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=0;mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=btidx;mCode[midx].rTyp=btiarg;
    midx++;
    enter(name,kind,type,0);
    adrOffset=0;
    //adrOffset++;//for $fp; todo
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tIdxCur].value=value;
    mCode[mIdxCur].arg2.value=value;
    adrOffset=adrOffset+10;//todo for $ra and $s0-$s7
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tIdxCur].name,tab[tIdxCur].value);
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
    mCode[midx].op=endFunOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.btid=btidCur;mCode[midx].rTyp=btiarg;
    midx++;
    btab[btidCur].spacesz=adrOffset;
    btab[btidCur].paraN=value;
    updateSymBuf();
    fprintf(fout,"\t\tthis is a ret func dec.\n");
}

void voidFuncDef(){//���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur,btIdxCur;
    int mIdxCur;
    kind=funkind;
    tIdxCur=tidx;btIdxCur=btidx;
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
    mIdxCur=midx;
    mCode[midx].op=funOp;
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=0;mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=btidx;mCode[midx].rTyp=btiarg;
    midx++;
    enter(name,kind,type,0);
    adrOffset=0;
    //adrOffset++;//todo for $fp
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    value=paraList();
    tab[tIdxCur].value=value;
    mCode[mIdxCur].arg2.value=value;
    adrOffset=adrOffset+10;//todo for $fp($sp+16) $ra($sp+20) and $s0-$s7
    fprintf(fout,"\t\tenter para num for func %s, para num: %d\n",tab[tIdxCur].name,tab[tIdxCur].value);
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
    mCode[midx].op=endFunOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.btid=btIdxCur;mCode[midx].rTyp=btiarg;
    midx++;
    btab[btIdxCur].spacesz=adrOffset;
    btab[btIdxCur].paraN=value;
    updateSymBuf();
    fprintf(fout,"\t\tthis is a void func dec.\n");
}

int paraList(){//�����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}|���գ�
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
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
        mCode[midx].op=paraOp;
        mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
        midx++;
        enter(name,kind,type,0);
        paraCnt=paraCnt+1;
        adrOffset++;
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
            mCode[midx].op=paraOp;
            mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
            mCode[midx].arg2Typ=earg;
            mCode[midx].res.tidx=tidx;mCode[midx].rTyp=tiarg;
            midx++;
            enter(name,kind,type,0);
            adrOffset++;
            paraCnt=paraCnt+1;
            updateSymBuf();
        }
    }
    adrOffset+=(4-paraCnt);
    fprintf(fout,"\t\tthis is para list.\n");
    return paraCnt;
}

void mainDef(){//����������    ::= void main��(����)�� ��{����������䣾��}��
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind=funkind;
    int tidCur=tidx;
    int btidCur=btidx;
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
    mCode[midx].op=funOp;
    mCode[midx].arg1.typ=type;mCode[midx].arg1Typ=targ;//const type value name
    mCode[midx].arg2.value=0; mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=btidx;mCode[midx].rTyp=btiarg;
    midx++;
    enter(name,kind,type,0);
    adrOffset=0;
    adrOffset++;//for fp;
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
    mCode[midx].op=endFunOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.tidx=btidCur;mCode[midx].rTyp=tiarg;
    midx++;
    btab[btidCur].spacesz=adrOffset;
    fprintf(fout,"\t\tthis is main func dec.\n");
}

int call(int hasRet){
    int funcId,resTid=0,paraCnt,funcBtid=0;
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo
        return -1;
    }
    funcId=lookup(symBuf[symBufIdx].token,1);
    while (funcBtid<btidx && strcmp(btab[funcBtid].name,tab[funcId].name)!=0){
        funcBtid++;
    }
    if(funcId==-1){
        error(99);// todo ����δ����
        return -1;
    }
    if(tab[funcId].typ==voidtyp && hasRet==1){
        error(99);//todo Ӧ���ַ���ֵ����
        return -1;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo
        return -1;
    }
    if(hasRet){
        resTid=getTemVar();
        tab[resTid].typ=tab[funcId].typ;
    }
    updateSymBuf();//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
    paraCnt=valueParaList();//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
    mCode[midx].op=callOp;
    mCode[midx].arg1.tidx=resTid;mCode[midx].arg1Typ=hasRet==1?tiarg:earg;//const type value name
    mCode[midx].arg2.value=paraCnt;mCode[midx].arg2Typ=varg;
    mCode[midx].res.btid=funcBtid;mCode[midx].rTyp=btiarg;
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return -1;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a call stat.\n");
    return resTid;
}


int valueParaList(){//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
    int paraCnt=0;
    int resTid;
    if(symBuf[symBufIdx].id==rparent){//!��
        //getsym();//!
    }else{
        resTid=expr();//!����һ��
        mCode[midx].op=calPaOp;
        mCode[midx].arg1Typ=earg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=resTid;mCode[midx].rTyp=tiarg;
        midx++;
        paraCnt++;
        while(symBuf[symBufIdx].id==comma){
            updateSymBuf();
            resTid=expr();
            mCode[midx].op=calPaOp;
            mCode[midx].arg1Typ=earg;//const type value name
            mCode[midx].arg2Typ=earg;
            mCode[midx].res.tidx=resTid;mCode[midx].rTyp=tiarg;
            midx++;
            paraCnt++;
        }
    }
    fprintf(fout,"\t\tthis is value para list.\n");
    return paraCnt;
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
        enum SYMBOL nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==become || nextSym==lbrack){
            assignment();
        }else if(nextSym==lparent){
            call(0);//todo �������ַ���ֵ��
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

int expr(){//�����ʽ��::=�ۣ������ݣ��{���ӷ�����������}
    int ti1,ti2,resTi,negflag;
    negflag=0;
    if(symBuf[symBufIdx].id==plus){//!��ѡ��
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==minus){
        updateSymBuf();
        negflag=1;
    }
    ti1=term();
    if(negflag){
        resTi=getTemVar();
        mCode[midx].op=subOp;
        mCode[midx].arg1.value=0; mCode[midx].arg1Typ=varg;//const type value name
        mCode[midx].arg2.tidx=ti1; mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    }else{
        resTi=ti1;
    }
    while(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus){//!��+-�ָ�
        enum MOP op=symBuf[symBufIdx].id==plus?addOp:subOp;
        updateSymBuf();
        ti1=resTi;
        ti2=term();
        resTi=getTemVar();
        mCode[midx].op=op;
        mCode[midx].arg1.tidx=ti1; mCode[midx].arg1Typ=tiarg;//const type value name
        mCode[midx].arg2.tidx=ti2; mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    }
    fprintf(fout,"\t\tthis is an expr.\n");
    return resTi;
}

int term(){//���::=�����ӣ�{���˷�������������ӣ�}
    int ti1,ti2,resTi;
    ti1=factor();
    resTi=ti1;
    while(symBuf[symBufIdx].id==mul || symBuf[symBufIdx].id==divd){
        enum MOP op=symBuf[symBufIdx].id==mul?mulOp:divOp;
        updateSymBuf();
        ti1=resTi;
        ti2=factor();
        resTi=getTemVar();
        mCode[midx].op=op;
        mCode[midx].arg1.tidx=ti1; mCode[midx].arg1Typ=tiarg;//const type value name
        mCode[midx].arg2.tidx=ti2; mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;

    }
    fprintf(fout,"\t\tthis is a term.\n");
    return resTi;
}

int factor(){//�����ӣ�::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
    int resTi;
    if(symBuf[symBufIdx].id==ident){
        enum SYMBOL nextSym=symBuf[(symBufIdx+1)%3].id;
        if(nextSym==lparent){
            return call(1);
        }else{
            resTi=lookup(symBuf[symBufIdx].token,0);//��ʶ��
            updateSymBuf();
            if(symBuf[symBufIdx].id==lbrack){//!��ѡ�� ����ʶ������[�������ʽ����]��
                int ti1,ti2;
                if(tab[resTi].kind!=arrkind){
                    error(99);//todo ��Ϊ����
                    return -1;
                }
                ti1=resTi;
                updateSymBuf();
                ti2=expr();
                resTi=getTemVar();
                tab[resTi].typ=tab[ti1].typ;//!������Ϊ����Ԫ�ص�����
                mCode[midx].op=getArrOp;
                mCode[midx].arg1.tidx=ti1; mCode[midx].arg1Typ=tiarg;//const type value name
                mCode[midx].arg2.tidx=ti2; mCode[midx].arg2Typ=tiarg;
                mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
                midx++;
                if(symBuf[symBufIdx].id!=rbrack){
                    error(5);//todo Ӧ��]
                    return -1;
                }
                updateSymBuf();
            }
        }
    }else if(symBuf[symBufIdx].id==charcon){
        resTi=getTemVar();
        tab[resTi].typ=chtyp;
        mCode[midx].op=liop;//todo ��Ϊ li
        mCode[midx].arg1.value=symBuf[symBufIdx].token[1]; mCode[midx].arg1Typ=varg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
        updateSymBuf();
    }else if(symBuf[symBufIdx].id==plus || symBuf[symBufIdx].id==minus || symBuf[symBufIdx].id==zero || symBuf[symBufIdx].id==unsignum){//!���� first ����
        resTi=getTemVar();
        mCode[midx].op=liop;
        mCode[midx].arg1.value=numDef(); mCode[midx].arg1Typ=varg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    }else if(symBuf[symBufIdx].id==lparent){//��(�������ʽ����)��
        updateSymBuf();
        resTi=expr();
        if(symBuf[symBufIdx].id!=rparent){
            error(5);//todo Ӧ��])
            return -1;
        }
        updateSymBuf();
    }else{
        error(5);//todo �Ƿ�����
        return -1;
    }
    fprintf(fout,"\t\tthis is a factor.\n");
    return resTi;
}

void assignment(){//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
    int resTid=-1,ti1=-1,ti2=-1;
    int isArr=0;
    if(symBuf[symBufIdx].id!=ident){
        error(5);//todo Ӧ�Ǳ�ʶ��
        return;
    }
    resTid=lookup(symBuf[symBufIdx].token,0);
    updateSymBuf();
    if(symBuf[symBufIdx].id==lbrack){
        updateSymBuf();//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
        ti2=expr();isArr=1;
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
        ti1=expr();
    }else if(symBuf[symBufIdx].id==become){//������ֵ����ֵ��䣾   ::=  ����ʶ�����������ʽ��
        updateSymBuf();
        ti1=expr();//!ֱ�ӵ���
    }else{
        error(5);//todo �Ƿ����
        return;
    }
    mCode[midx].op=isArr?setArrOp:becomeOp;
    mCode[midx].arg1.tidx=ti1;mCode[midx].arg1Typ=tiarg;//const type value name
    mCode[midx].arg2.tidx=ti2;mCode[midx].arg2Typ=isArr?tiarg:earg;
    mCode[midx].res.tidx=resTid;mCode[midx].rTyp=tiarg;
    midx++;
    fprintf(fout,"\t\tthis is a assignment.\n");
}
/*
 * if cond
 * (true)->
 * (false)-> goto labElse(BP)
 * then
 *  ...
 *  goto labNext(BP)
 * else
 * labElse:
 *  ...
 *  goto labNext
 * end if
 * labNext:
 */


//if��ͷ
void ifStat(){//��������䣾::=if ��(������������)������䣾��else����䣾��
    int midxElse=0,midxNext=0,brTi=-1;
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
    brTi=condition();//!ֱ�ӵ���
    midxElse=midx;
    mCode[midx].op=brfOp;
    mCode[midx].arg1.tidx=brTi;mCode[midx].arg1Typ=tiarg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//Ӧ��)
        return;
    }
	updateSymBuf();//!read one more sym
    stat();//!ֱ�ӵ���
    midxNext=midx;
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    mCode[midxElse].res.labIdx=getLab();
    if(symBuf[symBufIdx].id==elsesy){//!��ѡ��
		updateSymBuf();//!one more sym
        stat();
    }
    mCode[midxNext].res.labIdx=getLab();
    fprintf(fout,"\t\tthis is an if stat.\n");
}

int condition(){//��������::=�����ʽ������ϵ������������ʽ���������ʽ��
    int ti1,ti2=-1,resTi;
    int hasOp=0;
    enum MOP op=sltOp;
    ti1=expr();//!ֱ�ӵ���
    if(symBuf[symBufIdx].id==les){//!��ѡ��
        hasOp=1;op=sltOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==leq){
        hasOp=1;op=sleOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==gtr){
        hasOp=1;op=sgtOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==geq){
        hasOp=1;op=sgeOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==eql){
        hasOp=1;op=seqOp;
        updateSymBuf();
        ti2=expr();
    }else if(symBuf[symBufIdx].id==neq){//!©д
        hasOp=1;op=sneOp;
        updateSymBuf();
        ti2=expr();
    }
    if(hasOp==1){
        resTi=getTemVar();
        mCode[midx].op=op;
        mCode[midx].arg1.tidx=ti1;mCode[midx].arg1Typ=tiarg;//const type value name
        mCode[midx].arg2.tidx=ti2;mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=resTi;mCode[midx].rTyp=tiarg;
        midx++;
    } else{
        resTi=ti1;
    }
    fprintf(fout,"\t\tthis is a condition.\n");
    return resTi;
}
/*
 * /*
 * labLoop:
 * while cond
 * (true)->
 * (false)-> goto labNext(BP)
 * do
 *  ...
 *  goto labLoop
 * end if
 * labNext:
 */
//while��ͷ
void  whileStat(){//��ѭ����䣾::=while ��(������������)������䣾
    int loopLabIdx=0,endLabIdx=0,brTi=-1,loopMidx=-1;
    if(symBuf[symBufIdx].id!=whilesy){
        error(5);//todo Ӧ��while
        return;
    }
    loopLabIdx=getLab();
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){
        error(5);//todo Ӧ��(
        return;
    }
    updateSymBuf();
    brTi=condition();//!ֱ�ӵ���
    loopMidx=midx;
    mCode[midx].op=brfOp;
    mCode[midx].arg1.tidx=brTi;mCode[midx].arg1Typ=tiarg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    if(symBuf[symBufIdx].id!=rparent){
        error(5);//todo Ӧ��)
        return;
    }
    updateSymBuf();
    stat();//!ֱ�ӵ���
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=loopLabIdx;mCode[midx].rTyp=liarg;//BACKPATCH
    midx++;
    endLabIdx=getLab();
    mCode[loopMidx].res.labIdx=endLabIdx;
    fprintf(fout,"\t\tthis is a while stat.\n");
}
//printf��ͷ
void writeStat(){//��д��䣾::=printf��(�� ���ַ�����,�����ʽ�� ��)��|printf ��(�����ַ����� ��)��|printf ��(�������ʽ����)��
    char str[STRMAX]="";
    int expTid=-1;
    int hasStr=0,hasExp=0;
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
        strcpy(str,symBuf[symBufIdx].token);hasStr=1;
        updateSymBuf();
        if(symBuf[symBufIdx].id==comma){//!��ѡ��
            updateSymBuf();
            expTid=expr();hasExp=1;
        }
    }else{
        expTid=expr();//!ֱ�ӵ��� todo expr first����
        hasExp=1;
    }
    mCode[midx].op=writeOp;
    mCode[midx].arg1Typ=earg;//const type value name
    if(hasStr){
        mCode[midx].arg2.stridx=enterStr(str);
    }
    mCode[midx].arg2Typ=hasStr?siarg:earg;
    mCode[midx].res.tidx=expTid;mCode[midx].rTyp=hasExp?tiarg:earg;
    midx++;
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
        int ti=lookup(symBuf[symBufIdx].token,0);
        if(ti==-1){
            error(99);//todo ��ʶ��δ����
            return;
        }
        mCode[midx].op=readOp;
        mCode[midx].arg1Typ=earg;//const type value name
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.tidx=ti;mCode[midx].rTyp=tiarg;
        midx++;
        updateSymBuf();
        while(symBuf[symBufIdx].id==comma){//!ѭ����,�ָ�
            updateSymBuf();
            if(symBuf[symBufIdx].id!=ident){
                error(5);//todo Ӧ�Ǳ�ʶ��
                return;
            }
            ti=lookup(symBuf[symBufIdx].token,0);
            if(ti==-1){
                error(99);//todo ��ʶ��δ����
                return;
            }
            mCode[midx].op=readOp;
            mCode[midx].arg1Typ=earg;//const type value name
            mCode[midx].arg2Typ=earg;
            mCode[midx].res.tidx=ti;mCode[midx].rTyp=tiarg;
            midx++;
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
/*
 * swtich E
 * goto labTest(BP)
 * labCase1:
 *      goto labNext(BP)
 * labCase2:
 *      goto labNext(BP)
 * labDefault:
 *      goto labNext(BP)
 * labTest:
 *      if E=e1 goto labCase1(already exits)
 *      if E=e2 goto labCase2(already exits)
 *      goto labDefault      (already exits)
 * labNext:
 */
//switch:    cal E; gen labTest; gen labNext; goto labTest; BP labNext
//case:      cmp E; gen labCase; goto labNext;
//default:   gen labDefault;     goto labNext;
//switch��ͷ
void switchStat(){//�������䣾  ::=  switch ��(�������ʽ����)�� ��{����������ۣ�ȱʡ���ݡ�}��
    int eva=-1;
    int swtMidx=-1;
    int hasDefault=0;
    int nextLabIdx=0;int i,brTi;
    /*case record
    int caseCntCur;
    caseCnt++;
    caseCntCur=caseCnt;
     */
    struct CASTAB casetb;
    casetb.caseCnt=0;
    if(symBuf[symBufIdx].id!=switchsy){ error(5);       return; }//todo Ӧ��switch
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lparent){        error(5);        return;    }//todo Ӧ��(
    updateSymBuf();
    eva=expr();//!ֱ�ӵ���
    swtMidx=midx;//FOR BACK PATCH goto TEST
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//goto test
    midx++;
    if(symBuf[symBufIdx].id!=rparent){        error(5);        return;    }//todo Ӧ��)
    updateSymBuf();
    if(symBuf[symBufIdx].id!=lbrace){ error(5);return; }//todo Ӧ��{
    updateSymBuf();
    caseStat(&casetb);//!ֱ�ӵ���
    if(symBuf[symBufIdx].id==defaultsy){//!first����Ϊ{default}
        hasDefault=1;
        defaultCase(&casetb);//!��ѡ��
    }
    mCode[swtMidx].res.labIdx=getLab();//gen labTest
    //todo labTest code
    for(i=0;i<casetb.caseCnt-hasDefault;i++){
        brTi=getTemVar();
        mCode[midx].op=sneOp;
        mCode[midx].arg1.value=casetb.cValue[i];mCode[midx].arg1Typ=varg;
        mCode[midx].arg2.tidx=eva;mCode[midx].arg2Typ=tiarg;
        mCode[midx].res.tidx=brTi;mCode[midx].rTyp=tiarg;//goto test
        midx++;
        mCode[midx].op=brfOp;
        mCode[midx].arg1.tidx=brTi;mCode[midx].arg1Typ=tiarg;
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.labIdx=casetb.labIdx[i];mCode[midx].rTyp=liarg;//goto test
        midx++;
    }
    if(hasDefault==1){
        mCode[midx].op=jOp;
        mCode[midx].arg1Typ=earg;
        mCode[midx].arg2Typ=earg;
        mCode[midx].res.labIdx=casetb.labIdx[casetb.caseCnt-1];mCode[midx].rTyp=liarg;//goto test
        midx++;
    }
    //gen nextLab
    nextLabIdx=getLab();
    //todo BP nextLab in case and default
    for(i=0;i<casetb.caseCnt;i++){
        mCode[casetb.midx[i]].res.labIdx=nextLabIdx;
    }
    if(symBuf[symBufIdx].id!=rbrace){
        error(5);//todo Ӧ��}
        return;
    }
    updateSymBuf();
    fprintf(fout,"\t\tthis is a switch stat.\n");
}
//case��ͷ
void caseStat(struct CASTAB* casetb){//�������   ::=  ���������䣾{���������䣾}
    if(symBuf[symBufIdx].id!=casesy){//!����һ��case���
        error(5);//todo Ӧ��case
        return;
    }
    while(symBuf[symBufIdx].id==casesy){//! first����Ϊ{case}
        oneCase(casetb);
    }
    fprintf(fout,"\t\tthis is a case table.\n");
}
//case��ͷ
void oneCase(struct CASTAB* casetb){//���������䣾::=case��������������䣾
    if(casetb->caseCnt==CASEMAX){
        error(99);
        return;//case��ǩ����
    }
    if(symBuf[symBufIdx].id!=casesy){
        error(5);//todo Ӧ��case
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==charcon){
        casetb->cValue[casetb->caseCnt]=symBuf[symBufIdx].token[1];
        //caseTab[caseCnt].cValue=symBuf[symBufIdx].token[1];
        updateSymBuf();
    }else {
        casetb->cValue[casetb->caseCnt]=numDef();
        //caseTab[caseCnt].cValue=numDef();//!ֱ�ӵ���
    }
    if(symBuf[symBufIdx].id!=colon){
        error(5);//todo Ӧ��:
        return;
    }
    updateSymBuf();
    casetb->labIdx[casetb->caseCnt]=getLab();
    //caseTab[caseCnt].labIdx=getLab();
    stat();//!ֱ�ӵ���
    casetb->midx[casetb->caseCnt]=midx;
    //caseTab[caseCnt].midx=midx;
    mCode[midx].op=jOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//goto test
    midx++;
    casetb->caseCnt=casetb->caseCnt+1;
    //caseCnt++;
    fprintf(fout,"\t\tthis is a one case.\n");
}
//default��ͷ
void defaultCase(struct CASTAB* casetb){//��ȱʡ��::=default : ����䣾
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
    casetb->labIdx[casetb->caseCnt]=getLab();
    //caseTab[caseCnt].labIdx=getLab();
    stat();//!ֱ�ӵ���
    mCode[midx].op=jOp;
    casetb->midx[casetb->caseCnt]=midx;
    //caseTab[caseCnt].midx=midx;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.labIdx=0;mCode[midx].rTyp=liarg;//goto test
    midx++;
    //caseCnt++;
    casetb->caseCnt=casetb->caseCnt+1;
    fprintf(fout,"\t\tthis is a default case.\n");
}
//return��ͷ
void retStat(){//��������䣾::=return[��(�������ʽ����)��]
    int hasRet=0;
    int expTid=-1;
    if(symBuf[symBufIdx].id!=returnsy){
        error(5);//todo Ӧ��return
        return;
    }
    updateSymBuf();
    if(symBuf[symBufIdx].id==lparent){//!��ѡ��
        updateSymBuf();
        expTid=expr();//!ֱ�ӵ���
        hasRet=1;
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
    if(btidx==0){
        error(99);//todo �Ƿ����
        return;
    }
    if(tab[btab[btidx-1].tidx].typ==voidtyp && hasRet==1){
        error(99);//todo ӦΪ�޷���ֵret
        return;
    }
    if(tab[btab[btidx-1].tidx].typ==inttyp && (hasRet==0||tab[expTid].typ!=inttyp)){
        error(99);//todo ӦΪint����ֵ
    }
    if(tab[btab[btidx-1].tidx].typ==chtyp && (hasRet==0||tab[expTid].typ!=chtyp)){
        error(99);//todo ӦΪchar����ֵ
    }
    mCode[midx].op=retOp;
    mCode[midx].arg1Typ=earg;//const type value name
    mCode[midx].arg2Typ=earg;
    mCode[midx].res.tidx=expTid;mCode[midx].rTyp=hasRet?tiarg:earg;
    midx++;
    fprintf(fout,"\t\tthis is a return stat.\n");
}
