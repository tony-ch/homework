#include "common.h"

FILE *fin = NULL;//源文件
FILE *fout = NULL;//结果代码文件
FILE *codefile = NULL; //中间代码文件
extern wchar_t ch;
int main() {
    setlocale(LC_ALL,"");
    fin = getFile("r","input source file","input_test.txt");
#if LOG_LEVEL==LOG_DEBUG
    fout = getFile("w","result file","./res.out");
    codefile = getFile("w","mid code file","./code.out");
#endif

    while(1){
        getch(fin);
        if(ch==EOF)
            break;
    }

    endProc(0);
    return 0;
}

