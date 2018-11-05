#include "common.h"

FILE *fin = NULL;//源文件
FILE *fout = NULL;//结果代码文件
FILE *codefile = NULL; //中间代码文件

int main() {

    fin = getFile(L"input source file",NULL);
#if LOG_LEVEL==LOG_DEBUG
    fout = getFile(L"result file",NULL);
    codefile = getFile(L"mid code file",NULL);
#endif

    endProc(0);
    return 0;
}

