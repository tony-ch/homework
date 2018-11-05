//
// Created by tony on 11/5/18.
//
#include "common.h"
#define PATHLEN 100
#define RET_ERR -1
#define RET_SUCCESS 0
#define LOGSRC "UTIL"

int getPath(char *path);
const char* LOG_LEVEL_STR[]={"DEBUG","INFO","WARN","ERR"};
wchar_t MSG[MSGLEN]={};

void LOG(int level, const char* source, const wchar_t* msg){
    if(level >= LOG_LEVEL){
        wprintf(L"[%s] %s: %ls\n",LOG_LEVEL_STR[level],source,msg);
    }
}

int getPath(char *path){
    int i = 0;
    char ch;
    int flag = 0;
    while ((ch = getchar()) != EOF && ch != L'\n') {
        if (i < PATHLEN - 1)
            path[i++] = ch;
        else
            flag = 1;
    }
    if (flag) {//大于长度限制,报错
        wprintf(L"path is too long\n");
        return RET_ERR;//checked
    }
    path[i] = L'\0';
    return RET_SUCCESS;
}

FILE* getFile(const char* mode,const char* name, const char* default_path){
    FILE *f=NULL;
    char path[PATHLEN];
    while (f == NULL) {//打开源文件
        wprintf(L"please %s path:",name);
        if(default_path!=NULL){
            wprintf(L"(press enter to use default: %s)",default_path);
        }
        wprintf(L"\n");
        if (getPath(path) != RET_SUCCESS)
            continue;
        if(default_path!=NULL && (strcmp(path, "") == 0 || strcmp(path, " ") == 0) ){
            strcpy(path, default_path);
        }
        if ((f = fopen(path, mode)) == NULL)
            wprintf(L"can't open %s\n",name);
    }
    swprintf(MSG,MSGLEN,L"%s, %s",name,path);
    LOG(LOG_INFO,LOGSRC,MSG);
    return f;
}

void endProc(int n){
    if (fin != NULL)
        fclose(fin);
    if (fout != NULL)
        fclose(fout);
    if (codefile != NULL)
        fclose(codefile);
    wprintf(L"press enter to end process ...\n");
    getwchar();
    exit(n);
}