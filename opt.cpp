//
// Created by tony on 2016/12/27.
//
#include "global.h"
#include "opt.h"

//t1 opt jmp
//t2 opt lab
//t3 opt const
//t3.1 li
//t3.2 con
//t3.3 exp
//t3.4 调整头文件和global
//t3.3.1 calPa
//t3.3.2 call
//t3.3.3 ret
//t3.3.4 fun endfun para
//t3.3.5 wr
//t3.3.6 输出调整
//t3.5 passconst
//t3.6 become &
//cpp cpp
//opt opt.txt become &
//dag1 dag
//t3.7 x=y z=x
//dag2 =dag1
//dag3 dag.txt
//dag4 dag.txt optdag
//con[opt]  type    value   tid
//cvar      type    _       tid
//arr       type    sz      tid

//jmp       _       _       labidx
//gen       _       _       labidx
//brf       con     _       labidx  (con:v,ti)

//fun       btid    paCnt   type                                para tid,_,type        endFun btid,_,_
//ret       ret     _       _       (ret:v,ti)
//cal       btid    paCnt   des
//calPa     pa      _       _       (pa:v,ti)

//become    src     _       des     (src:v,ti)
//li[opt]   val     _       des
//math      arg1    arg2    des     (arg1:v,ti arg2:v,ti)       slt,sle,seq,sne,sgt,sge,add,sub,mul,div
//=[]       arr     idx     des     (idx:v,ti)
//[]=       src     idx     arr     (src:v,ti idx:v,ti)

//rd        _       _       tid
//wr        [exp]   [sidx]  [typ]   (exp:v,ti)

#define BLKMAX 512
#define DAGMAX 512
#define CODEMAX 4096
int block[BLKMAX];
void opt() {
    if (!OPT) {
        return;
    }
    optLab();
    optJmp();
    optConst();//常量替换

    divdBlk();
    //dataflow();

    optExp();//优化表达式，合并常数 todo check
    dag();
}

void divdBlk() {
    int cnt = 0;
    int i = 0;
    while (i < mcodeCnt && mCode[i].op != funOp)
        i++;
    block[cnt] = i;
    cnt++;
    i++;
    for (; i < mcodeCnt; i++) {
        enum MOP op = mCode[i].op;
        if (i + 1 < mcodeCnt &&
            (op == genOp || op == jOp || op == brfOp || op == retOp || op == endFunOp || op == callOp)) {
            block[cnt] = i + 1;
            cnt++;
        }
    }
    for (int j = 0; j < cnt; j++) {
        fprintf(fout, "blk %d: code %d\n", j, block[j]);
    }
}

void optLab() {
    for (int i = 0; i < mcodeCnt - 1; i++) {//0-midx-2 第二条到倒数第二条
        if (mCode[i].op == genOp) {
            int labId = mCode[i].res.labIdx;
            int j = i + 1;
            while (mCode[j].op == genOp) {
                mCode[j].op = optedOp;
                delLab(mCode[j].res.labIdx, labId);
                j++;
            }
        }
    }
}

void delLab(int toDel, int replaceLabIdx) {
    for (int i = 0; i < mcodeCnt; i++) {
        if (mCode[i].rTyp == liarg && mCode[i].res.labIdx == toDel) {
            mCode[i].res.labIdx = replaceLabIdx;
        }
    }
}

void optJmp() {
    for (int i = 1; i < mcodeCnt - 1; i++) {//0-midx-2 第一条到倒数第二条
        if (mCode[i].op != jOp && mCode[i].op != brfOp)
            continue;
        if (mCode[i + 1].op != genOp && mCode[i - 1].op != genOp)
            continue;
        int labIdx = mCode[i].res.labIdx;
        if (labIdx == mCode[i + 1].res.labIdx || labIdx == mCode[i - 1].res.labIdx) {
            mCode[i].op = optedOp;
        }
    }
}

void optConst() {
    for (int i = 0; i < mcodeCnt; i++) {//li,v,_,des
        if (mCode[i].op == liop) {
            mCode[i].op = optedOp;
            delConst(mCode[i].res.tidx, mCode[i].arg1.value);
        }
        if (mCode[i].op == conOp) {//const,_,v,tid
            mCode[i].op = optedOp;
            delConst(mCode[i].res.tidx, mCode[i].arg2.value);
        }
    }
}

void delConst(int tid, int value) {
    for (int i = 0; i < mcodeCnt; i++) {
        if (mCode[i].arg1Typ == tiarg && mCode[i].arg1.tidx == tid) {
            mCode[i].arg1Typ = varg;
            mCode[i].arg1.value = value;
            if (mCode[i].op == writeOp) {
                mCode[i].rTyp = targ;
                mCode[i].res.typ = tab[tid].typ;
            }
        }
        if (mCode[i].arg2Typ == tiarg && mCode[i].arg2.tidx == tid) {
            mCode[i].arg2Typ = varg;
            mCode[i].arg2.value = value;
        }
    }
    //更改tab中的地址偏移
    for (int i = tid + 1; i < tabCnt && tab[i].kind != funkind; i++) {
        tab[i].adr -= 1;
    }
    if (tid >= btab[0].tidx) {//不是全局变量
        for (int i = 0; i < btabCnt; i++) {
            if (tid < btab[i].tidx) {
                btab[i - 1].spacesz -= 1;
                break;
            }
        }
    }
}

void optExp() {
    for (int i = 0; i < mcodeCnt; i++) {
        enum MOP op = mCode[i].op;
        if (mCode[i].arg1Typ == varg && mCode[i].arg2Typ == varg) {
            if (op == addOp || op == subOp || op == mulOp || op == divOp
                || op == sltOp || op == sleOp || op == seqOp || op == sneOp || op == sgtOp || op == sgeOp) {
                mCode[i].arg1.value = calExp(i);
                mCode[i].op = becomeOp;
                mCode[i].arg1Typ = varg;
                mCode[i].arg2Typ = earg;
            }
        }
        if (mCode[i].op == becomeOp && mCode[i].arg1Typ == varg) {
//            mCode[i].op=optedOp;可能跨越基本块，不能直接删掉
            passConst(i);
            if (tab[mCode[i].res.tidx].name[0] == '&' && canDelTemVar(i)) {//例子：return(1+2+x());
                mCode[i].op = optedOp;//临时变量不能跨越基本块，可以在满足情况的条件下删掉
            }
        }
        if (mCode[i].op == becomeOp && mCode[i].arg1Typ == tiarg && mCode[i].arg1.tidx == mCode[i].res.tidx) {
            mCode[i].op = optedOp;
        }
        if (mCode[i].op == becomeOp && mCode[i].arg1Typ == tiarg &&
            mCode[i - 1].rTyp == tiarg && mCode[i - 1].res.tidx == mCode[i].arg1.tidx
            && tab[mCode[i].arg1.tidx].name[0] == '&') {
            mCode[i].arg1Typ = earg;
            if (canDelTemVar(i - 1)) {
                mCode[i].op = optedOp;
                mCode[i - 1].res.tidx = mCode[i].res.tidx;
            }
            mCode[i].arg1Typ = tiarg;
        }
    }
}

//此函数用于尝试对临时变量的进行赋值(改变其值)的四元式进行优化
int canDelTemVar(int codeIdx) {//todo check
    int tid = mCode[codeIdx].res.tidx;
    int used = 0;
    for (int i = codeIdx + 1; i < mcodeCnt; i++) {//应从下一条语开始
        if (mCode[i].arg1Typ == tiarg && mCode[i].arg1.tidx == tid) {
            used = 1;
            break;
        }
        if (mCode[i].arg2Typ == tiarg && mCode[i].arg2.tidx == tid) {
            used = 1;
            break;
        }
        if (mCode[i].rTyp == tiarg && mCode[i].res.tidx == tid) {//上一个定义点被kill
            used = 0;
            break;
        }
        enum MOP op = mCode[i].op;
        if (op == genOp || op == jOp || op == brfOp || op == endFunOp || op == retOp) {//函数调用不用判定
            used = 0;//函数调用不会改变临时变量的值，除非返回值是这个临时变量，但是会被上面一个if检测到
            break;//!这种情况按理不会出现
            //! 临时变量不能跨越基本块，局部变量可以
        }
    }
    return (!used);
}

void passConst(int codeIdx) {//没有数据流图，不能跨越基本块，函数可能改变一些变量(仅限全局)
    int value = mCode[codeIdx].arg1.value;
    int tid = mCode[codeIdx].res.tidx;
//    int isTempVar=tab[tid].name[0]=='&';
    int isGlobal = tid < btab[0].tidx;
    for (int i = codeIdx + 1; i < mcodeCnt; i++) {
        if (mCode[i].arg1Typ == tiarg && mCode[i].arg1.tidx == tid) {
            mCode[i].arg1Typ = varg;
            mCode[i].arg1.value = value;
        }
        if (mCode[i].arg2Typ == tiarg && mCode[i].arg2.tidx == tid) {
            mCode[i].arg2Typ = varg;
            mCode[i].arg2.value = value;
        }
        if (mCode[i].rTyp == tiarg && mCode[i].res.tidx == tid)//到达下一个定义点，不能再进行值传递
            break;
        enum MOP op = mCode[i].op;
        if (op == brfOp || op == jOp || op == genOp || op == endFunOp || op == retOp)//不能跨越基本块
            break;
        if (isGlobal && op == callOp)//临时变量可以跨越函数调用 todo check
            break;//全局变量不能跨越函数调用
    }
}

int calExp(int codeIdx) {
    enum MOP op = mCode[codeIdx].op;
    int arg1 = mCode[codeIdx].arg1.value;
    int arg2 = mCode[codeIdx].arg2.value;
    if (arg2 == 0 && op == divOp) {
        printf("fetal error: div 0\n");
        endProc(-1);
    }
    switch (op) {
        case addOp:
            return arg1 + arg2;
        case subOp:
            return arg1 - arg2;
        case mulOp:
            return arg1 * arg2;
        case divOp:
            return arg1 / arg2;
        case sltOp:
            return arg1 < arg2;
        case sleOp:
            return arg1 <= arg2;
        case seqOp:
            return arg1 == arg2;
        case sneOp:
            return arg1 != arg2;
        case sgtOp:
            return arg1 > arg2;
        case sgeOp:
            return arg1 >= arg2;
        default:
            return 0;
    }

}

int isMathOp(enum MOP op) {
    return (op == addOp || op == subOp || op == mulOp || op == divOp || op == becomeOp
            || op == sltOp || op == sleOp || op == sneOp || op == seqOp || op == sgtOp || op == sgeOp);
}

struct {
    int sel;
    int val;//sel,tid|value|op,left,right
    int left;//dagIdx
    int right;
} dagTab[DAGMAX];
struct {//sel,tid|value,dagIdx
    int sel;
    int val;
    int dagIdx;
} nodeTab[DAGMAX];
int dagCnt = 0;
int nodeCnt = 0;

int findInNodeTab(int sel, int val) {//return node Idx
    for (int i = 0; i < nodeCnt; i++) {
        if (nodeTab[i].sel == sel && nodeTab[i].val == val)
            return i;
    }
    return -1;
}

int findInDagTab(enum MOP op, int left, int right) {
    for (int i = 0; i < dagCnt; i++) {
        if (dagTab[i].sel == 2 && dagTab[i].val == op && dagTab[i].left == left && dagTab[i].right == right)
            return i;
    }
    return -1;
}

char moppStr[][10] = {"conOp", "varOp", "funOp", "arrOp", "paraOp", "retOp", "endFunOp", "callOp", "calPaOp", "readOp",
                      "writeOp",
                      "jOp", "brfOp", "sltOp", "sleOp", "sgtOp", "sgeOp", "seqOp", "sneOp",
                      "liop", "addOp", "subOp", "mulOp", "divOp", "setArrOp", "getArrOp", "becomeOp", "genOp",
                      "optedOp"};

void printDag() {
    for (int k = 0; k < nodeCnt; k++) {
        int sel = nodeTab[k].sel;
        if (sel == 0) {
            fprintf(fout, "node %d,sel:%d,ti:%s,dag:%d\n", k, nodeTab[k].sel, tab[nodeTab[k].val].name,
                    nodeTab[k].dagIdx);
        } else if (sel == 1) {
            fprintf(fout, "node %d,sel:%d,val:%d,dag:%d\n", k, nodeTab[k].sel, nodeTab[k].val, nodeTab[k].dagIdx);
        }
    }
    for (int k = 0; k < dagCnt; k++) {
        int sel = dagTab[k].sel;
        if (sel == 0) {
            fprintf(fout, "dag %d,sel:%d,ti:%s,left:%d,right:%d\n", k, dagTab[k].sel, tab[dagTab[k].val].name,
                    dagTab[k].left, dagTab[k].right);
        } else if (sel == 1) {
            fprintf(fout, "dag %d,sel:%d,val:%d,left:%d,right:%d\n", k, dagTab[k].sel, dagTab[k].val, dagTab[k].left,
                    dagTab[k].right);
        } else if (sel == 2) {
            fprintf(fout, "dag %d,sel:%d,op:%s,left:%d,right:%d\n", k, dagTab[k].sel, moppStr[dagTab[k].val],
                    dagTab[k].left, dagTab[k].right);
        }

    }
    nodeCnt = 0;
    dagCnt = 0;
}

int findFisrtNode(int dagIdx) {
    for (int i = 0; i < nodeCnt; i++) {
        if (nodeTab[i].dagIdx == dagIdx)
            return i;
    }
    return -1;
}

void dag() {//math become
    for (int i = 0; i < mcodeCnt; i++) {
        enum MOP op = mCode[i].op;
        if (!isMathOp(op)) {
            continue;
        }
        if (i + 1 < mcodeCnt && !isMathOp(mCode[i + 1].op)) {
            continue;
        }
        int begin = i;
        for (int j = i; j < mcodeCnt; j++) {
            enum MOP mop = mCode[j].op;
            if (!isMathOp(mop)) {
                i = j - 1;
                break;
            }
            int arg1Sel = mCode[j].arg1Typ == varg;
            int arg1 = mCode[j].arg1.value;
            int arg2Sel = mCode[j].arg2Typ == varg ? 1 : (mCode[j].arg2Typ == earg ? -1 : 0);
            int arg2 = mCode[j].arg2.value;
            int res = mCode[j].res.tidx;
            int arg1Node = findInNodeTab(arg1Sel, arg1);
            if (arg1Node == -1) {
                arg1Node = nodeCnt;
                nodeTab[nodeCnt++] = {arg1Sel, arg1, dagCnt};
                dagTab[dagCnt++] = {arg1Sel, arg1, -1, -1};
            }
            int arg2Node = -1;
            if (mop != becomeOp) {
                arg2Node = findInNodeTab(arg2Sel, arg2);
                if (arg2Node == -1) {
                    arg2Node = nodeCnt;
                    nodeTab[nodeCnt++] = {arg2Sel, arg2, dagCnt};
                    dagTab[dagCnt++] = {arg2Sel, arg2, -1, -1};
                }
            }
            int resNode = findInNodeTab(0, res);
            if (resNode == -1) {
                resNode = nodeCnt;
                nodeTab[nodeCnt] = {0, res, -1};
                nodeCnt++;
            }
            if (mop == becomeOp) {
                nodeTab[resNode].dagIdx = nodeTab[arg1Node].dagIdx;
                continue;
            }
            int resDag = findInDagTab(mop, nodeTab[arg1Node].dagIdx, nodeTab[arg2Node].dagIdx);
            if (resDag == -1) {
                nodeTab[resNode].dagIdx = dagCnt;
                dagTab[dagCnt] = {2, mop, nodeTab[arg1Node].dagIdx, nodeTab[arg2Node].dagIdx};
                dagCnt++;
                //没有找到公共表达式，尝试将操作数优化
                int newArg1Node = findFisrtNode(nodeTab[arg1Node].dagIdx);//一定不为-1，至少可以找到arg1Node
                int newArg2Node = findFisrtNode(nodeTab[arg2Node].dagIdx);
                mCode[j].arg1Typ = nodeTab[newArg1Node].sel == 0 ? tiarg : varg;
                mCode[j].arg1.value = nodeTab[newArg1Node].val;
                mCode[j].arg2Typ = nodeTab[newArg2Node].sel == 0 ? tiarg : varg;
                mCode[j].arg2.value = nodeTab[newArg2Node].val;
            } else {
                nodeTab[resNode].dagIdx = resDag;
                int newSrcNode = findFisrtNode(resDag);//找到了公共表达式，转换为赋值
                if (newSrcNode != -1) {//todo 可能为-1吗？
                    mCode[j].op = becomeOp;
                    mCode[j].arg2Typ = earg;
                    mCode[j].arg1.value = nodeTab[newSrcNode].val;
                    mCode[j].arg1Typ = nodeTab[newSrcNode].sel == 0 ? tiarg : varg;
                }
            }
        }
        int end = i;
        fprintf(fout, "begin code: %d\n", begin);
        fprintf(fout, "end code %d\n", end);
        for (int k = begin; k < end; k++) {
            if (tab[mCode[k].res.tidx].name[0] == '&' && canDelTemVar(k))
                mCode[k].op = optedOp;
        }
        printDag();
    }
}