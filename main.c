#include "global.h"
#include "err.h"

int getPath(char *path);//读取控制台输入的路径
FILE *fin = NULL;//源文件
FILE *fout = NULL;//结果文件
FILE *codefile = NULL;//目标代码文件
int main() {
    //char cwd[100];
    //getcwd(cwd,100);
    //printf("cwd:\"%s\"\n",cwd);
    char finPath[PATHLEN];
    char foutPath[PATHLEN];
    while (fin == NULL) {//打开源文件
        printf("please input source file path:\n");
        if (getPath(finPath) < 0)
            continue;
        if ((fin = fopen(finPath, "r")) == NULL)
            printf("can't open source file\n");
    }
#ifdef DEBUG
    while (fout == NULL) {//打开结果文件
        printf("please input mid code file path: (press enter to use default: a.out)\n");
        if (getPath(foutPath) < 0)
            continue;
        if (strcmp(foutPath, "") == 0 || strcmp(foutPath, " ") == 0)
            strcpy(foutPath, "a.out");
        if ((fout = fopen(foutPath, "w")) == NULL)
            printf("can't open midcode file\n");
    }
#else
    strcpy(foutPath,"a.out");
    if ((fout = fopen(foutPath, "w")) == NULL){
            printf("can't open midcode file\n");
            endProc(-1);
    }
#endif

    initSymBuf();
    program();
    if (ecnt == 0) {
        char fcodePath[PATHLEN];
#ifdef DEBUG
        printf("no error, begin to generate obj code.\n");
        while (codefile == NULL) {//打开结果文件
            printf("please input code file path: (press enter to use default: a.asm)\n");
            if (getPath(fcodePath) < 0)
                continue;
            if (strcmp(fcodePath, "") == 0 || strcmp(fcodePath, " ") == 0)
                strcpy(fcodePath, "a.asm");
            if ((codefile = fopen(fcodePath, "w")) == NULL)
                printf("can't open obj code file\n");
        }
#else
        printf("no error.\nbegin to generate obj code to a.asm.\n");
        strcpy(fcodePath,"a.asm");
        if ((codefile = fopen(fcodePath, "w")) == NULL) {
            printf("can't open obj code file\n");
            endProc(-1);
        }
#endif
        generate();
    }
    endProc(0);
    return 0;
}

int getPath(char *path) {//读取控制台输入的路径
    int i = 0;
    char ch;
    int flag = 0;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (i < PATHLEN - 1)
            path[i++] = ch;
        else
            flag = 1;
    }
    if (flag) {//大于长度限制,报错
        printf("path is too long\n");
        return -1;//checked
    }
    path[i] = '\0';
    return 0;
}

void endProc(int n) {
    if (fin != NULL)
        fclose(fin);
    if (fout != NULL)
        fclose(fout);
    if (codefile != NULL)
        fclose(codefile);
    system("pause");
    exit(n);
}