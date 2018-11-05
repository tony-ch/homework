//
// Created by tony on 11/5/18.
//
#include "common.h"

void error(enum ERRORTYPE e, const char *source, int l, int c){
    switch (e){
        case INCOMPLETE_INPUT_ERR:
        case INVAID_CHAR_ERR:
            LOG(ERR_LOG,source,L"%s, line %d, col %d",getErrStr(e),l,c);
            endProc(-1);
            break;
        default:
            LOG(ERR_LOG,source,L"%s",getErrStr(e));
            endProc(-1);
    }
}