//
// Created by tony on 11/5/18.
//
#include "common.h"
#define PATHLEN 100

int getPath(wchar_t *path);
const wchar_t* LOG_LEVEL_STR[]={L"DEBUG",L"INFO",L"WARN",L"ERR"};
wchar_t MSG[MSGLEN]={};

void LOG(int level, const wchar_t* source, const wchar_t* msg){
    if(level >= LOG_LEVEL){
        wprintf(L"[%ls] %ls: %ls\n",LOG_LEVEL_STR[level],source,msg);
    }
}

int getPath(wchar_t *path){
    int i = 0;
    wchar_t ch;
    int flag = 0;
    while ((ch = getwchar()) != EOF && ch != L'\n') {
        if (i < PATHLEN - 1)
            path[i++] = ch;
        else
            flag = 1;
    }
    if (flag) {//大于长度限制,报错
        wprintf(L"path is too long\n");
        return -1;//checked
    }
    path[i] = L'\0';
    return 0;
}

FILE* getFile(const wchar_t* name, const wchar_t* default_path){
    FILE *f=NULL;
    wchar_t path[PATHLEN];
    while (f == NULL) {//打开源文件
        wprintf(L"please %ls path:",name);
        if(default_path!=NULL){
            wprintf(L"(press enter to use default: %ls)",default_path);
        }
        wprintf(L"\n");
        if (getPath(path) < 0)
            continue;
        if(default_path!=NULL && (wcscmp(path, L"") == 0 || wcscmp(path, L" ") == 0) ){
            wcpcpy(path, default_path);
        }
        if ((f = fopen(path, "r")) == NULL)
            wprintf(L"can't open %ls\n",name);
    }
    swprintf(MSG,MSGLEN,L"%ls, %ls",name,path);
    LOG(LOG_INFO,L"UTIL",MSG);
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