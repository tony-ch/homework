#include "global.h"
#include "err.h"
int getPath(char* path);//读取控制台输入的路径
FILE* fin=NULL;//源文件
FILE* fout=NULL;//结果文件
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

