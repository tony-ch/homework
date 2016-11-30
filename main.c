#include "global.h"
#include "err.h"
int getPath(char* path);//读取控制台输入的路径
FILE* fin=NULL;//源文件
FILE* fout=NULL;//结果文件
FILE* codefile=NULL;//目标代码文件
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

    initSymBuf();
    program();
    if(!reachEof()){
        fprintf(fout,"there should be nothing after main func.\n");
        printf("there should be nothing after main func.\n");
    }
    if(ecnt==0){
        printf("no error, begin to generate obj code.");
        while(codefile==NULL){//打开结果文件
            printf("please input code file path:\n");
            if(getPath(foutPath)<0)
                continue;
            if((codefile=fopen(foutPath,"w"))==NULL)
                printf("can't open file\n");
        }
        generate();
    }
    fclose(fin);
    fclose(fout);
    fclose(codefile);
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

