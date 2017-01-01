//
// Created by tony on 2016/12/27.
//
#include "global.h"
#include "opt.h"
#include <algorithm>
#include <map>
#include <vector>

//con[opt]  type    value   tid
//cvar      type    _       tid
//arr       type    sz      tid

//jmp       _       _       labidx
//gen       _       _       labidx
//brf       con     _       labidx  (con:v,ti)

//fun       btid    paCnt   type                                para tid,_,type        endFun _,_,btid
//ret       ret     _       btid    (ret:v,ti)
//cal       btid    paCnt   des
//calPa     pa      _       _       (pa:v,ti)

//become    src     _       des     (src:v,ti)
//li[opt]   val     _       des
//math      arg1    arg2    des     (arg1:v,ti arg2:v,ti)       slt,sle,seq,sne,sgt,sge,add,sub,mul,div
//=[]       arr     idx     des     (idx:v,ti)
//[]=       src     idx     arr     (src:v,ti idx:v,ti)

//rd        _       _       tid
//wr        [exp]   [sidx]  [typ]   (exp:v,ti)

#define BLKMAX 1024
#define DAGMAX 512
#define SETMAX 128
#define TREGNUM 8
#define SREGNUM 8
extern char mopStr[][10];

void opt() {
#ifndef OPT
        return;
#endif
    optLab();//删除多余标签
    optJmp();//删除多余跳转

    divdBlk();//划分基本块
    dataflow();//建立数据流图

    optExp();//优化表达式，合并常数 todo check
    dag();//删除公共子表达式
    delDeadCode();//删除死代码
    globalReg();
}

BLOCK block[BLKMAX];
int blkCnt = 0;

void optCode(int codeIdx) {
    mCode[codeIdx].op = optedOp;
    mCode[codeIdx].rTyp = earg;
    mCode[codeIdx].arg1Typ = earg;
    mCode[codeIdx].arg2Typ = earg;
}

int findInBlk(int codeIdx) {
    int i;
    for (i = 0; i < blkCnt; i++) {
        if (block[i].end > codeIdx)
            break;
    }
    return i;
}
void divdBlk() {
    int i = 0;
    while (i < mcodeCnt && mCode[i].op != funOp)
        i++;
    block[blkCnt].begin = i;
    blkCnt++;
    i++;
    for (; i < mcodeCnt; i++) {
        enum MOP op = mCode[i].op;
        if (i + 1 < mcodeCnt &&
            (op == genOp || op == jOp || op == brfOp || op == retOp || op == endFunOp)) {
            block[blkCnt].begin = i + 1;
            block[blkCnt - 1].end = i + 1;
            if (op == retOp || op == endFunOp) {
                block[blkCnt - 1].next1 = -1;
                block[blkCnt - 1].next2 = -1;
            } else if (op == brfOp) {
                block[blkCnt - 1].next1 = blkCnt;
                //block[blkCnt-1].next2=findInBlk(lab[mCode[i].res.labIdx]);
            } else if (op == jOp) {
                //block[blkCnt-1].next1=findInBlk(lab[mCode[i].res.labIdx]);
                block[blkCnt - 1].next2 = -1;
            } else {
                block[blkCnt - 1].next1 = blkCnt;
                block[blkCnt - 1].next2 = -1;
            }
            blkCnt++;
        }
    }
    block[blkCnt - 1].end = mcodeCnt;
    for (int j = 0; j < blkCnt; j++) {
        int codeIdx = block[j].end - 1;
        if (mCode[codeIdx].op == brfOp) {
            block[j].next2 = findInBlk(lab[mCode[codeIdx].res.labIdx]);
        } else if (mCode[codeIdx].op == jOp) {
            block[j].next1 = findInBlk(lab[mCode[codeIdx].res.labIdx]);
        }
    }
}
void dataflow() {
    for (int i = 0; i < blkCnt; i++) {
        //初始化use和def
        for (int j = block[i].begin; j < block[i].end; j++) {
            if (mCode[j].op == optedOp)
                continue;
            int arg1 = mCode[j].arg1Typ == tiarg ? mCode[j].arg1.tidx : -1;
            int arg2 = mCode[j].arg2Typ == tiarg ? mCode[j].arg2.tidx : -1;
            int res = mCode[j].rTyp == tiarg ? mCode[j].res.tidx : -1;
            arg1 = arg1 < btab[0].tidx ? -1 : arg1;
            arg2 = arg2 < btab[0].tidx ? -1 : arg2;
            res = res < btab[0].tidx ? -1 : res;
            if (arg1 != -1 && block[i].use.find(arg1) == block[i].use.end() &&
                block[i].def.find(arg1) == block[i].def.end()) {
                block[i].use.insert(arg1);
            }
            if (arg2 != -1 && block[i].use.find(arg2) == block[i].use.end() &&
                block[i].def.find(arg2) == block[i].def.end()) {
                block[i].use.insert(arg2);
            }
            if (res != -1 && block[i].use.find(res) == block[i].use.end() &&
                block[i].def.find(res) == block[i].def.end()) {
                block[i].def.insert(res);
            }
        }
    }
    int flag;
    do {
        flag = 0;
        for (int i = blkCnt - 1; i >= 0; i--) {
            int next1 = block[i].next1;
            int next2 = block[i].next2;
            unsigned long long oldOutSize = block[i].out.size();
            if (next1 != -1) {
                block[i].out.insert(block[next1].in.begin(), block[next1].in.end());
            }
            if (next2 != -1) {
                block[i].out.insert(block[next2].in.begin(), block[next2].in.end());
            }
            if (oldOutSize != block[i].out.size())
                flag = 1;
            //in=use V (out-def)
            block[i].in.clear();
            block[i].in.insert(block[i].use.begin(), block[i].use.end());
            set<int> dif;
            set_difference(block[i].out.begin(), block[i].out.end(), block[i].def.begin(), block[i].def.end(),
                           inserter(dif, dif.begin()));
            block[i].in.insert(dif.begin(), dif.end());
        }
    } while (flag);
    for (int i = 0; i < blkCnt; i++) {
        fprintf(fout, "blk %d,begin %d,end %d,next1 %d,next2 %d\n",
                i, block[i].begin, block[i].end, block[i].next1, block[i].next2);
        fprintf(fout, "in: \n");
        for (set<int>::iterator si = block[i].in.begin(); si != block[i].in.end(); si++) {
            fprintf(fout, "%s ", tab[*si].name);
        }
        fprintf(fout, "\nout:\n");
        for (set<int>::iterator si = block[i].out.begin(); si != block[i].out.end(); si++) {
            fprintf(fout, "%s ", tab[*si].name);
        }
        fprintf(fout, "\nuse:\n");
        for (set<int>::iterator si = block[i].use.begin(); si != block[i].use.end(); si++) {
            fprintf(fout, "%s ", tab[*si].name);
        }
        fprintf(fout, "\ndef:\n");
        for (set<int>::iterator si = block[i].def.begin(); si != block[i].def.end(); si++) {
            fprintf(fout, "%s ", tab[*si].name);
        }
        fprintf(fout, "\n");
    }
}

void optLab() {
    for (int i = 0; i < mcodeCnt - 1; i++) {//0-midx-2 第二条到倒数第二条
        if (mCode[i].op == genOp) {//总是保留第一条gen语句
            int labId = mCode[i].res.labIdx;
            int j = i + 1;
            while (mCode[j].op == genOp) {
                optCode(j);
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
            optCode(i);
        }
    }
}


void delConst(int tid, int value) {
    for (int i = 0; i < mcodeCnt; i++) {
        if (mCode[i].op == optedOp)
            continue;
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
        if (op == optedOp)
            continue;
        if (mCode[i].op == liop) {
            optCode(i);
            delConst(mCode[i].res.tidx, mCode[i].arg1.value);
            continue;
        }
        if (mCode[i].op == conOp) {//const,_,v,tid
            optCode(i);
            delConst(mCode[i].res.tidx, mCode[i].arg2.value);
            continue;
        }
        if (mCode[i].arg1Typ == varg && mCode[i].arg2Typ == varg) {
            if (op == addOp || op == subOp || op == mulOp || op == divOp
                || op == sltOp || op == sleOp || op == seqOp || op == sneOp || op == sgtOp || op == sgeOp) {
                mCode[i].arg1.value = calExp(i);
                mCode[i].op = becomeOp;
                mCode[i].arg1Typ = varg;
                mCode[i].arg2Typ = earg;
            }
        }
        //优化x=常数
        if (mCode[i].op == becomeOp && mCode[i].arg1Typ == varg) {
//            mCode[i].op=optedOp;可能跨越基本块，不能直接删掉
            passConst(i);
            if (mCode[i].res.tidx >= btab[0].tidx && canDelTemVar(i)) {//例子：return(1+2+x());
                //mCode[i].op = optedOp;//临时变量不能跨越基本块，可以在满足情况的条件下删掉
                optCode(i);
            }
        }
        //优化x=x
        if (mCode[i].op == becomeOp && mCode[i].arg1Typ == tiarg && mCode[i].arg1.tidx == mCode[i].res.tidx) {
            optCode(i);
        }
        //优化tx=x y=tx
        if (mCode[i].op == becomeOp && mCode[i].arg1Typ == tiarg &&
            mCode[i - 1].rTyp == tiarg && mCode[i - 1].res.tidx == mCode[i].arg1.tidx
            && mCode[i].arg1.tidx >= btab[0].tidx) {//全局变量就作改变
            mCode[i].arg1Typ = earg;
            if (canDelTemVar(i - 1)) {
                optCode(i);
                mCode[i - 1].res.tidx = mCode[i].res.tidx;
            } else {
                mCode[i].arg1Typ = tiarg;
            }
        }
    }
}

//此函数用于尝试对临时变量的进行赋值(改变其值)的四元式进行优化
int canDelTemVar(int codeIdx) {//todo check
    int tid = mCode[codeIdx].res.tidx;
    int used = 0;
//    int isTem=tab[tid].name[0]=='&';
    for (int i = codeIdx + 1; i < mcodeCnt; i++) {//应从下一条语开始
        if (mCode[i].op == optedOp)
            continue;
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
            int blk = findInBlk(codeIdx);
            if (block[blk].out.find(tid) == block[blk].out.end())
                used = 0;
            else
                used = 1;
//            used = 0;函数调用不会改变临时变量的值，除非返回值是这个临时变量，但是会被上面一个if检测到
            break;//!这种情况按理不会出现
            //! 临时变量不能跨越基本块，局部变量可以
        }
    }
    return (!used);
}

void passConst(int codeIdx) {//不跨越基本块，函数可能改变全局变量
    int value = mCode[codeIdx].arg1.value;
    int tid = mCode[codeIdx].res.tidx;
//    int isTempVar=tab[tid].name[0]=='&';
    int isGlobal = tid < btab[0].tidx;
    for (int i = codeIdx + 1; i < mcodeCnt; i++) {
        if (mCode[i].op == optedOp)
            continue;
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
            fprintf(fout, "dag %d,sel:%d,op:%s,left:%d,right:%d\n", k, dagTab[k].sel, mopStr[dagTab[k].val],
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
        printDag();
    }
}

void delDeadCode() {
    //删除死代码
    for (int k = 0; k < mcodeCnt; k++) {
        if (mCode[k].op == varOp || mCode[k].op == arrOp || mCode[k].op == optedOp)
            continue;
        if (mCode[k].rTyp == tiarg && mCode[k].res.tidx >= btab[0].tidx && canDelTemVar(k))
            optCode(k);
    }
}

struct CmpByValue {
    bool operator()(const pair<int, int> &lhs, const pair<int, int> &rhs) {
        return lhs.second < rhs.second;
    }
};

void globalReg() {
    int codeIdx = 0;
    for (int i = 0; i < btabCnt; i++) {
        while (mCode[codeIdx].op != funOp)
            codeIdx++;
        int beginCode = codeIdx;
        while (mCode[codeIdx].op != endFunOp)
            codeIdx++;
        int endCode = codeIdx;
        int beginBlk = findInBlk(beginCode);
        int endBlk = findInBlk(endCode);
        map<int, int> outMap;
        for (int j = beginBlk; j < endBlk; j++) {
            for (set<int>::iterator it = block[j].out.begin(); it != block[j].out.end(); ++it)
                outMap[(*it)]++;
        }
        btab[i].glbReg = (int) outMap.size() < SREGNUM ? (int) outMap.size() : SREGNUM;
        vector<pair<int, int> > counts(outMap.begin(), outMap.end());
        sort(counts.begin(), counts.end(), CmpByValue());
        for (int j = 0; j < btab[i].glbReg; j++) {
            tab[counts[j].first].regIdx = TREGNUM - 1 + j;//todo check
        }
        //输出
        fprintf(fout, "func:%s size:%d\n", btab[i].name, (int) outMap.size());
        for (map<int, int>::iterator iter = outMap.begin(); iter != outMap.end(); ++iter) {
            fprintf(fout, "%s:%d ", tab[iter->first].name, iter->second);
        }
        fprintf(fout, "\n");

    }
}
