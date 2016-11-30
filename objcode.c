//
// Created by tony on 2016/11/27.
//
#include "global.h"
#include "objcode.h"
//mCode[];
//midx;
#define TREGNUM 8
#define PAMAX 300
int mIdxCur=0;
int btidCur=-1;
struct treg{
    int used;
    int lastUsed;
    int tidx[TREGNUM];
    int dif[TREGNUM];
    int busy[TREGNUM];
}tReg;
char calopStr[][4]={"slt","sle","sgt","sge","seq","sne","add","sub","mul","div"};

struct {
    int cnt;
    union{
        int tidx;
        int value;
    }para[PAMAX];
    //int isVal;
}paraQue;


void generate(){
    int i;
    tReg.used=0;
    tReg.lastUsed=TREGNUM-1;
    paraQue.cnt=0;
    for(i=0;i<TREGNUM;i++){
        tReg.busy[i]=0;
        tReg.tidx[i]=-1;
        tReg.dif[i]=0;
    }
    //test(0,1,2,3,4);
    storeGlobal();
    while(mIdxCur<midx){
        //printf("%d\n",mIdxCur);
        switch (mCode[mIdxCur].op){
            case conOp:
                conToObj();
                mIdxCur++;
                break;
            case varOp:
                varToObj();
                mIdxCur++;
                break;
            case funOp:
                funToObj();
                mIdxCur++;
                break;
            case paraOp:mIdxCur++;break;
            case arrOp:
                arrToObj();
                mIdxCur++;
                break;
            case retOp:
                retToObj();
                mIdxCur++;
                break;
            case endFunOp:
                endFunToObj();
                mIdxCur++;
                break;
            case callOp:
                callToObj();
                mIdxCur++;
                break;
            case calPaOp:
                calPaToObj();
                mIdxCur++;
                break;
            case readOp:
                rdToObj();
                mIdxCur++;
                break;
            case writeOp:
                wrToObj();
                mIdxCur++;
                break;
            case jOp:
                jmpToObj();
                mIdxCur++;
                break;
            case brfOp:
                brfToObj();
                mIdxCur++;
                break;
            case sltOp:
                mathToObj(0);//sltToObj();
                mIdxCur++;
                break;
            case sleOp:
                mathToObj(1);//sleToObj();
                mIdxCur++;
                break;
            case sgtOp:
                mathToObj(2);//sgtToObj();
                mIdxCur++;
                break;
            case sgeOp:
                mathToObj(3);//sgeToObj();
                mIdxCur++;
                break;
            case seqOp:
                mathToObj(4);//seqToObj();
                mIdxCur++;
                break;
            case sneOp:
                mathToObj(5);//sneToObj();
                mIdxCur++;
                break;
            case liop:
                liToObj();
                mIdxCur++;
                break;
            case addOp:
                mathToObj(6);//addToObj();
                mIdxCur++;
                break;
            case subOp:
                mathToObj(7);//subToObj();
                mIdxCur++;
                break;
            case mulOp:
                mathToObj(8);//mulToObj();
                mIdxCur++;
                break;
            case divOp:
                mathToObj(9);//divToObj();
                mIdxCur++;
                break;
            case setArrOp:
                setArrToObj();
                mIdxCur++;
                break;
            case getArrOp:
                getArrToObj();
                mIdxCur++;
                break;
            case becomeOp:
                bcomToObj();//become,x1,_,x2    x2=x1;
                mIdxCur++;
                break;
            case genOp:
                genToObj();//gen,_,_,labidx
                mIdxCur++;
                break;
        }
    }
    fprintf(codefile,"end:\n");
}

int isGlobal(int tidx){
    return tidx<btab[0].tidx?1:0;
}

void freeTemReg(int i){//todo check
    if(tReg.tidx[i]==-1 || tReg.dif[i]==0) {// wrong!! tab[tReg.tidx[i]].kind!=varkind
        //-1代表为程序中的立即数；不为变量不需要写回
    }else if(isGlobal(tReg.tidx[i])){
        fprintf(codefile,"sw $t%d,glb_%s\n",i,tab[tReg.tidx[i]].name);
    }else{
        fprintf(codefile,"sw $t%d,%d($fp) # sw %s\n",i,(tab[tReg.tidx[i]].adr)*4,tab[tReg.tidx[i]].name);
    }
    tReg.dif[i]=0;
    tReg.tidx[i]=-1;
    tReg.busy[i]=0;
}

void clearTemReg(){
    int i;
    tReg.used=0;
    tReg.lastUsed=TREGNUM-1;
    for(i=0;i<TREGNUM;i++){
        freeTemReg(i);
    }
}

void loadToReg(int tid,int reg){
    int i;
    if(isGlobal(tid)){
        fprintf(codefile,"lw $t%d,glb_%s\n",reg,tab[tid].name);
    }else{
        fprintf(codefile,"lw $t%d,%d($fp) # load %s\n",reg,(tab[tid].adr)*4,tab[tid].name);
    }
//    printf("*********used %d**************\n",tReg.used);
//    for(i=0;i<8;i++){
//        printf("treg %d: busy:%d,tidx:%d,dirty:%d",i,tReg.busy[i],tReg.tidx[i],tReg.dif[i]);
//        if(tReg.tidx[i]>0){
//            printf(",name:%s",tab[tReg.tidx[i]].name);
//        }
//        printf("\n");
//    }
}

int findInTemReg(int tid){
    int i;
    for(i=TREGNUM-1;i>=0 && tReg.tidx[i]!=tid;i--)
        ;
    printf("\t\t\tfind %s, res:%d\n",tab[tid].name,i);
    return i;
}
int getEmpTemReg(int tid,int regToUse1,int regToUse2){
    int res,i;
    if(tReg.used<TREGNUM){
        for(res=0;res<TREGNUM;res++) {
            if(tReg.busy[res]==0){
                break;
            }else{
                printf("reg :%d can't use.\n", res);
            }
        }
        tReg.used=tReg.used+1;
    }else{
        res=tReg.lastUsed;
        do{
            res=(res+1)%TREGNUM;
            //printf("loop2：i:%d,use1:%d,use2:%d\n",i,regToUse1,regToUse2);
        }while (res==regToUse1 || res==regToUse2);
        if(tReg.dif[res]==1){
            freeTemReg(res);
        }
    }
    tReg.busy[res]=1;
    tReg.dif[res]=0;
    tReg.tidx[res]=tid;
    tReg.lastUsed=res;
//    printf("\t\t\tget reg %d for %s\n",res,tab[tid].name);
//    printf("*********used %d**************\n",tReg.used);
//    for(i=0;i<8;i++){
//        printf("treg %d: busy:%d,tidx:%d,dirty:%d",i,tReg.busy[i],tReg.tidx[i],tReg.dif[i]);
//        if(tReg.tidx[i]>0){
//            printf(",name:%s",tab[tReg.tidx[i]].name);
//        }
//        printf("\n");
//    }
    return res;
}

/*




    int i=-1;
    if(tidx!=-1){

    }
    if(i==-1){
        i= askForReg(regToUse1, regToUse2);
        tReg.tidx[i]=tidx;
    }
    return i;

 */

void jmpToObj(){
    clearTemReg();
    fprintf(codefile,"j label_%d#%d\n",mCode[mIdxCur].res.labIdx,mIdxCur);
    fprintf(codefile,"nop\n");
}

void genToObj(){
    clearTemReg();
    fprintf(codefile,"label_%d:#%d\n",mCode[mIdxCur].res.labIdx,mIdxCur);
}

void bcomToObj(){//todo maybe imediate num
    int des=mCode[mIdxCur].res.tidx;
    int desReg= findInTemReg(des);
    if(desReg==-1){
        desReg=getEmpTemReg(des,-1,-1);
    }
    tReg.dif[desReg]=1;
    if(mCode[mIdxCur].arg1Typ==varg){
        fprintf(codefile,"addi $t%d,$0,%d #code %d# %s=%d\n",
                desReg,mCode[mIdxCur].arg1.value,mIdxCur,tab[des].name,mCode[mIdxCur].arg1.value);
    } else{
        int sc=mCode[mIdxCur].arg1.tidx;
        int scReg= findInTemReg(sc);
        if(scReg==-1){
            scReg=getEmpTemReg(sc,desReg,-1);
            loadToReg(sc,scReg);
        }
        fprintf(codefile,"add $t%d,$0,$t%d #code %d %s=%s\n",
                desReg,scReg,mIdxCur,tab[des].name,tab[sc].name);
    }
}

void brfToObj(){
    if(mCode[mIdxCur].arg1Typ==varg){
        if(mCode[mIdxCur].arg1.value==0){
            jmpToObj();
        } else{
            fprintf(codefile,"#no use br here.code %d\n",mIdxCur);
        }
    } else{
        int arg1=mCode[mIdxCur].arg1.tidx;
        int arg1Reg= findInTemReg(arg1);
        if(arg1Reg==-1){
            arg1Reg=getEmpTemReg(arg1, -1, -1);
            loadToReg(arg1,arg1Reg);
        }
        clearTemReg();
        fprintf(codefile,"beq $t%d,$0,label_%d #code %d\n",arg1Reg,mCode[mIdxCur].res.labIdx,mIdxCur);
        fprintf(codefile,"nop\n");
    }
}

void liToObj(){
    int value=mCode[mIdxCur].arg1.value;
    int des=mCode[mIdxCur].res.tidx;
    int regDes=findInTemReg(des);
    if(regDes==-1){
        regDes=getEmpTemReg(des,-1,-1);
    }
    tReg.dif[regDes]=1;
    fprintf(codefile,"li $t%d,%d#li %s\n",regDes,value,tab[des].name);
}

void mathToObj(int op){
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= findInTemReg(code.res.tidx);
    if(regDes==-1){
        regDes=getEmpTemReg(code.res.tidx,-1,-1);
    }
    tReg.dif[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getEmpTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d#src1\n",regSrc1,code.arg1.value);
    }else{
        regSrc1=findInTemReg(code.arg1.tidx);
        if(regSrc1==-1){
            regSrc1=getEmpTemReg(code.arg1.tidx, regDes, -1);
            loadToReg(code.arg1.tidx,regSrc1);
        }
    }
    if(code.arg2Typ==varg && op!=0){//not slt
        fprintf(codefile,"%s $t%d,$t%d,%d# des: %s\n",
                calopStr[op],regDes,regSrc1,code.arg2.value,tab[code.res.tidx]);
    }else if(code.arg2Typ==varg && op==0){
        regSrc2= getEmpTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
        fprintf(codefile,"slt $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
        freeTemReg(regSrc2);
    } else{
        regSrc2=findInTemReg(code.arg2.tidx);
        if(regSrc2==-1){
            regSrc2=getEmpTemReg(code.arg2.tidx, regDes, regSrc1);
            loadToReg(code.arg2.tidx,regSrc2);
        }
        fprintf(codefile,"%s $t%d,$t%d,$t%d\n",calopStr[op],regDes,regSrc1,regSrc2);
    }
    if(code.arg1Typ==varg){
        freeTemReg(regSrc1);
    }
}

/*
void sltToObj(){//todo
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        regSrc2= getTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
    }
    fprintf(codefile,"slt $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
    if(code.arg2Typ==varg){
        leaTemReg(regSrc2);
    }

}
void sleToObj(){//todo
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        regSrc2= getTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
    }
    fprintf(codefile,"sle $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
    if(code.arg2Typ==varg){
        leaTemReg(regSrc2);
    }
}
void sgtToObj(){//todo
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        regSrc2= getTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
    }
    fprintf(codefile,"sgt $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
    if(code.arg2Typ==varg){
        leaTemReg(regSrc2);
    }
}
void sgeToObj(){//todo
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        regSrc2= getTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
    }
    fprintf(codefile,"sge $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
    if(code.arg2Typ==varg){
        leaTemReg(regSrc2);
    }
}
void seqToObj(){//todo
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        regSrc2= getTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
    }
    fprintf(codefile,"seq $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
    if(code.arg2Typ==varg){
        leaTemReg(regSrc2);
    }
}
void sneToObj(){//todo
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        regSrc2= getTemReg(-1,regDes, regSrc1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc2,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
    }
    fprintf(codefile,"sne $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
    if(code.arg2Typ==varg){
        leaTemReg(regSrc2);
    }
}
 */
void conToObj(){//con,type,value,name
    int tid=mCode[mIdxCur].res.tidx;
    int reg= getEmpTemReg(tid,-1, -1);
    //tReg.dirty[reg]=1;//运行栈中该数字的
    fprintf(codefile,"addi $t%d,$0,%d  #code %d\n",reg,mCode[mIdxCur].arg2.value,mIdxCur);
    fprintf(codefile,"sw $t%d,%d($fp) #const %s code %d\n",reg,tab[tid].adr*4,tab[tid].name,mIdxCur);
}

void varToObj(){//var,type,_,name
    //int reg= getEmpTemReg(mCode[mIdxCur].res.tidx,-1, -1);
    fprintf(codefile,"# var %s code %d\n",tab[mCode[mIdxCur].res.tidx].name,mIdxCur);
}

void arrToObj(){//arr,type,size,name;
    //int reg= getEmpTemReg(mCode[mIdxCur].res.tidx,-1, -1);
    fprintf(codefile,"# arr %s \n",tab[mCode[mIdxCur].res.tidx].name);
}

void storeGlobal(){//todo 向常量赋值
    int i,limit=btab[0].tidx;
    fprintf(codefile,".data:\n");
    for(i=0;i<limit;i++){//const var arr (func para)
        switch (tab[i].kind){
            case conkind:
                if(tab[i].typ==inttyp){
                    fprintf(codefile,"glb_%s: .word %d\n",tab[i].name,tab[i].value);
                }else{
                    fprintf(codefile,"glb_%s: .word \'%c\'\n",tab[i].name,tab[i].value);
                }
                break;
            case varkind:
                fprintf(codefile,"glb_%s: .word\n",tab[i].name);
                break;
            case arrkind:
                fprintf(codefile,"glb_%s: .space %d\n",tab[i].name,tab[i].value*4);
                break;
            default:
                break;
        }
    }
    fprintf(codefile,"#str\n");
    for(i=0;i<strCnt;i++){
        fprintf(codefile,"str_%d: .asciiz %s\n",i,strtab[i]);
    }
    mIdxCur=btab[0].tidx;
    fprintf(codefile,".text:\n");
    fprintf(codefile,"j func_main\n");
    fprintf(codefile,"nop\n");
}

void rdToObj(){//todo read 内标识符的检验
    int tid=mCode[mIdxCur].res.tidx;
    int v0=tab[tid].typ==inttyp?5:12;
    int reg= findInTemReg(tid);
    if(reg==-1){
        reg=getEmpTemReg(tid,-1,-1);
    }
    tReg.dif[reg]=1;
    fprintf(codefile,"addi $v0,$0,%d\n",v0);
    fprintf(codefile,"syscall#read %d\n",mIdxCur);
    fprintf(codefile,"add $t%d,$0,$v0\n",reg);
}

void wrToObj(){
    int hasStr=mCode[mIdxCur].arg2Typ!=earg?1:0;
    int hasExp=mCode[mIdxCur].rTyp!=earg?1:0;
    if(hasStr){
        fprintf(codefile,"la $a0,str_%d\n",mCode[mIdxCur].arg2.stridx);
        fprintf(codefile,"addi $v0,$0,4\n");
        fprintf(codefile,"syscall\n");
    }
    if(hasExp && mCode[mIdxCur].rTyp==tiarg){
        int tid=mCode[mIdxCur].res.tidx;
        int reg= findInTemReg(tid);
        if(reg==-1){
            reg=getEmpTemReg(tid,-1,-1);
            loadToReg(tid,reg);
        }
        fprintf(codefile,"add $a0,$0,$t%d\n",reg);
        if(tab[tid].typ==chtyp){
            fprintf(codefile,"addi $v0,$0,11\n");
            fprintf(codefile,"syscall\n");
        } else{
            fprintf(codefile,"addi $v0,$0,1\n");
            fprintf(codefile,"syscall\n");
        }
    }//todo check avaliable
}
/*
void addToObj(){
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        fprintf(codefile,"add $t%d,$t%d,%d\n",regDes,regSrc1,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
        fprintf(codefile,"add $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    }
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
}

void subToObj(){
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        fprintf(codefile,"sub $t%d,$t%d,%d\n",regDes,regSrc1,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
        fprintf(codefile,"sub $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    }
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
}

void mulToObj(){
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        fprintf(codefile,"mul $t%d,$t%d,%d\n",regDes,regSrc1,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
        fprintf(codefile,"mul $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    }
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
}

void divToObj(){
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regSrc1=-1,regSrc2=-1;
    regDes= getTemReg(code.res.tidx, -1, -1);
    tReg.dirty[regDes]=1;
    if(code.arg1Typ==varg){
        regSrc1= getTemReg(-1,regDes, -1);
        fprintf(codefile,"addi $t%d,$0,%d\n",regSrc1,code.arg1.value);
    }else{
        regSrc1= getTemReg(code.arg1.tidx, regDes, -1);
        loadToReg(code.arg1.tidx,regSrc1);
    }
    if(code.arg2Typ==varg){
        fprintf(codefile,"div $t%d,$t%d,%d\n",regDes,regSrc1,code.arg2.value);
    }else{
        regSrc2= getTemReg(code.arg2.tidx, regDes, regSrc1);
        loadToReg(code.arg2.tidx,regSrc2);
        fprintf(codefile,"div $t%d,$t%d,$t%d\n",regDes,regSrc1,regSrc2);
    }
    if(code.arg1Typ==varg){
        leaTemReg(regSrc1);
    }
}
*/
void getArrToObj(){//=[],arr,idx,des
    struct MIDCODE code=mCode[mIdxCur];
    int regDes=-1,regArr=-1,regIdx=-1;
    int arrTid=code.arg1.tidx;
    regDes= findInTemReg(code.res.tidx);
    if(regDes==-1){
        regDes=getEmpTemReg(code.res.tidx,-1,-1);
    }
    tReg.dif[regDes]=1;
    regArr=findInTemReg(arrTid);
    if(regArr==-1){
        regArr=getEmpTemReg(arrTid, regDes, -1);
        if(arrTid<btab[0].tidx){
            fprintf(codefile,"la $t%d,glb_%s\n",regArr,tab[arrTid].name);
        }else{
            fprintf(codefile,"addi $t%d,$fp,%d #base adr of arr %s \n",regArr,tab[arrTid].adr*4,tab[arrTid].name);
        }
    }
    if(code.arg2Typ==varg){
        fprintf(codefile,"lw $t%d,%d($t%d)\n",regDes,code.arg2.value*4,regArr);
    } else{
        regIdx= findInTemReg(code.arg2.tidx);
        if(regIdx==-1){
            regIdx=getEmpTemReg(-1, regDes, regArr);
            loadToReg(code.arg2.tidx,regIdx);
            freeTemReg(regIdx);//若是之前不在寄存器中，提前释放
        }
        fprintf(codefile,"sll $at,$t%d,2\n",regIdx);
        fprintf(codefile,"add $at,$at,$t%d\n",regArr);
        fprintf(codefile,"lw $t%d,0($at)\n",regDes);
        freeTemReg(regIdx);
    }
}

void setArrToObj(){//[]=,src,idx,arr
    struct MIDCODE code=mCode[mIdxCur];
    int regSrc=-1,regArr=-1,regIdx=-1;
    int arrtid=code.res.tidx;
    if(code.arg1Typ==varg){
        regSrc=getEmpTemReg(-1,-1,-1);//to be free
        fprintf(codefile,"addi $t%d,$0,%d#set arr: src\n",regSrc,code.arg1.value);
    }else{
        regSrc=findInTemReg(code.arg1.tidx);
        if(regSrc==-1){
            regSrc=getEmpTemReg(code.arg1.tidx, -1, -1);
            loadToReg(code.arg1.tidx,regSrc);
        }
    }
    regArr=findInTemReg(arrtid);
    if(regArr==-1){
        regArr= getEmpTemReg(arrtid, regSrc, -1);
        if(arrtid<btab[0].tidx){
            fprintf(codefile,"la $t%d,glb_%s\n",regArr,tab[arrtid].name);
        }else{
            fprintf(codefile,"addi $t%d,$fp,%d #base adr of arr %s \n",regArr,tab[arrtid].adr*4,tab[arrtid].name);
        }
    }
    if(code.arg2Typ==varg){
        fprintf(codefile,"sw $t%d,%d($t%d)\n",regSrc,code.arg2.value*4,regArr);
    } else{
        regIdx= findInTemReg(code.arg2.tidx);
        if(regIdx==-1){
            regIdx=getEmpTemReg(-1, regSrc, regArr);
            loadToReg(code.arg2.tidx,regIdx);
            freeTemReg(regIdx);//若是之前不在寄存器中，提前释放
        }
        fprintf(codefile,"sll $at,$t%d,2 # cal arr offset\n",regIdx);
        fprintf(codefile,"add $at,$at,$t%d #cal adr\n",regArr);
        fprintf(codefile,"sw $t%d,($at) # set %s\n",
                regSrc,tab[arrtid].name);
        //freeTemReg(temReg);
    }
    if(code.arg1Typ==varg){
        freeTemReg(regSrc);
    }
}
void funToObj(){
    int paraN=mCode[mIdxCur].arg2.value;
    btidCur=mCode[mIdxCur].res.btid;
    fprintf(codefile,"func_%s:\n",btab[btidCur].name);
    if(strcmp(btab[btidCur].name,"main")==0){
        fprintf(codefile,"addi,$sp,$sp,-%d\n",(btab[btidCur].spacesz)*4);
        fprintf(codefile,"add $fp,$0,$sp\n");
    }else{
        if(paraN<=4){
            fprintf(codefile,"sw $ra,20($fp)\n");
        }else{
            fprintf(codefile,"sw $ra,%d($fp)\n",(paraN+1)*4);
        }
    }
}

void endFunToObj(){
    int paraN=btab[btidCur].paraN;
    if(strcmp(btab[btidCur].name,"main")==0){
        fprintf(codefile,"addi,$sp,$sp,%d\n",(btab[btidCur].spacesz)*4);
        //fprintf(codefile,"j end\n");
        return;
    }
    clearTemReg();
    fprintf(codefile,"end_func_%s:\n",btab[btidCur].name);
    fprintf(codefile,"add $sp,$0,$fp\n");
    if(paraN<=4){
        fprintf(codefile,"lw $ra,20($fp)\n");
        fprintf(codefile,"lw $fp,16($fp)\n");
    }else{
        fprintf(codefile,"lw $ra,%d($fp)\n",(paraN+1)*4);
        fprintf(codefile,"lw $fp,%d($fp)\n",(paraN)*4);
    }
    fprintf(codefile,"addi,$sp,$sp,%d\n",(btab[btidCur].spacesz)*4);
    fprintf(codefile,"jr $ra\n");
    fprintf(codefile,"nop\n");
}

void retToObj(){
    if(strcmp(btab[btidCur].name,"main")==0){
        fprintf(codefile,"j end\n");
        return;
    }
    if(mCode[mIdxCur].rTyp==earg){
        clearTemReg();
        fprintf(codefile,"j end_func_%s\n",btab[btidCur].name);
        fprintf(codefile,"nop\n");
    } /*else if(mCode[mIdxCur].rTyp==varg){
        fprintf(codefile,"addi $v0,$0,%d\n",mCode[mIdxCur].res.);
        fprintf(codefile,"j end_func_%s\n",tab[funcTidCur].name);
    } */else if(mCode[mIdxCur].rTyp==tiarg){
        int tid=mCode[mIdxCur].res.tidx;
        int resReg=findInTemReg(tid);
        if(resReg==-1){
            resReg=getEmpTemReg(tid,-1,-1);
            loadToReg(tid,resReg);
        }
        fprintf(codefile,"add $v0,$0,$t%d\n",resReg);
        clearTemReg();
        fprintf(codefile,"j end_func_%s\n",btab[btidCur].name);
        fprintf(codefile,"nop\n");
    }
}


void callToObj(){//call,ret,paraN,func
    int funcBtid=mCode[mIdxCur].res.btid;
    int hasRet=mCode[mIdxCur].arg2Typ!=earg?1:0;
    int retTid=hasRet==1?mCode[mIdxCur].arg1.tidx:0;
    int calparaN=mCode[mIdxCur].arg2.value;
    int paraN=btab[funcBtid].paraN;
    int swFlag[]={0,0,0,0};
    int i,j;
    //todo 检查参数个数和类型
    fprintf(codefile,"addi,$sp,$sp,-%d\n",(btab[funcBtid].spacesz)*4);
    if(paraN!=0 && calparaN!=0){
        int regPara,vparaTid;
//        i=0;
//        while(i<paraN && i<4 && i<btab[btidCur].paraN){
//            fprintf(codefile,"sw $a%d,%d($fp)\n",i,i*4);
//            swFlag[i]=1;
//            i++;
//        }
        i=calparaN;j=0;
        while(i>0){
            vparaTid=paraQue.para[paraQue.cnt-i].tidx;
            if((regPara=findInTemReg(vparaTid))==-1){
                if(isGlobal(vparaTid)){
//                    if(j<4){
//                        fprintf(codefile,"lw $a%d,glb_%s\n",j,tab[vparaTid].name);
//                    }else{
                        fprintf(codefile,"lw $v0,glb_%s\n",tab[vparaTid].name);//v0 use for temp reg
                        fprintf(codefile,"sw $v0,%d($sp)\n",j*4);
//                    }
                }else{
//                    if(j<4){
//                        fprintf(codefile,"lw $a%d,%d($fp)\n",j,tab[vparaTid].adr*4);
//                    }else{
                        fprintf(codefile,"lw $v0,%d($fp)\n",tab[vparaTid].adr*4);
                        fprintf(codefile,"sw $v0,%d($sp)\n",j*4);
//                    }
                }
            }else{
//                if(j<4){
//                    fprintf(codefile,"add $a%d,$0,$t%d\n",j,regPara);
//                }else{
                    fprintf(codefile,"sw $t%d,%d($sp)\n",regPara,j*4);
//                }
            }
            j++;
            i--;
        }
    }
    paraQue.cnt=paraQue.cnt-calparaN;
    clearTemReg();
    if(paraN<=4){
        fprintf(codefile,"sw $fp,16($sp)\n");
    }else{
        fprintf(codefile,"sw $fp,%d($sp)\n",(paraN)*4);
    }
    fprintf(codefile,"add $fp,$0,$sp\n");
    fprintf(codefile,"jal func_%s\n",btab[funcBtid].name);
    fprintf(codefile,"nop\n");
//    for(i=0;i<4 && swFlag[i]==1;i++){
//        fprintf(codefile,"lw $a%d,%d($fp)\n",i,i*4);
//    }
    if(hasRet==1){
        int retReg;
        retReg=findInTemReg(retTid);
        if(retReg==-1){
            retReg=getEmpTemReg(retTid,-1,-1);
        }
        tReg.dif[retReg]=1;
        fprintf(codefile,"add $t%d,$0,$v0# ret value from call of func %s\n",
        retReg,btab[funcBtid].name);
    }

}

void calPaToObj(){//todo calpa can be value
    //paraQue.isVal=mCode[mIdxCur].rTyp==varg?1:0;
    if(paraQue.cnt==PAMAX){
        fprintf(codefile,"#error, exit");
        exit(1);
    }
    paraQue.para[paraQue.cnt].tidx=mCode[mIdxCur].res.tidx;
    paraQue.cnt++;
}