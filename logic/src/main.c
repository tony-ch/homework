#include "common.h"

#define LOGSRC "MAIN"
#define PASSONE 0
#define PASSTWO 1
FILE *fin = NULL;//源文件
FILE *fout = NULL;//结果代码文件
FILE *codefile = NULL; //中间代码文件
extern wchar_t ch;
int main(int argc,char** argv) {
    setlocale(LC_ALL,"");
    fin = getFile("r,ccs=UTF-8","input source file",argv[1]);
#if LOG_LEVEL==DEBUG_LOG
    fout = getFile("w,ccs=UTF-8","result file","./res.out");
    codefile = getFile("w,ccs=UTF-8","mid code file","./code.out");
#endif
	wprintf(L"\n####################################### LOGIC ########################################\n");
	wprintf(L"\n######## inputs ########\n");
    init(PASSONE);
    functions();
    LOG(DEBUG_LOG,LOGSRC,L"two pass");
    init(PASSTWO);
    program();
    run();
    check_complete();
    endProc(0);
    return 0;
}

