//
// Created by tony on 11/5/18.
//
#include "common.h"
#define PATHLEN 100
#define ERR_RET -1
#define SUCCESS_RET 0
#define LOGSRC "UTIL"

#define ENUM_TO_STR(x) case x: return (#x);

int getPath(char *path);
const char* LOG_LEVEL_STR[]={"DEBUG","INFO","WARN","ERR"};

void LOG(int level, const char* source, const wchar_t* format, ...){
    if(level >= LOG_LEVEL){
        wprintf(L"[%S] %S: ",LOG_LEVEL_STR[level],source);
        va_list arglist;
        va_start(arglist,format);
        vwprintf(format,arglist);
        wprintf(L"\n");
        va_end(arglist);
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
        return ERR_RET;//checked
    }
    path[i] = L'\0';
    return SUCCESS_RET;
}

FILE* getFile(const char* mode,const char* name, const char* default_path){
    FILE *f=NULL;
    char path[PATHLEN];
    while (f == NULL) {//打开源文件
        wprintf(L"please input %S path: like ../input_test.txt  ",name);
        if(default_path!=NULL){
            wprintf(L"using default: %S\n",default_path);
			if ((f = fopen(default_path, mode)) == NULL) {
				wprintf(L"can't open %S\n", name);
				default_path = NULL;
				continue;
			}
		}else{
			wprintf(L"(press 'Enter' to use stdin instead of file)\n");
			if (getPath(path) != SUCCESS_RET)
				continue;
			if (strcmp(path, "") == 0 || strcmp(path, " ") == 0) {
				wprintf(L"using stdin, press 'Ctrl'+'Z' and 'Enter' to end input\n");
				f = stdin;
				break;
			}
			if ((f = fopen(path, mode)) == NULL)
				wprintf(L"can't open %S\n", name);
		}
    }
	if(f!=stdin)
		LOG(INFO_LOG,LOGSRC,L"%S, %S",name,path);
    return f;
}

void endProc(int n){
    if (fin != NULL && fin !=stdin)
        fclose(fin);
    if (fout != NULL)
        fclose(fout);
    if (codefile != NULL)
        fclose(codefile);
    for(int i=0;i<funccnt;i++){
        free(functab[i].truth_table);
        functab[i].truth_table = NULL;
    }
    wprintf(L"press enter to end process ...\n");
	getwchar();
    exit(n);
}

const char * getSymStr(enum SYMBOL sym){
    switch (sym){
        ENUM_TO_STR(EOFSYM)
        ENUM_TO_STR(DEFSYM)
        ENUM_TO_STR(LPARENNTSYM)
        ENUM_TO_STR(RPARENTSYM)
        ENUM_TO_STR(COMMASYM)
        ENUM_TO_STR(EQUSYM)
        ENUM_TO_STR(IMPSYM)
        ENUM_TO_STR(DISJSYM)
        ENUM_TO_STR(CONJSYM)
        ENUM_TO_STR(XORSYM)
        ENUM_TO_STR(NOTSYM)
        ENUM_TO_STR(IDENTSYM)
        ENUM_TO_STR(NUMSYM)
        ENUM_TO_STR(LOGICNUMSYM)
        ENUM_TO_STR(NULSYM)
    }
    return "INVALID SYM";
}

const char * getErrStr(enum ERRORTYPE e){
    switch (e){
        ENUM_TO_STR(INVAID_SENTENCE)
        ENUM_TO_STR(INCOMPLETE_INPUT_ERR)
        ENUM_TO_STR(INVAID_CHAR_ERR)
        ENUM_TO_STR(ASSERT_ERROR)
        ENUM_TO_STR(PARA_NUM_TOO_LARGE_ERR)
        ENUM_TO_STR(FUNC_DEF_ERR)
        ENUM_TO_STR(RUNTIME_ERR)
    }
    return "INVALID ERR TYPE";
}

const char * getMopStr(enum MOP op){
    switch (op){
        ENUM_TO_STR(PARAOP)
        ENUM_TO_STR(CALLOP)
        ENUM_TO_STR(EQUOP)
        ENUM_TO_STR(IMPOP)
        ENUM_TO_STR(CONJOP)
        ENUM_TO_STR(DISJOP)
        ENUM_TO_STR(XOROP)
        ENUM_TO_STR(NOTOP)
        ENUM_TO_STR(ENDOP)
    }
    return "INVALID OP TYPE";
}

int is_digit(wchar_t ch){
    return ch >=L'0' && ch <= L'9';
}
int is_alpha(wchar_t ch){
    return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch == L'_';
}
int is_alnum(wchar_t ch){
    return is_alpha(ch) || is_digit(ch);
}

int push(int value){
    stack[pstack] = value;
    pstack++;
    LOG(DEBUG_LOG,LOGSRC,L"stack push %d;p:%d",value,pstack);
    return value;
}

int pop(){
    pstack += -1;
    int res = stack[pstack];
    assert(pstack>=0);
    LOG(DEBUG_LOG,LOGSRC,L"stack pop %d;p:%d",res,pstack);
    return res;
}
