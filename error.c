//
// Created by tony on 11/5/18.
//
#include "common.h"

char * emsg[] ={
        "输入不完整",//INCOMPLETE_INPUT_ERR
        "非法字符",//INVALID_CHAR_ERR
        "运行时错误"//RUNTIME_ERR
};

void error(enum ERRORTYPE e, const char *source, int l, int c){
    switch (e){
        case INCOMPLETE_INPUT_ERR:
        case INVAID_CHAR_ERR:
            LOG(ERR_LOG,source,L"%s, line %d, col %d",emsg[e],l,c);
            endProc(-1);
            break;
        default:
            LOG(ERR_LOG,source,L"%s",emsg[RUNTIME_ERR]);
            endProc(-1);
    }
}