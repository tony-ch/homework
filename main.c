#include "global.h"
#include "err.h"
int getPath(char* path);//��ȡ����̨�����·��
FILE* fin=NULL;//Դ�ļ�
FILE* fout=NULL;//����ļ�
FILE* codefile=NULL;//Ŀ������ļ�
int main()
{
    char finPath[PATHLEN];
    char foutPath[PATHLEN];
    while(fin==NULL){//��Դ�ļ�
        printf("please input source file path:\n");
        if(getPath(finPath)<0)
            continue;
        if((fin=fopen(finPath,"r"))==NULL)
            printf("can't open file\n");
    }
     while(fout==NULL){//�򿪽���ļ�
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
        while(codefile==NULL){//�򿪽���ļ�
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

int getPath(char* path){//��ȡ����̨�����·��
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
    if(flag){//���ڳ�������,����
        printf("path is too long\n");
        return -1;
    }
    path[i]='\0';
    return 0;
}

