#include "global.h"
#include "syntax.h"


void program() {
    if (symBuf[symBufIdx].id == constsy) {//!头符号为const
        decConst();
    }
    int hasVarDef = 0;
    while ((symBuf[symBufIdx].id == intsy || symBuf[symBufIdx].id == charsy) &&
           symBuf[(symBufIdx + 2) % 3].id != lparentsy) {
        hasVarDef = 1;
        varDef();
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是分号(F)
        } else {
            updateSymBuf();//!read one more sym
        }
        if (symBuf[symBufIdx].id == constsy) {
            warn(4);//可以正确的分配存储，只定为warn
            decConst();
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
        if (symBuf[symBufIdx].id == constsy) {
            error(37);//不能正确分配存储，定为error
            decConst();
        }
        while ((symBuf[symBufIdx].id == intsy || symBuf[symBufIdx].id == charsy) &&
               symBuf[(symBufIdx + 2) % 3].id != lparentsy) {
            error(38);
            varDef();
            if (symBuf[symBufIdx].id != semicolonsy) {
                error(4);//!应是分号(F)
            } else {
                updateSymBuf();//!read one more sym
            }
            if (symBuf[symBufIdx].id == constsy) {
                error(37);
                decConst();
            }
        }
        if (symBuf[symBufIdx].id != voidsy && symBuf[symBufIdx].id != charsy && symBuf[symBufIdx].id != intsy) {
            error(34);
        }
    }
    mainDef();
    fprintf(fout, "\t\tthis is a program.\n\n");
    if (!reachEof()) {
        fprintf(fout, "there should be nothing after main func.\n");
        printf("there should be nothing after main func.\n");
    }
    //printCode();

}

//const 开头
void decConst() {
    updateSymBuf();
    constDef();
    if (symBuf[symBufIdx].id != semicolonsy) {
        error(4);//!应是;(F)
    } else {
        updateSymBuf();
    }
    while (symBuf[symBufIdx].id == constsy) {//! first集合为{const}
        updateSymBuf();
        constDef();
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是;(F)
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
        if (symBuf[symBufIdx].id != identsy) {
            errPlace = 'c';
            error(9);//!应是标识符
            return;
        }
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
        if (symBuf[symBufIdx].id != becomesy) {
            error(18);//!常量必须有初始值
            enter(name, kind, type, 0);
            return;
        }
        updateSymBuf();
        if (symBuf[symBufIdx].id != minussy && symBuf[symBufIdx].id != plussy && symBuf[symBufIdx].id != unsignumsy &&
            symBuf[symBufIdx].id != zerosy) {//!first集合
            error(19);//!int常量的初始值应为整数，char常量的初始值应为字符
            enter(name, kind, type, 0);
            return;
        }
        value = numDef('c');//'c'->pos
        emitMid(conOp, type, value, tabCnt, targ, varg, tiarg);
        enter(name, kind, type, value);
        adrOffset++;
        while (symBuf[symBufIdx].id == commasy) {//可选项
            updateSymBuf();
            if (symBuf[symBufIdx].id != identsy) {
                errPlace = 'c';
                error(9);//!应是标识符
                return;
            }
            strcpy(name, symBuf[symBufIdx].token);
            updateSymBuf();
            if (symBuf[symBufIdx].id != becomesy) {
                enter(name, kind, type, 0);
                error(18);//!常量必须有初始值
                return;
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id != minussy && symBuf[symBufIdx].id != plussy &&
                symBuf[symBufIdx].id != unsignumsy &&
                symBuf[symBufIdx].id != zerosy) {
                error(19);//!int常量的初始值应为整数，char常量的初始值应为字符
                enter(name, kind, type, 0);
                return;
            }
            value = numDef('c');//'c'->pos
            emitMid(conOp, type, value, tabCnt, targ, varg, tiarg);
            enter(name, kind, type, value);
            adrOffset++;
        }
    } else if (symBuf[symBufIdx].id == charsy) {
        type = chtyp;
        updateSymBuf();
        if (symBuf[symBufIdx].id != identsy) {
            errPlace = 'c';
            error(9);//!应是标识符
            return;
        }
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
        if (symBuf[symBufIdx].id != becomesy) {
            enter(name, kind, type, '0');
            error(18);//!常量必须有初始值
            return;
        }
        updateSymBuf();
        if (symBuf[symBufIdx].id != charconsy) {
            error(19);//!int常量的初始值应为整数，char常量的初始值应为字符
            enter(name, kind, type, '0');
            return;
        }
        value = symBuf[symBufIdx].token[1];
        emitMid(conOp, type, value, tabCnt, targ, varg, tiarg);
        enter(name, kind, type, value);
        adrOffset++;
        updateSymBuf();
        while (symBuf[symBufIdx].id == commasy) {//可选项
            updateSymBuf();
            if (symBuf[symBufIdx].id != identsy) {
                errPlace = 'c';
                error(9);//!应是标识符
                return;
            }
            strcpy(name, symBuf[symBufIdx].token);
            updateSymBuf();
            if (symBuf[symBufIdx].id != becomesy) {
                error(18);//!常量必须有初始值
                enter(name, kind, type, '0');
                return;
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id != charconsy) {
                error(19);//!int常量的初始值应为整数，char常量的初始值应为字符
                enter(name, kind, type, '0');
                return;
            }
            value = symBuf[symBufIdx].token[1];
            emitMid(conOp, type, value, tabCnt, targ, varg, tiarg);
            enter(name, kind, type, value);
            adrOffset++;
            updateSymBuf();
        }
    } else {
        error(7);//! 应为int或char 未知的常量类型标识
    }
    fprintf(fout, "\t\tthis is const def.\n");
}

/*
void decVar(){//＜变量说明＞::=＜变量定义＞;{＜变量定义＞;}
}
*/
void varDef() {//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ }
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
    if (symBuf[symBufIdx].id != identsy) {
        errPlace = 'v';
        error(9);//!应是标识符
    } else {
        strcpy(name, symBuf[symBufIdx].token);
        kind = varkind;
        value = 1;
        updateSymBuf();
        if (symBuf[symBufIdx].id == lbracksy) {//!可选项
            updateSymBuf();
            if (symBuf[symBufIdx].id != unsignumsy) {
                error(20);//!数组的大小应为非0无符号整数
                value = 1;
            } else {
                value = atoi(symBuf[symBufIdx].token);
            }
            kind = arrkind;
            updateSymBuf();
            if (symBuf[symBufIdx].id != rbracksy)
                error(14);//!应是]
            else
                updateSymBuf();
        }
        emitMid((kind == arrkind ? arrOp : varOp), type, value, tabCnt, targ, (kind == arrkind ? varg : earg), tiarg);
        enter(name, kind, type, value);
        adrOffset = adrOffset + value;
    }
    if (symBuf[symBufIdx].id != commasy && symBuf[symBufIdx].id != semicolonsy) {
        error(33);//非法的变量定义
    }
    while (symBuf[symBufIdx].id == commasy) {
        updateSymBuf();
        if (symBuf[symBufIdx].id != identsy) {//!不应有类型标识符
            errPlace = 'v';
            error(9);//!应是标识符
            continue;
        }
        strcpy(name, symBuf[symBufIdx].token);
        kind = varkind;
        value = 1;
        updateSymBuf();
        if (symBuf[symBufIdx].id == lbracksy) {//!可选项
            updateSymBuf();
            if (symBuf[symBufIdx].id != unsignumsy) {
                error(20);//!数组的大小应为非0无符号整数
                value = 1;
            } else {
                value = atoi(symBuf[symBufIdx].token);
            }
            kind = arrkind;
            updateSymBuf();
            if (symBuf[symBufIdx].id != rbracksy) {
                error(11);//!应是)
                return;
            }
            updateSymBuf();
        }
        emitMid((kind == arrkind ? arrOp : varOp), type, value, tabCnt, targ, (kind == arrkind ? varg : earg), tiarg);
        enter(name, kind, type, value);
        adrOffset = adrOffset + value;
    }
    fprintf(fout, "\t\tthis is var def.\n");
}

int numDef(char pos) {//＜整数＞::= ［＋｜－］＜无符号整数＞｜０
    //pos 'c': constDef 'f': factor 'o': onecase
    int flag = 0;
    int num = 0;
    if (symBuf[symBufIdx].id == zerosy) {
        updateSymBuf();
        num = 0;
    } else {
        if (symBuf[symBufIdx].id == plussy) {//!可选项
            updateSymBuf();
        } else if (symBuf[symBufIdx].id == minussy) {
            updateSymBuf();
            flag = 1;
        }
        if (symBuf[symBufIdx].id == unsignumsy) {
            num = atoi(symBuf[symBufIdx].token);
            if (flag == 1) {
                num = -num;
            }
            updateSymBuf();
        } else {
            error(21);//!非法的整数
            errPlace = pos;
            return 0;
        }
    }
    fprintf(fout, "\t\tthis is a num:%d.\n", num);
    return num;
}

void retFuncDef() {//＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur, btidCur;
    int mIdxCur;
    kind = funkind;
    tIdxCur = tabCnt;
    btidCur = btabCnt;
    if (symBuf[symBufIdx].id == charsy) {
        type = chtyp;
    } else if (symBuf[symBufIdx].id == intsy) {
        type = inttyp;
    } else {
        error(-1);//! should't happen , run time err
        return;
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != identsy) {
        errPlace = 'f';
        error(9);//!应是标识符
        sprintf(name, "#fun_%d", btabCnt);
    } else {
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
    }
    mIdxCur = mcodeCnt;
    emitMid(funOp, type, 0, btabCnt, targ, varg, btiarg);
    enter(name, kind, type, 0);
    adrOffset = 0;
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    value = paraList();
    tab[tIdxCur].value = value;
    mCode[mIdxCur].arg2.value = value;
    adrOffset = adrOffset + 10;//for $fp $ra and $s0-$s7
    fprintf(fout, "\t\tenter para num for func %s, para num: %d\n", tab[tIdxCur].name, tab[tIdxCur].value);
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();
    if (symBuf[symBufIdx].id != lbracesy)
        error(12);//!应是{
    else
        updateSymBuf();
    complexStat();
    if (symBuf[symBufIdx].id != rbracesy)
        error(13);//!应是}
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

void voidFuncDef() {//＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    int value;
    int tIdxCur, btIdxCur;
    int mIdxCur;
    kind = funkind;
    tIdxCur = tabCnt;
    btIdxCur = btabCnt;
    type = voidtyp;
    updateSymBuf();
    if (symBuf[symBufIdx].id != identsy) {
        errPlace = 'f';
        error(9);//!应是标识符
        sprintf(name, "#func_%d", btabCnt);
    } else {
        strcpy(name, symBuf[symBufIdx].token);
        updateSymBuf();
    }
    mIdxCur = mcodeCnt;
    emitMid(funOp, type, 0, btabCnt, targ, varg, btiarg);
    enter(name, kind, type, 0);
    adrOffset = 0;
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    value = paraList();
    tab[tIdxCur].value = value;
    mCode[mIdxCur].arg2.value = value;
    adrOffset = adrOffset + 10;//!for $fp($sp+16) $ra($sp+20) and $s0-$s7
    fprintf(fout, "\t\tenter para num for func %s, para num: %d\n", tab[tIdxCur].name, tab[tIdxCur].value);
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();
    if (symBuf[symBufIdx].id != lbracesy)
        error(12);//!应是{
    else
        updateSymBuf();
    complexStat();
    if (symBuf[symBufIdx].id != rbracesy)
        error(13);//!应是}
    else
        updateSymBuf();
    emitMid(endFunOp, -1, -1, btIdxCur, earg, earg, btiarg);
    btab[btIdxCur].spacesz = adrOffset;
    btab[btIdxCur].paraN = value;
    fprintf(fout, "\t\tthis is a void func dec.\n");
}

int paraList() {//＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind = parakind;
    int paraCnt = 0;
    if (symBuf[symBufIdx].id == charsy || symBuf[symBufIdx].id == intsy) {//!可以为空
        if (symBuf[symBufIdx].id == charsy) {
            type = chtyp;
        } else {
            type = inttyp;
        }
        updateSymBuf();
        if (symBuf[symBufIdx].id != identsy) {
            errPlace = 'p';
            error(9);//!应是标识符
            sprintf(name, "#p_%d_%d", btabCnt, paraCnt);
        } else {
            strcpy(name, symBuf[symBufIdx].token);
            updateSymBuf();
        }
        emitMid(paraOp, type, -1, tabCnt, targ, earg, tiarg);
        enter(name, kind, type, 0);
        paraCnt = paraCnt + 1;
        adrOffset++;
        while (symBuf[symBufIdx].id == commasy) {
            updateSymBuf();
            if (symBuf[symBufIdx].id != charsy && symBuf[symBufIdx].id != intsy) {
                error(8);//!未知的参数类型标识符
                continue;
            }
            if (symBuf[symBufIdx].id == charsy) {
                type = chtyp;
            } else {
                type = inttyp;
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id != identsy) {
                errPlace = 'p';
                error(9);//!应是标识符
                sprintf(name, "#p_%d_%d", btabCnt, paraCnt);
            } else {
                strcpy(name, symBuf[symBufIdx].token);
                updateSymBuf();
            }
            emitMid(paraOp, type, -1, tabCnt, targ, earg, tiarg);
            enter(name, kind, type, 0);
            adrOffset++;
            paraCnt = paraCnt + 1;
        }
    }
    adrOffset += (4 - paraCnt);
    fprintf(fout, "\t\tthis is para list.\n");
    return paraCnt;
}

void mainDef() {//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    char name[ALENMAX];
    enum KINDS kind;
    enum TYPES type;
    kind = funkind;
    //int tidCur=tidx;
    int btidCur = btabCnt;
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
    emitMid(funOp, type, 0, btabCnt, targ, varg, btiarg);
    enter(name, kind, type, 0);
    adrOffset = 0;
    adrOffset++;//for fp;
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();
    if (symBuf[symBufIdx].id != lbracesy)
        error(12);//!应是{
    else
        updateSymBuf();
    complexStat();
    printSym();
    if (symBuf[symBufIdx].id != rbracesy) {
        error(13);//!应是}
    }
    emitMid(endFunOp, -1, -1, btidCur, earg, earg, btiarg);
    btab[btidCur].spacesz = adrOffset;
    fprintf(fout, "\t\tthis is main func dec.\n");
}

int call(int needRet) {// needRet=0 : stat ; needRet=1 : factor
    int funcId = lookup(symBuf[symBufIdx].token, 1);//可能是-1 checked
    if (funcId == -1) {
        error(16);//!函数未定义 标识符对应多种情况 checked 排除变量 常量 数组和参数
        if (needRet)
            errPlace = 'f';//factor
        else
            errPlace = 's';//stat
        return -1;//checked：stat()调用，但不需要返回值; factor() 调用，需检查
    }
    int funcBtid = 0;
    while (funcBtid < btabCnt && strcmp(btab[funcBtid].name, tab[funcId].name) != 0) {
        funcBtid++;
    }
    if (tab[funcId].typ == voidtyp && needRet == 1) {
        error(25);//!应是有返回值函数 非法语句
        return -1;//checked
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    int resTid = -1;//checked term中使用 stat中不使用
    if (needRet) {
        resTid = getTemVar();
        tab[resTid].typ = tab[funcId].typ;
    }
    //＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
    int paraCnt = valueParaList(funcId);//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
    emitMid(callOp, resTid, paraCnt, funcBtid, needRet == 1 ? tiarg : earg, varg, btiarg);
    if (btab[btabCnt - 1].callParaN < paraCnt)//调整当前函数的值参数个数
        btab[btabCnt - 1].callParaN = paraCnt;
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();
    fprintf(fout, "\t\tthis is a call stat.\n");
    return resTid;
}


int valueParaList(int funcId) {//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
    //funcId 一定大于等于0
    int paraCnt = 0;
    if (symBuf[symBufIdx].id != rparentsy) {//!不为空
        int resTid = expr();//!至少一个 //可能是-1 checked check3
        emitMid(calPaOp, -1, -1, resTid, earg, earg, tiarg);
        paraCnt++;
        if (resTid != -1 && tab[resTid].typ != tab[funcId + paraCnt].typ) {//!check3
            warn(3);//参数类型不正确
        }
        while (symBuf[symBufIdx].id == commasy) {
            updateSymBuf();
            resTid = expr();//可能是-1 checked check4
            emitMid(calPaOp, -1, -1, resTid, earg, earg, tiarg);
            paraCnt++;
            if (resTid != -1 && tab[resTid].typ != tab[funcId + paraCnt].typ) {//check4
                warn(3);//参数类型不正确
            }
        }
    }
    if (tab[funcId].value != paraCnt) {
        error(31);//!参数个数不正确
    }
    fprintf(fout, "\t\tthis is value para list.\n");
    return paraCnt;
}

void complexStat() {//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
    if (symBuf[symBufIdx].id == constsy) {//!头符号为const
        decConst();
    }
    int hasVarDef = 0;
    while (symBuf[symBufIdx].id == charsy || symBuf[symBufIdx].id == intsy) {
        varDef();
        hasVarDef = 1;
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是;(F)
        } else {
            updateSymBuf();
        }
        if (symBuf[symBufIdx].id == constsy) {
            warn(4);
            decConst();
        }
    }
    if (hasVarDef) {
        fprintf(fout, "\t\tthis is dec of var.\n");
    }
    statList();
    fprintf(fout, "\t\tthis is complex stat.\n");
}

void stat(char pos) {
//＜语句＞::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
    if (symBuf[symBufIdx].id == ifsy) {//!头符号集合
        ifStat();
    } else if (symBuf[symBufIdx].id == whilesy) {
        whileStat();
    } else if (symBuf[symBufIdx].id == lbracesy) {//语句列
        updateSymBuf();
        statList();
        if (symBuf[symBufIdx].id != rbracesy)
            error(13);//!应是}
        else
            updateSymBuf();
    } else if (symBuf[symBufIdx].id == scanfsy) {
        readStat();
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是;(F)
        } else {
            updateSymBuf();
        }
    } else if (symBuf[symBufIdx].id == printfsy) {
        writeStat();
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是;(F)
        } else {
            updateSymBuf();
        }
    } else if (symBuf[symBufIdx].id == returnsy) {
        retStat();
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是;(F)
        } else {
            updateSymBuf();
        }
    } else if (symBuf[symBufIdx].id == switchsy) {
        switchStat();
    } else if (symBuf[symBufIdx].id == identsy) {//!first集合相交
        //标识符对应多种情况，checked 转移到调用的函数
        enum SYMBOL nextSym = symBuf[(symBufIdx + 1) % 3].id;
        if (nextSym == becomesy || nextSym == lbracksy) {
            assignment();
        } else if (nextSym == lparentsy) {
            call(0);
        } else {
            error(25);//!非法语句
            return;
        }
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!应是;(F)
        } else {
            updateSymBuf();
        }
    } else {
        if (symBuf[symBufIdx].id != semicolonsy) {
            error(4);//!!应是;(F)
        } else {
            updateSymBuf();
        }
    }//!空
    testAfterStat(pos);
    fprintf(fout, "\t\tthis is a stat.\n");
}

void statList() {
    //!可以没有一条
    while (symBuf[symBufIdx].id == ifsy || symBuf[symBufIdx].id == whilesy || symBuf[symBufIdx].id == lbracesy ||
           symBuf[symBufIdx].id == scanfsy || symBuf[symBufIdx].id == printfsy || symBuf[symBufIdx].id == returnsy ||
           symBuf[symBufIdx].id == switchsy || symBuf[symBufIdx].id == identsy ||
           symBuf[symBufIdx].id == semicolonsy) {//!first集合
        stat('s');//pos is statlist
    }
    fprintf(fout, "\t\tthis is a stat list.\n");
}

int expr() {//＜表达式＞::=［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    int ti1, ti2, resTi, negflag;
    negflag = 0;
    if (symBuf[symBufIdx].id == plussy) {//!可选项
        updateSymBuf();
    } else if (symBuf[symBufIdx].id == minussy) {
        updateSymBuf();
        negflag = 1;
    }
    ti1 = term();//可能是-1 checked 流向emit或返回，检查调用者(checked)
    if (negflag) {
        resTi = getTemVar();
        emitMid(subOp, 0, ti1, resTi, varg, tiarg, tiarg);
    } else {
        resTi = ti1;
    }
    while (symBuf[symBufIdx].id == plussy || symBuf[symBufIdx].id == minussy) {//!以+-分隔
        enum MOP op = symBuf[symBufIdx].id == plussy ? addOp : subOp;
        updateSymBuf();
        ti1 = resTi;
        ti2 = term();//可能是-1 checked 流向emit或返回，检查调用者(checked)
        resTi = getTemVar();
        emitMid(op, ti1, ti2, resTi, tiarg, tiarg, tiarg);
    }
    fprintf(fout, "\t\tthis is an expr.\n");
    return resTi;
}

int term() {//＜项＞::=＜因子＞{＜乘法运算符＞＜因子＞}
    int ti1, ti2, resTi;
    ti1 = factor();//返回值可能是-1, checked 流向emit或返回，检查调用者expr()
    resTi = ti1;
    while (symBuf[symBufIdx].id == mulsy || symBuf[symBufIdx].id == divsy) {
        enum MOP op = symBuf[symBufIdx].id == mulsy ? mulOp : divOp;
        updateSymBuf();
        ti1 = resTi;
        ti2 = factor();//返回值可能是-1 checked 流向emit或返回，检查调用者expr()
        resTi = getTemVar();
        emitMid(op, ti1, ti2, resTi, tiarg, tiarg, tiarg);

    }
    fprintf(fout, "\t\tthis is a term.\n");
    return resTi;
}

int factor() {//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
    int resTi;
    if (symBuf[symBufIdx].id == identsy) {
        enum SYMBOL nextSym = symBuf[(symBufIdx + 1) % 3].id;
        if (nextSym == lparentsy) {
            return call(1);//checked: 此处未使用，检查调用factor的函数term() 必须是函数，checked call内判断
        } else {
            resTi = lookup(symBuf[symBufIdx].token, 0);//标识符, 0 not func 可能是-1 checked
            if (resTi == -1) {
                errPlace = 'f';
                error(17);//!标识符未定义 排除函数
                //return -1; ! 不应return
            }
            updateSymBuf();
            if (symBuf[symBufIdx].id == lbracksy) {//!可选项 ＜标识符＞‘[’＜表达式＞‘]’
                int ti1, ti2;
                if (resTi != -1 && tab[resTi].kind != arrkind) {
                    error(28);//!不为数组
                    //!不应return return -1;
                }
                ti1 = resTi;
                updateSymBuf();
                ti2 = expr();//可能是-1 checked 流向emit
                resTi = getTemVar();
                if (ti1 != -1) {//!类型设为数组元素的类型
                    tab[resTi].typ = tab[ti1].typ;
                }
                emitMid(getArrOp, ti1, ti2, resTi, tiarg, tiarg, tiarg);
                if (symBuf[symBufIdx].id != rbracksy)
                    error(14);//!应是]
                else
                    updateSymBuf();
            } else {//标识符对应多种情况 checked 已经排除函数，需要排除数组
                if (resTi != -1 && tab[resTi].kind == arrkind) {
                    error(17);//!不应为数组 标识符种类不正确
                }
            }
        }
    } else if (symBuf[symBufIdx].id == charconsy) {
        resTi = getTemVar();
        tab[resTi].typ = chtyp;
        emitMid(liop, symBuf[symBufIdx].token[1], -1, resTi, varg, earg, tiarg);
        updateSymBuf();
    } else if (symBuf[symBufIdx].id == plussy || symBuf[symBufIdx].id == minussy || symBuf[symBufIdx].id == zerosy ||
               symBuf[symBufIdx].id == unsignumsy) {//!整数 first 集合
        resTi = getTemVar();
        emitMid(liop, numDef('f'), -1, resTi, varg, earg, tiarg);//'f'->pos
    } else if (symBuf[symBufIdx].id == lparentsy) {//‘(’＜表达式＞‘)’
        updateSymBuf();
        resTi = expr();//可能是-1 checked 检查调用者term()
        if (symBuf[symBufIdx].id != rparentsy)
            error(11);//!应是)
        else
            updateSymBuf();
    } else {
        error(24);//!非法因子
        resTi = -1;//checked: 此处未使用，检查调用factor()的函数term()
    }
    fprintf(fout, "\t\tthis is a factor.\n");
    return resTi;
}

void assignment() {//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
    int resTid, ti1, ti2 = -1;
    int isArr = 0;
    resTid = lookup(symBuf[symBufIdx].token, 0);//0 not func 可能是-1,checked (排除函数)
    //标识符对应多种情况 checked 排除常量 数组和函数
    if (resTid == -1) {
        errPlace = 'a';
        error(17);//!标识符未定义
        return;
    }
    if (tab[resTid].kind == conkind && tab[resTid].kind == funkind) {
        error(32);//!不能对常量和函数名赋值 (排除常量)
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id == lbracksy) {
        if (tab[resTid].kind != arrkind) {
            error(28);//!不是数组
        }
        updateSymBuf();//＜赋值语句＞::=＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
        ti2 = expr();//可能是-1 checked 流向emit
        isArr = 1;
        if (symBuf[symBufIdx].id != rbracksy)
            error(14);//!应是]
        else
            updateSymBuf();
        if (symBuf[symBufIdx].id != becomesy) {
            error(25);//!非法语句
            return;
        }
        updateSymBuf();
        ti1 = expr();//可能是-1 checked 下文check1
    } else if (symBuf[symBufIdx].id == becomesy) {//变量赋值＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
        if (tab[resTid].kind == arrkind) {
            error(32);//!不能对数组直接赋值 (排除数组)
        }
        updateSymBuf();
        ti1 = expr();//!直接调用 //可能是-1 checked 下文check1
    } else {
        error(25);//!非法语句
        return;
    }
    if (resTid != -1 && ti1 != -1 && tab[resTid].typ != tab[ti1].typ)//check1
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


//if开头
void ifStat() {//＜条件语句＞::=if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    int brTi = condition();//!直接调用 //可能是-1 checked emit
    int midxElse = mcodeCnt;
    emitMid(brfOp, brTi, -1, 0, tiarg, earg, liarg);
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();//!read one more sym
    stat('i');//!直接调用 pos if
    int midxNext = mcodeCnt;
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    mCode[midxElse].res.labIdx = getLab();
    if (symBuf[symBufIdx].id == elsesy) {//!可选项
        updateSymBuf();//!one more sym
        stat('e');//pos else
    }
    mCode[midxNext].res.labIdx = getLab();
    fprintf(fout, "\t\tthis is an if stat.\n");
}

int condition() {//＜条件＞::=＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
    int ti1, ti2 = -1, resTi;
    int hasOp = 0;
    enum MOP op = sltOp;
    ti1 = expr();//!直接调用 可能是-1 checked, 流向emit或返回
    if (symBuf[symBufIdx].id == lessy) {//!可选项
        hasOp = 1;
        op = sltOp;
        updateSymBuf();
        ti2 = expr();//可能是-1 checked 流向emit
    } else if (symBuf[symBufIdx].id == leqsy) {
        hasOp = 1;
        op = sleOp;
        updateSymBuf();
        ti2 = expr();//可能是-1 checked 流向emit
    } else if (symBuf[symBufIdx].id == gtrsy) {
        hasOp = 1;
        op = sgtOp;
        updateSymBuf();
        ti2 = expr();//可能是-1 checked 流向emit
    } else if (symBuf[symBufIdx].id == geqsy) {
        hasOp = 1;
        op = sgeOp;
        updateSymBuf();
        ti2 = expr();//可能是-1 checked 流向emit
    } else if (symBuf[symBufIdx].id == eqlsy) {
        hasOp = 1;
        op = seqOp;
        updateSymBuf();
        ti2 = expr();//可能是-1 checked 流向emit
    } else if (symBuf[symBufIdx].id == neqsy) {//!漏写
        hasOp = 1;
        op = sneOp;
        updateSymBuf();
        ti2 = expr();//可能是-1 checked 流向emit
    }
    if (hasOp == 1) {
        resTi = getTemVar();
        emitMid(op, ti1, ti2, resTi, tiarg, tiarg, tiarg);
    } else {
        resTi = ti1;//可能是-1 checked 返回值，检查调用者 if和while
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
//while开头
void whileStat() {//＜循环语句＞::=while ‘(’＜条件＞‘)’＜语句＞
    int loopLabIdx = getLab();
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    int brTi = condition();//!直接调用 //可能是-1 checked emit
    int loopMidx = mcodeCnt;
    emitMid(brfOp, brTi, -1, 0, tiarg, earg, liarg);
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();
    stat('w');//!直接调用 pos while
    emitMid(jOp, -1, -1, loopLabIdx, earg, earg, liarg);
    int endLabIdx = getLab();
    mCode[loopMidx].res.labIdx = endLabIdx;
    fprintf(fout, "\t\tthis is a while stat.\n");
}

//printf开头
void writeStat() {//＜写语句＞::=printf‘(’ ＜字符串＞,＜表达式＞ ‘)’|printf ‘(’＜字符串＞ ‘)’|printf ‘(’＜表达式＞‘)’
    char str[STRMAX] = "";
    int expTid = -1, strTid = -1;
    int hasStr = 0, hasExp = 0;
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    if (symBuf[symBufIdx].id == strconsy) {
        strcpy(str, symBuf[symBufIdx].token);
        hasStr = 1;
        updateSymBuf();
        if (symBuf[symBufIdx].id == commasy) {//!可选项
            updateSymBuf();
            expTid = expr();//可能是-1 checked emit
            hasExp = 1;
        }
    } else {
        expTid = expr();//!直接调用 //可能是-1 checked emit
        hasExp = 1;
    }
    if (hasStr) {
        strTid = enterStr(str);
    }
    emitMid(writeOp, -1, strTid, expTid, earg, hasStr ? siarg : earg, hasExp ? tiarg : earg);
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
    else
        updateSymBuf();//!one more sym
    fprintf(fout, "\t\tthis is a write stat.\n");
}

//scanf开头
void readStat() {//＜读语句＞::=scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    int ti;
    if (symBuf[symBufIdx].id != identsy) {//标识符对应多种情况,此处排除函数，需要排除常量和数组
        errPlace = 'r';
        error(9);//!应是标识符
    } else {
        ti = lookup(symBuf[symBufIdx].token, 0);//0 not func 可能是-1, checked 排除函数
        if (ti == -1) {
            errPlace = 'r';
            error(17);//!标识符未定义
        }
        if (ti != -1 && (tab[ti].kind == arrkind || tab[ti].kind == conkind)) {//排除常量和数组
            error(17);//!标识符种类不正确
        }
        emitMid(readOp, -1, -1, ti, earg, earg, tiarg);
        updateSymBuf();
    }
    while (symBuf[symBufIdx].id == commasy) {//!循环以,分隔
        updateSymBuf();
        if (symBuf[symBufIdx].id != identsy) {
            errPlace = 'r';
            error(9);//!应是标识符
            continue;
        }
        ti = lookup(symBuf[symBufIdx].token, 0);//0 not func 可能是-1, checeked //标识符对应多种情况,此处排除函数，需要排除常量和数组
        if (ti == -1) {
            errPlace = 'r';
            error(17);//!标识符未定义
        }
        if (ti != -1 && (tab[ti].kind == arrkind || tab[ti].kind == conkind)) {//排除函数和数组
            error(17);//!标识符种类不正确
        }
        emitMid(readOp, -1, -1, ti, earg, earg, tiarg);
        updateSymBuf();
    }
    if (symBuf[symBufIdx].id != rparentsy)
        error(11);//!应是)
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
//switch开头
void switchStat() {//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞［＜缺省＞］‘}’
    /*case record
    int caseCntCur;
    caseCnt++;
    caseCntCur=caseCnt;
     */
    struct CASTAB casetb;
    casetb.caseCnt = 0;
    updateSymBuf();
    if (symBuf[symBufIdx].id != lparentsy) {
        error(10);//!应是(
    } else {
        updateSymBuf();
    }
    int eva = expr();//!直接调用 //可能是-1 checked 流向emit
    int swtMidx = mcodeCnt;//FOR BACK PATCH goto TEST
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    if (symBuf[symBufIdx].id != rparentsy) {
        error(11);//!应是)
        return;
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id != lbracesy)
        error(12);//!应是{
    else
        updateSymBuf();
    caseStat(&casetb);//!直接调用
    int hasDefault = 0;
    if (symBuf[symBufIdx].id == defaultsy) {//!first集合为{default}
        hasDefault = 1;
        defaultCase(&casetb);//!可选项
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
    if (symBuf[symBufIdx].id != rbracesy)
        error(13);//!应是}
    else
        updateSymBuf();
    fprintf(fout, "\t\tthis is a switch stat.\n");
}

//case开头
void caseStat(struct CASTAB *casetb) {//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    if (symBuf[symBufIdx].id != casesy) {//!至少一个case语句
        error(26);//!至少一个case
        return;
    }
    while (symBuf[symBufIdx].id == casesy) {//! first集合为{case}
        oneCase(casetb);
    }
    fprintf(fout, "\t\tthis is a case table.\n");
}

//case开头
void oneCase(struct CASTAB *casetb) {//＜情况子语句＞::=case＜常量＞：＜语句＞
    if (casetb->caseCnt == CASEMAX) {
        error(27);//!case标签过多
        casetb->caseCnt = casetb->caseCnt - 1;//!上一个case不保存
    }
    updateSymBuf();
    if (symBuf[symBufIdx].id == charconsy) {
        casetb->cValue[casetb->caseCnt] = symBuf[symBufIdx].token[1];
        updateSymBuf();
    } else {
        casetb->cValue[casetb->caseCnt] = numDef('o');//'o'->pos
    }
    if (symBuf[symBufIdx].id != colonsy)
        error(15);//!应是:
    else
        updateSymBuf();
    casetb->labIdx[casetb->caseCnt] = getLab();
    stat('c');//!直接调用 pos case
    casetb->midx[casetb->caseCnt] = mcodeCnt;
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    casetb->caseCnt = casetb->caseCnt + 1;
    fprintf(fout, "\t\tthis is a one case.\n");
}

//default开头
void defaultCase(struct CASTAB *casetb) {//＜缺省＞::=default : ＜语句＞
    updateSymBuf();
    if (symBuf[symBufIdx].id != colonsy)
        error(15);//!应是:
    else
        updateSymBuf();
    casetb->labIdx[casetb->caseCnt] = getLab();
    stat('d');//!直接调用 pos default
    casetb->midx[casetb->caseCnt] = mcodeCnt;
    emitMid(jOp, -1, -1, 0, earg, earg, liarg);
    casetb->caseCnt = casetb->caseCnt + 1;
    fprintf(fout, "\t\tthis is a default case.\n");
}

//return开头
void retStat() {//＜返回语句＞::=return[‘(’＜表达式＞‘)’]
    int hasRet = 0;
    int expTid = -1;
    btab[btabCnt - 1].reted = 1;//标记有返回语句
    updateSymBuf();
    if (symBuf[symBufIdx].id == lparentsy) {//!可选项
        if (tab[btab[btabCnt - 1].tidx].typ == voidtyp)
            error(29);//!应为无返回值ret
        updateSymBuf();
        expTid = expr();//!直接调用 //可能是-1 checked 下文check2
        hasRet = 1;
        if (symBuf[symBufIdx].id == rparentsy)
            updateSymBuf();
        else
            error(11);//!应是)
    }
    if (tab[btab[btabCnt - 1].tidx].typ == inttyp &&
        (hasRet == 0 || (expTid != -1 && tab[expTid].typ != inttyp)))//check2
        warn(1);//!应为int返回值
    if (tab[btab[btabCnt - 1].tidx].typ == chtyp && (hasRet == 0 || (expTid != -1 && tab[expTid].typ != chtyp)))//check2
        warn(2);//!应为char返回值
    emitMid(retOp, -1, -1, expTid, earg, earg, hasRet ? tiarg : earg);
    fprintf(fout, "\t\tthis is a return stat.\n");
}
