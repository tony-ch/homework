#include "global.h"
#include "err.h"


void program() {
    if (symBuf[symBufIdx].id == constsy) {//!ͷ����Ϊconst
        decConst();
    }
    int hasVarDef = 0;
    while ((symBuf[symBufIdx].id == intsy || symBuf[symBufIdx].id == charsy) &&
           symBuf[(symBufIdx + 2) % 3].id != lparent) {
        hasVarDef = 1;
        varDef();
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ�Ƿֺ�(F)
        } else {
            updateSymBuf();//!read one more sym
        }
    }
    if (hasVarDef) {
        fprintf(fout, "\t\tthis is dec of var.\n");
    }
    while ((symBuf[symBufIdx].id == intsy || symBuf[symBufIdx].id == charsy || symBuf[symBufIdx].id == voidsy) &&
           symBuf[(symBufIdx + 1) % 3].id != mainsy) {
        if (symBuf[symBufIdx].id == voidsy) {
            voidFuncDef();
        } else {
            retFuncDef();
        }
    }
    mainDef();
    fprintf(fout, "\t\tthis is a program.\n");

    fprintf(fout, "\n");
    printCode();

}

//const ��ͷ
void decConst() {
//    if(symBuf[symBufIdx].id!=constsy){//!���Ը�д�ķ�
//        error(-1);//!should't happen run time err
//        return;
//    }
    updateSymBuf();
    constDef();
    if (symBuf[symBufIdx].id != semicolon) {
        error(4);//!Ӧ��;(F)
    } else {
        updateSymBuf();
    }
    while (symBuf[symBufIdx].id == constsy) {//! first����Ϊ{const}
        updateSymBuf();
        constDef();
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    }
    fprintf(fout, "\t\tthis is dec of const.\n");
}

void constDef() {
    enum KINDS kind = conkind;
    enum TYPES type;
    char name[ALENMAX];
    int value;
    if (symBuf[symBufIdx].id == intsy) {
        type = inttyp;
        updateSymBuf();
        if (symBuf[symBufIdx].id != ident) {
            strcpy(errPlace, "condef");
            error(9);//!Ӧ�Ǳ�ʶ��
            return;
        }
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
        if (symBuf[symBufIdx].id != become) {
            error(18);//!���������г�ʼֵ
            enter(name, kind, type, 0);
            return;
        }
        updateSymBuf();
        if (symBuf[symBufIdx].id != minus && symBuf[symBufIdx].id != plus && symBuf[symBufIdx].id != unsignum &&
            symBuf[symBufIdx].id != zero) {//!first����
            error(19);//!int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�
            enter(name, kind, type, 0);
            return;
        }
        value = numDef(0);
        emitMid(conOp, type, value, tidx, targ, varg, tiarg);
        enter(name, kind, type, value);
        adrOffset++;
        while (symBuf[symBufIdx].id == comma) {//��ѡ��
            updateSymBuf();
            if (symBuf[symBufIdx].id != ident) {
                strcpy(errPlace, "condef");
                error(9);//!Ӧ�Ǳ�ʶ��
                return;
            }
            strcpy(name, symBuf[symBufIdx].token);
            updateSymBuf();
            if (symBuf[symBufIdx].id != become) {
                enter(name, kind, type, 0);
                error(18);//!���������г�ʼֵ
                return;
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id != minus && symBuf[symBufIdx].id != plus && symBuf[symBufIdx].id != unsignum &&
                symBuf[symBufIdx].id != zero) {
                error(19);//!int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�
                enter(name, kind, type, 0);
                return;
            }
            value = numDef(0);
            emitMid(conOp, type, value, tidx, targ, varg, tiarg);
            enter(name, kind, type, value);
            adrOffset++;
        }
    } else if (symBuf[symBufIdx].id == charsy) {
        type = chtyp;
        updateSymBuf();
        if (symBuf[symBufIdx].id != ident) {
            strcpy(errPlace, "condef");
            error(9);//!Ӧ�Ǳ�ʶ��
            return;
        }
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
        if (symBuf[symBufIdx].id != become) {
            enter(name, kind, type, '0');
            error(18);//!���������г�ʼֵ
            return;
        }
        updateSymBuf();
        if (symBuf[symBufIdx].id != charcon) {
            error(19);//!int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�
            enter(name, kind, type, '0');
            return;
        }
        value = symBuf[symBufIdx].token[1];
        emitMid(conOp, type, value, tidx, targ, varg, tiarg);
        enter(name, kind, type, value);
        adrOffset++;
        updateSymBuf();
        while (symBuf[symBufIdx].id == comma) {//��ѡ��
            updateSymBuf();
            if (symBuf[symBufIdx].id != ident) {
                strcpy(errPlace, "condef");
                error(9);//!Ӧ�Ǳ�ʶ��
                return;
            }
            strcpy(name, symBuf[symBufIdx].token);
            updateSymBuf();
            if (symBuf[symBufIdx].id != become) {
                error(18);//!���������г�ʼֵ
                enter(name, kind, type, '0');
                return;
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id != charcon) {
                error(19);//!int�����ĳ�ʼֵӦΪ������char�����ĳ�ʼֵӦΪ�ַ�
                enter(name, kind, type, '0');
                return;
            }
            value = symBuf[symBufIdx].token[1];
            emitMid(conOp, type, value, tidx, targ, varg, tiarg);
            enter(name, kind, type, value);
            adrOffset++;
            updateSymBuf();
        }
    } else {
        error(7);//! ӦΪint��char δ֪�ĳ������ͱ�ʶ
    }
    fprintf(fout, "\t\tthis is const def.\n");
}

/*
void decVar(){//������˵����::=���������壾;{���������壾;}
}
*/
void varDef() {//���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,����ʶ����|����ʶ������[�����޷�����������]�� }
    //enum symbol typ=sym;
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    if (symBuf[symBufIdx].id == intsy) {
        type = inttyp;
    } else if (symBuf[symBufIdx].id == charsy) {
        type = chtyp;
    } else {
        error(-1);//! should't happen , run time err
        return;
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != ident) {
        strcpy(errPlace, "vardef");
        error(9);//!Ӧ�Ǳ�ʶ��
    } else {
        strcpy(name, symBuf[symBufIdx].token);
        kind = varkind;
        value = 1;
        updateSymBuf();
        if (symBuf[symBufIdx].id == lbrack) {//!��ѡ��
            updateSymBuf();
            if (symBuf[symBufIdx].id != unsignum) {
                error(20);//!����Ĵ�СӦΪ��0�޷�������
                value = 1;
            } else {
                value = atoi(symBuf[symBufIdx].token);
            }
            kind = arrkind;
            updateSymBuf();
            if (symBuf[symBufIdx].id != rbrack)
                error(14);//!Ӧ��]
            else
                updateSymBuf();
        }
        emitMid((kind == arrkind ? arrOp : varOp), type, value, tidx, targ, (kind == arrkind ? varg : earg), tiarg);
        enter(name, kind, type, value);
        adrOffset = adrOffset + value;
    }
    while (symBuf[symBufIdx].id == comma) {
        updateSymBuf();
        if (symBuf[symBufIdx].id != ident) {//!��Ӧ�����ͱ�ʶ��
            strcpy(errPlace, "vardef");
            error(9);//!Ӧ�Ǳ�ʶ��
            continue;
        }
        strcpy(name, symBuf[symBufIdx].token);
        kind = varkind;
        value = 1;
        updateSymBuf();
        if (symBuf[symBufIdx].id == lbrack) {//!��ѡ��
            updateSymBuf();
            if (symBuf[symBufIdx].id != unsignum) {
                error(20);//!����Ĵ�СӦΪ��0�޷�������
                value = 1;
            } else {
                value = atoi(symBuf[symBufIdx].token);
            }
            kind = arrkind;
            updateSymBuf();
            if (symBuf[symBufIdx].id != rbrack) {
                error(11);//!Ӧ��)
                return;
            }
            updateSymBuf();
        }
        emitMid((kind == arrkind ? arrOp : varOp), type, value, tidx, targ, (kind == arrkind ? varg : earg), tiarg);
        enter(name, kind, type, value);
        adrOffset = adrOffset + value;
    }
    fprintf(fout, "\t\tthis is var def.\n");
}

int numDef(int useTo) {//��������::= �ۣ������ݣ��޷�������������
    int flag = 0;
    int num = 0;
    if (symBuf[symBufIdx].id == zero) {
        updateSymBuf();
        num = 0;
    } else {
        if (symBuf[symBufIdx].id == plus) {//!��ѡ��
            updateSymBuf();
        } else if (symBuf[symBufIdx].id == minus) {
            updateSymBuf();
            flag = 1;
        }
        if (symBuf[symBufIdx].id == unsignum) {
            num = atoi(symBuf[symBufIdx].token);
            if (flag == 1) {
                num = -num;
            }
            updateSymBuf();
        } else {
            error(21);//!�Ƿ�������
            sprintf(errPlace, "%d", useTo);
            return 0;
        }
    }
    fprintf(fout, "\t\tthis is a num:%d.\n", num);
    return num;
}

void retFuncDef() {//���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur, btidCur;
    int mIdxCur;
    kind = funkind;
    tIdxCur = tidx;
    btidCur = btidx;
    if (symBuf[symBufIdx].id == charsy) {
        type = chtyp;
    } else if (symBuf[symBufIdx].id == intsy) {
        type = inttyp;
    } else {
        error(-1);//! should't happen , run time err
        return;
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != ident) {
        strcpy(errPlace, "fundef");
        error(9);//!Ӧ�Ǳ�ʶ��
        sprintf(name, "#fun_%d", btidx);
    } else {
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
    }
    mIdxCur = midx;
    emitMid(funOp, type, 0, btidx, targ, varg, btiarg);
    enter(name, kind, type, 0);
    adrOffset = 0;
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    value = paraList();
    tab[tIdxCur].value = value;
    mCode[mIdxCur].arg2.value = value;
    adrOffset = adrOffset + 10;//for $fp $ra and $s0-$s7
    fprintf(fout, "\t\tenter para num for func %s, para num: %d\n", tab[tIdxCur].name, tab[tIdxCur].value);
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();
    if (symBuf[symBufIdx].id != lbrace)
        error(12);//!Ӧ��{
    else
        updateSymBuf();
    complexStat();
    if (symBuf[symBufIdx].id != rbrace)
        error(13);//!Ӧ��}
    else
        updateSymBuf();
    if (btab[btidCur].reted == 0) {
        error(30);
    }
    emitMid(endFunOp, -1, -1, btidCur, earg, earg, btiarg);
    btab[btidCur].spacesz = adrOffset;
    btab[btidCur].paraN = value;
    fprintf(fout, "\t\tthis is a ret func dec.\n");
}

void voidFuncDef() {//���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur, btIdxCur;
    int mIdxCur;
    kind = funkind;
    tIdxCur = tidx;
    btIdxCur = btidx;
    if (symBuf[symBufIdx].id != voidsy) {
        error(-1);//!should't happen , run time err
        return;
    }
    type = voidtyp;
    updateSymBuf();
    if (symBuf[symBufIdx].id != ident) {
        strcpy(errPlace, "fundef");
        error(9);//!Ӧ�Ǳ�ʶ��
        sprintf(name, "#func_%d", btidx);
    } else {
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
    }
    mIdxCur = midx;
    emitMid(funOp, type, 0, btidx, targ, varg, btiarg);
    enter(name, kind, type, 0);
    adrOffset = 0;
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    value = paraList();
    tab[tIdxCur].value = value;
    mCode[mIdxCur].arg2.value = value;
    adrOffset = adrOffset + 10;//!for $fp($sp+16) $ra($sp+20) and $s0-$s7
    fprintf(fout, "\t\tenter para num for func %s, para num: %d\n", tab[tIdxCur].name, tab[tIdxCur].value);
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();
    if (symBuf[symBufIdx].id != lbrace)
        error(12);//!Ӧ��{
    else
        updateSymBuf();
    complexStat();
    if (symBuf[symBufIdx].id != rbrace)
        error(13);//!Ӧ��}
    else
        updateSymBuf();
    emitMid(endFunOp, -1, -1, btIdxCur, earg, earg, btiarg);
    btab[btIdxCur].spacesz = adrOffset;
    btab[btIdxCur].paraN = value;
    fprintf(fout, "\t\tthis is a void func dec.\n");
}

int paraList() {//�����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}|���գ�
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind = parakind;
    int paraCnt = 0;
    if (symBuf[symBufIdx].id == charsy || symBuf[symBufIdx].id == intsy) {//!����Ϊ��
        if (symBuf[symBufIdx].id == charsy) {
            type = chtyp;
        } else {
            type = inttyp;
        }
        updateSymBuf();
        if (symBuf[symBufIdx].id != ident) {
            strcpy(errPlace, "paralist");
            error(9);//!Ӧ�Ǳ�ʶ��
            sprintf(name, "#p_%d_%d", btidx, paraCnt);
        } else {
            strcpy(name, symBuf[symBufIdx].token);
            updateSymBuf();
        }
        emitMid(paraOp, type, -1, tidx, targ, earg, tiarg);
        enter(name, kind, type, 0);
        paraCnt = paraCnt + 1;
        adrOffset++;
        while (symBuf[symBufIdx].id == comma) {
            updateSymBuf();
            if (symBuf[symBufIdx].id != charsy && symBuf[symBufIdx].id != intsy) {
                error(8);//!δ֪�Ĳ������ͱ�ʶ��
                continue;
            }
            if (symBuf[symBufIdx].id == charsy) {
                type = chtyp;
            } else {
                type = inttyp;
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id != ident) {
                strcpy(errPlace, "paralist");
                error(9);//!Ӧ�Ǳ�ʶ��
                sprintf(name, "#p_%d_%d", btidx, paraCnt);
            } else {
                strcpy(name, symBuf[symBufIdx].token);
                updateSymBuf();
            }
            emitMid(paraOp, type, -1, tidx, targ, earg, tiarg);
            enter(name, kind, type, 0);
            adrOffset++;
            paraCnt = paraCnt + 1;
        }
    }
    adrOffset += (4 - paraCnt);
    fprintf(fout, "\t\tthis is para list.\n");
    return paraCnt;
}

void mainDef() {//����������    ::= void main��(����)�� ��{����������䣾��}��
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind = funkind;
    //int tidCur=tidx;
    int btidCur = btidx;
    if (symBuf[symBufIdx].id != voidsy) {
        error(22);//!
    } else {
        updateSymBuf();
    }
    type = voidtyp;
    if (symBuf[symBufIdx].id != mainsy) {
        error(23);//!
    }
    strcpy(name, symBuf[symBufIdx].token);
    emitMid(funOp, type, 0, btidx, targ, varg, btiarg);
    enter(name, kind, type, 0);
    adrOffset = 0;
    adrOffset++;//for fp;
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();
    if (symBuf[symBufIdx].id != lbrace)
        error(12);//!Ӧ��{
    else
        updateSymBuf();
    complexStat();
    printSym();
    if (symBuf[symBufIdx].id != rbrace) {
        error(13);//!Ӧ��}
    }
    emitMid(endFunOp, -1, -1, btidCur, earg, earg, btiarg);
    btab[btidCur].spacesz = adrOffset;
    fprintf(fout, "\t\tthis is main func dec.\n");
}

int call(int needRet) {
    if (symBuf[symBufIdx].id != ident) {
        error(-1);//! should't happen , run time err
        return -1;
    }
    int funcId = lookup(symBuf[symBufIdx].token, 1);
    int funcBtid = 0;
    while (funcBtid < btidx && strcmp(btab[funcBtid].name, tab[funcId].name) != 0) {
        funcBtid++;
    }
    if (funcId == -1) {
        error(16);//!����δ����
        sprintf(errPlace, "%d", needRet);
        return -1;
    }
    if (tab[funcId].typ == voidtyp && needRet == 1) {
        error(25);//!Ӧ���з���ֵ���� �Ƿ����
        return -1;
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    int resTid = 0;
    if (needRet) {
        resTid = getTemVar();
        tab[resTid].typ = tab[funcId].typ;
    }
    //���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
    int paraCnt = valueParaList(funcId);//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
    emitMid(callOp, resTid, paraCnt, funcBtid, needRet == 1 ? tiarg : earg, varg, btiarg);
    if (btab[btidx - 1].callParaN < paraCnt)//������ǰ������ֵ��������
        btab[btidx - 1].callParaN = paraCnt;
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();
    fprintf(fout, "\t\tthis is a call stat.\n");
    return resTid;
}


int valueParaList(int funcId) {//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
    int paraCnt = 0;
    if (symBuf[symBufIdx].id != rparent) {//!��Ϊ��
        int resTid = expr();//!����һ��
        emitMid(calPaOp, -1, -1, resTid, earg, earg, tiarg);
        paraCnt++;
        if (tab[resTid].typ != tab[funcId + paraCnt].typ) {
            warn(3);//�������Ͳ���ȷ
        }
        while (symBuf[symBufIdx].id == comma) {
            updateSymBuf();
            resTid = expr();
            emitMid(calPaOp, -1, -1, resTid, earg, earg, tiarg);
            paraCnt++;
            if (tab[resTid].typ != tab[funcId + paraCnt].typ) {
                warn(3);//�������Ͳ���ȷ
            }
        }
    }
    if (tab[funcId].value != paraCnt) {
        error(31);//!������������ȷ
    }
    fprintf(fout, "\t\tthis is value para list.\n");
    return paraCnt;
}

void complexStat() {//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
    if (symBuf[symBufIdx].id == constsy) {//!ͷ����Ϊconst
        decConst();
    }
    int hasVarDef = 0;
    while (symBuf[symBufIdx].id == charsy || symBuf[symBufIdx].id == intsy) {
        varDef();
        hasVarDef = 1;
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    }
    if (hasVarDef) {
        fprintf(fout, "\t\tthis is dec of var.\n");
    }
    statList();
    fprintf(fout, "\t\tthis is complex stat.\n");
}

void
stat() {//����䣾::= ��������䣾����ѭ����䣾| ��{��������У���}�������з���ֵ����������䣾; | ���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;|�������䣾����������䣾;
    if (symBuf[symBufIdx].id == ifsy) {//!ͷ���ż���
        ifStat();
    } else if (symBuf[symBufIdx].id == whilesy) {
        whileStat();
    } else if (symBuf[symBufIdx].id == lbrace) {//�����
        updateSymBuf();
        statList();
        if (symBuf[symBufIdx].id != rbrace)
            error(13);//!Ӧ��}
        else
            updateSymBuf();
    } else if (symBuf[symBufIdx].id == scanfsy) {
        readStat();
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    } else if (symBuf[symBufIdx].id == printfsy) {
        writeStat();
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    } else if (symBuf[symBufIdx].id == returnsy) {
        retStat();
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    } else if (symBuf[symBufIdx].id == switchsy) {
        switchStat();
    } else if (symBuf[symBufIdx].id == ident) {//!first�����ཻ
        enum SYMBOL nextSym = symBuf[(symBufIdx + 1) % 3].id;
        if (nextSym == become || nextSym == lbrack) {
            assignment();
        } else if (nextSym == lparent) {
            call(0);
        } else {
            error(25);//!�Ƿ����
            return;
        }
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    } else {
        if (symBuf[symBufIdx].id != semicolon) {
            error(4);//!!Ӧ��;(F)
        } else {
            updateSymBuf();
        }
    }//!��
    fprintf(fout, "\t\tthis is a stat.\n");
}

void statList() {
    //!����û��һ��
    while (symBuf[symBufIdx].id == ifsy || symBuf[symBufIdx].id == whilesy || symBuf[symBufIdx].id == lbrace ||
           symBuf[symBufIdx].id == scanfsy || symBuf[symBufIdx].id == printfsy || symBuf[symBufIdx].id == returnsy ||
           symBuf[symBufIdx].id == switchsy || symBuf[symBufIdx].id == ident ||
           symBuf[symBufIdx].id == semicolon) {//!first����
        stat();
    }
    fprintf(fout, "\t\tthis is a stat list.\n");
}

int expr() {//�����ʽ��::=�ۣ������ݣ��{���ӷ�����������}
    int ti1, ti2, resTi, negflag;
    negflag = 0;
    if (symBuf[symBufIdx].id == plus) {//!��ѡ��
        updateSymBuf();
    } else if (symBuf[symBufIdx].id == minus) {
        updateSymBuf();
        negflag = 1;
    }
    ti1 = term();
    if (negflag) {
        resTi = getTemVar();
        emitMid(subOp, 0, ti1, resTi, varg, tiarg, tiarg);
    } else {
        resTi = ti1;
    }
    while (symBuf[symBufIdx].id == plus || symBuf[symBufIdx].id == minus) {//!��+-�ָ�
        enum MOP op = symBuf[symBufIdx].id == plus ? addOp : subOp;
        updateSymBuf();
        ti1 = resTi;
        ti2 = term();
        resTi = getTemVar();
        emitMid(op, ti1, ti2, resTi, tiarg, tiarg, tiarg);
    }
    fprintf(fout, "\t\tthis is an expr.\n");
    return resTi;
}

int term() {//���::=�����ӣ�{���˷�������������ӣ�}
    int ti1, ti2, resTi;
    ti1 = factor();
    resTi = ti1;
    while (symBuf[symBufIdx].id == mul || symBuf[symBufIdx].id == divd) {
        enum MOP op = symBuf[symBufIdx].id == mul ? mulOp : divOp;
        updateSymBuf();
        ti1 = resTi;
        ti2 = factor();
        resTi = getTemVar();
        emitMid(op, ti1, ti2, resTi, tiarg, tiarg, tiarg);

    }
    fprintf(fout, "\t\tthis is a term.\n");
    return resTi;
}

int factor() {//�����ӣ�::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
    int resTi;
    if (symBuf[symBufIdx].id == ident) {
        enum SYMBOL nextSym = symBuf[(symBufIdx + 1) % 3].id;
        if (nextSym == lparent) {
            return call(1);
        } else {
            resTi = lookup(symBuf[symBufIdx].token, 0);//��ʶ��, 0 not func
            if (resTi == -1) {
                strcpy(errPlace, "fac");
                error(17);//!��ʶ��δ����
                //return -1; ! ��Ӧreturn
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id == lbrack) {//!��ѡ�� ����ʶ������[�������ʽ����]��
                int ti1, ti2;
                if (resTi != -1 && tab[resTi].kind != arrkind) {
                    error(28);//!��Ϊ����
                    //!��Ӧreturn return -1;
                }
                ti1 = resTi;
                updateSymBuf();
                ti2 = expr();
                resTi = getTemVar();
                tab[resTi].typ = ti1 == -1 ? inttyp : tab[ti1].typ;//!������Ϊ����Ԫ�ص�����
                emitMid(getArrOp, ti1, ti2, resTi, tiarg, tiarg, tiarg);
                if (symBuf[symBufIdx].id != rbrack)
                    error(14);//!Ӧ��]
                else
                    updateSymBuf();
            }
        }
    } else if (symBuf[symBufIdx].id == charcon) {
        resTi = getTemVar();
        tab[resTi].typ = chtyp;
        emitMid(liop, symBuf[symBufIdx].token[1], -1, resTi, varg, earg, tiarg);
        updateSymBuf();
    } else if (symBuf[symBufIdx].id == plus || symBuf[symBufIdx].id == minus || symBuf[symBufIdx].id == zero ||
               symBuf[symBufIdx].id == unsignum) {//!���� first ����
        resTi = getTemVar();
        emitMid(liop, numDef(1), -1, resTi, varg, earg, tiarg);
    } else if (symBuf[symBufIdx].id == lparent) {//��(�������ʽ����)��
        updateSymBuf();
        resTi = expr();
        if (symBuf[symBufIdx].id != rparent)
            error(11);//!Ӧ��)
        else
            updateSymBuf();
    } else {
        error(24);//!�Ƿ�����
        resTi = -1;
    }
    fprintf(fout, "\t\tthis is a factor.\n");
    return resTi;
}

void assignment() {//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
    int resTid, ti1, ti2 = -1;
    int isArr = 0;
    resTid = lookup(symBuf[symBufIdx].token, 0);//0 not func
    if (resTid == -1) {
        strcpy(errPlace, "assign");
        error(17);//!��ʶ��δ����
        return;
    }
    if (tab[resTid].kind == conkind && tab[resTid].kind == funkind) {
        error(32);//!���ܶԳ����ͺ�������ֵ
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id == lbrack) {
        if (tab[resTid].kind != arrkind) {
            error(28);//!��������
        }
        updateSymBuf();//����ֵ��䣾::=����ʶ������[�������ʽ����]��=�����ʽ��
        ti2 = expr();
        isArr = 1;
        if (symBuf[symBufIdx].id != rbrack)
            error(14);//!Ӧ��]
        else
            updateSymBuf();
        if (symBuf[symBufIdx].id != become) {
            error(25);//!�Ƿ����
            return;
        }
        updateSymBuf();
        ti1 = expr();
    } else if (symBuf[symBufIdx].id == become) {//������ֵ����ֵ��䣾   ::=  ����ʶ�����������ʽ��
        if (tab[resTid].kind == arrkind) {
            error(32);//!���ܶ�����ֱ�Ӹ�ֵ
        }
        updateSymBuf();
        ti1 = expr();//!ֱ�ӵ���
    } else {
        error(25);//!�Ƿ����
        return;
    }
    if (resTid >= 0 && ti1 >= 0 && tab[resTid].typ != tab[ti1].typ)
        warn(0);
    emitMid(isArr ? setArrOp : becomeOp, ti1, ti2, resTid, tiarg, isArr ? tiarg : earg, tiarg);
    fprintf(fout, "\t\tthis is a assignment.\n");
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
void ifStat() {//��������䣾::=if ��(������������)������䣾��else����䣾��
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    int brTi = condition();//!ֱ�ӵ���
    int midxElse = midx;
    emitMid(brfOp, brTi, -1, 0, tiarg, earg, liarg);
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();//!read one more sym
    stat();//!ֱ�ӵ���
    int midxNext = midx;
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    mCode[midxElse].res.labIdx = getLab();
    if (symBuf[symBufIdx].id == elsesy) {//!��ѡ��
        updateSymBuf();//!one more sym
        stat();
    }
    mCode[midxNext].res.labIdx = getLab();
    fprintf(fout, "\t\tthis is an if stat.\n");
}

int condition() {//��������::=�����ʽ������ϵ������������ʽ���������ʽ��
    int ti1, ti2 = -1, resTi;
    int hasOp = 0;
    enum MOP op = sltOp;
    ti1 = expr();//!ֱ�ӵ���
    if (symBuf[symBufIdx].id == les) {//!��ѡ��
        hasOp = 1;
        op = sltOp;
        updateSymBuf();
        ti2 = expr();
    } else if (symBuf[symBufIdx].id == leq) {
        hasOp = 1;
        op = sleOp;
        updateSymBuf();
        ti2 = expr();
    } else if (symBuf[symBufIdx].id == gtr) {
        hasOp = 1;
        op = sgtOp;
        updateSymBuf();
        ti2 = expr();
    } else if (symBuf[symBufIdx].id == geq) {
        hasOp = 1;
        op = sgeOp;
        updateSymBuf();
        ti2 = expr();
    } else if (symBuf[symBufIdx].id == eql) {
        hasOp = 1;
        op = seqOp;
        updateSymBuf();
        ti2 = expr();
    } else if (symBuf[symBufIdx].id == neq) {//!©д
        hasOp = 1;
        op = sneOp;
        updateSymBuf();
        ti2 = expr();
    }
    if (hasOp == 1) {
        resTi = getTemVar();
        emitMid(op, ti1, ti2, resTi, tiarg, tiarg, tiarg);
    } else {
        resTi = ti1;
    }
    fprintf(fout, "\t\tthis is a condition.\n");
    return resTi;
}
/*
 *
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
void whileStat() {//��ѭ����䣾::=while ��(������������)������䣾
    int loopLabIdx = getLab();
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    int brTi = condition();//!ֱ�ӵ���
    int loopMidx = midx;
    emitMid(brfOp, brTi, -1, 0, tiarg, earg, liarg);
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();
    stat();//!ֱ�ӵ���
    emitMid(jOp, -1, -1, loopLabIdx, earg, earg, liarg);
    int endLabIdx = getLab();
    mCode[loopMidx].res.labIdx = endLabIdx;
    fprintf(fout, "\t\tthis is a while stat.\n");
}

//printf��ͷ
void writeStat() {//��д��䣾::=printf��(�� ���ַ�����,�����ʽ�� ��)��|printf ��(�����ַ����� ��)��|printf ��(�������ʽ����)��
    char str[STRMAX] = "";
    int expTid = -1, strTid = -1;
    int hasStr = 0, hasExp = 0;
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    if (symBuf[symBufIdx].id == strcon) {
        strcpy(str, symBuf[symBufIdx].token);
        hasStr = 1;
        updateSymBuf();
        if (symBuf[symBufIdx].id == comma) {//!��ѡ��
            updateSymBuf();
            expTid = expr();
            hasExp = 1;
        }
    } else {
        expTid = expr();//!ֱ�ӵ���
        hasExp = 1;
    }
    if (hasStr) {
        strTid = enterStr(str);
    }
    emitMid(writeOp, -1, strTid, expTid, earg, hasStr ? siarg : earg, hasExp ? tiarg : earg);
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();//!one more sym
    fprintf(fout, "\t\tthis is a write stat.\n");
}

//scanf��ͷ
void readStat() {//������䣾::=scanf ��(������ʶ����{,����ʶ����}��)��
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    int ti;
    if (symBuf[symBufIdx].id != ident) {
        strcpy(errPlace, "read");
        error(9);//!Ӧ�Ǳ�ʶ��
    } else {
        ti = lookup(symBuf[symBufIdx].token, 0);//0 not func
        if (ti == -1) {
            strcpy(errPlace, "read");
            error(17);//!��ʶ��δ����
        }
        emitMid(readOp, -1, -1, ti, earg, earg, tiarg);
        updateSymBuf();
    }
    while (symBuf[symBufIdx].id == comma) {//!ѭ����,�ָ�
        updateSymBuf();
        if (symBuf[symBufIdx].id != ident) {
            strcpy(errPlace, "read");
            error(9);//!Ӧ�Ǳ�ʶ��
            continue;
        }
        ti = lookup(symBuf[symBufIdx].token, 0);//0 not func
        if (ti == -1) {
            strcpy(errPlace, "read");
            error(17);//!��ʶ��δ����
        }
        emitMid(readOp, -1, -1, ti, earg, earg, tiarg);
        updateSymBuf();
    }
    if (symBuf[symBufIdx].id != rparent)
        error(11);//!Ӧ��)
    else
        updateSymBuf();

    fprintf(fout, "\t\tthis is a read stat.\n");
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
void switchStat() {//�������䣾  ::=  switch ��(�������ʽ����)�� ��{����������ۣ�ȱʡ���ݡ�}��
    /*case record
    int caseCntCur;
    caseCnt++;
    caseCntCur=caseCnt;
     */
    struct CASTAB casetb;
    casetb.caseCnt = 0;
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparent) {
        error(10);//!Ӧ��(
    } else {
        updateSymBuf();
    }
    int eva = expr();//!ֱ�ӵ���
    int swtMidx = midx;//FOR BACK PATCH goto TEST
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    if (symBuf[symBufIdx].id != rparent) {
        error(11);//!Ӧ��)
        return;
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != lbrace)
        error(12);//!Ӧ��{
    else
        updateSymBuf();
    caseStat(&casetb);//!ֱ�ӵ���
    int hasDefault = 0;
    if (symBuf[symBufIdx].id == defaultsy) {//!first����Ϊ{default}
        hasDefault = 1;
        defaultCase(&casetb);//!��ѡ��
    }
    mCode[swtMidx].res.labIdx = getLab();//gen labTest
    int i;
    for (i = 0; i < casetb.caseCnt - hasDefault; i++) {
        int brTi = getTemVar();
        emitMid(sneOp, casetb.cValue[i], eva, brTi, varg, tiarg, tiarg);
        emitMid(brfOp, brTi, -1, casetb.labIdx[i], tiarg, earg, liarg);
    }
    if (hasDefault == 1) {
        emitMid(jOp, -1, -1, casetb.labIdx[casetb.caseCnt - 1], earg, earg, liarg);
    }
    //gen nextLab
    int nextLabIdx = getLab();
    for (i = 0; i < casetb.caseCnt; i++) {
        mCode[casetb.midx[i]].res.labIdx = nextLabIdx;
    }
    if (symBuf[symBufIdx].id != rbrace)
        error(13);//!Ӧ��}
    else
        updateSymBuf();
    fprintf(fout, "\t\tthis is a switch stat.\n");
}

//case��ͷ
void caseStat(struct CASTAB *casetb) {//�������   ::=  ���������䣾{���������䣾}
    if (symBuf[symBufIdx].id != casesy) {//!����һ��case���
        error(26);//!����һ��case
        return;
    }
    while (symBuf[symBufIdx].id == casesy) {//! first����Ϊ{case}
        oneCase(casetb);
    }
    fprintf(fout, "\t\tthis is a case table.\n");
}

//case��ͷ
void oneCase(struct CASTAB *casetb) {//���������䣾::=case��������������䣾
    if (casetb->caseCnt == CASEMAX) {
        error(27);//!case��ǩ����
        casetb->caseCnt = casetb->caseCnt - 1;//!��һ��case������
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id == charcon) {
        casetb->cValue[casetb->caseCnt] = symBuf[symBufIdx].token[1];
        updateSymBuf();
    } else {
        casetb->cValue[casetb->caseCnt] = numDef(2);
    }
    if (symBuf[symBufIdx].id != colon)
        error(15);//!Ӧ��:
    else
        updateSymBuf();
    casetb->labIdx[casetb->caseCnt] = getLab();
    stat();//!ֱ�ӵ���
    casetb->midx[casetb->caseCnt] = midx;
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    casetb->caseCnt = casetb->caseCnt + 1;
    fprintf(fout, "\t\tthis is a one case.\n");
}

//default��ͷ
void defaultCase(struct CASTAB *casetb) {//��ȱʡ��::=default : ����䣾
    updateSymBuf();
    if (symBuf[symBufIdx].id != colon)
        error(15);//!Ӧ��:
    else
        updateSymBuf();
    casetb->labIdx[casetb->caseCnt] = getLab();
    stat();//!ֱ�ӵ���
    casetb->midx[casetb->caseCnt] = midx;
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    casetb->caseCnt = casetb->caseCnt + 1;
    fprintf(fout, "\t\tthis is a default case.\n");
}

//return��ͷ
void retStat() {//��������䣾::=return[��(�������ʽ����)��]
    int hasRet = 0;
    int expTid = -1;
    btab[btidx - 1].reted = 1;//����з������
    updateSymBuf();
    if (symBuf[symBufIdx].id == lparent) {//!��ѡ��
        if (tab[btab[btidx - 1].tidx].typ == voidtyp)
            error(29);//!ӦΪ�޷���ֵret
        updateSymBuf();
        expTid = expr();//!ֱ�ӵ���
        hasRet = 1;
        if (symBuf[symBufIdx].id == rparent)
            updateSymBuf();
        else
            error(11);//!Ӧ��)
    }
    if (tab[btab[btidx - 1].tidx].typ == inttyp && (hasRet == 0 || tab[expTid].typ != inttyp))
        warn(1);//!ӦΪint����ֵ
    if (tab[btab[btidx - 1].tidx].typ == chtyp && (hasRet == 0 || tab[expTid].typ != chtyp))
        warn(2);//!ӦΪchar����ֵ
    emitMid(retOp, -1, -1, expTid, earg, earg, hasRet ? tiarg : earg);
    fprintf(fout, "\t\tthis is a return stat.\n");
}
