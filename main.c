#include "global.h"
int getPath(char* path);//读取控制台输入的路径
void setup();
int main()
{
    char finPath[PATHLEN];
    char foutPath[PATHLEN];
    while(fin==NULL){//打开源文件
        printf("please input source file path:\n");
        if(getPath(finPath)<0)
            continue;
        if((fin=fopen(finPath,"r"))==NULL)
            printf("can't open file\n");
    }
     while(fout==NULL){//打开结果文件
        printf("please input result file path:\n");
        if(getPath(foutPath)<0)
            continue;
        if((fout=fopen(foutPath,"w"))==NULL)
            printf("can't open file\n");
    }

    setup();
    while(1){
        getsym();
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int getPath(char* path){//读取控制台输入的路径
    int i=0;
    char ch;
    int flag=0;
    while((ch=getchar())!=EOF && ch!='\n'){
        if(i<PATHLEN-1){
            path[i]=ch;
            i++;
        }else{
            flag=1;
        }
    }
    if(flag){//大于长度限制,报错
        printf("path is too long\n");
        return -1;
    }
    path[i]='\0';
    return 0;
}

void setup(){
    key[0]="const";     key[1]="int";   key[2]="char";      key[3]="void";
    key[4]="main";      key[5]="if";    key[6]="else";      key[7]="while";
    key[8]="switch";    key[9]="case";  key[10]="default";  key[11]="scanf";
    key[12]="printf";   key[13]="return";
    ksym[0]=constsy;    ksym[1]=intsy;  ksym[2]=charsy;     ksym[3]=voidsy;
    ksym[4]=mainsy;     ksym[5]=ifsy;   ksym[6]=elsesy;     ksym[7]=whilesy;
    ksym[8]=switchsy;   ksym[9]=casesy; ksym[10]=defaultsy; ksym[11]=scanfsy;
    ksym[12]=printfsy;  ksym[13]=returnsy;
    /*
    punc[0]='+';
    punc[1]='-';
    punc[2]='*';
    punc[3]='/';
    punc[4]='<';
    punc[5]='>';
    punc[6]='=';
    punc[7]=';';
    punc[8]=',';
    punc[9]='(';
    punc[10]=')';
    punc[11]='[';
    punc[12]=']';
    punc[13]='{';
    punc[14]='}';
    punc[15]=':';
    */
    emsg[0]="invalid character";
    emsg[1]=" ";
}

void error(int n){
    if(ecnt<ERRMAX){
        err[ecnt][0]=lcnt;
        err[ecnt][1]=ccnt;
        err[ecnt][2]=n;
    }
    fprintf(fout,"line:%d col:%d errno:%d %s\n",lcnt,ccnt,n,emsg[n]);
    ecnt=ecnt+1;
}

