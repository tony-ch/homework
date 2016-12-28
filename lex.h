//
// Created by tony on 2016/12/29.
//

#ifndef COMPILE_LEX_H
#define COMPILE_LEX_H

#define KEYNO 14//number of key words
#define LLEN 200//line len limit
#define NMAX 10//num max len

char ch = ' ';//最新读入的字符!需要初始化
char line[LLEN];
int lcnt = 0;//line num
int lleng = 0;
int ccnt = 0;
//char punc[SPSN];//分隔符号
//struct SYMSTRC symBuf[3];
int symBufIdx = 0;
char *key[KEYNO] =//保留字
        {
                "const", "int", "char", "void",
                "main", "if", "else", "while",
                "switch", "case", "default", "scanf",
                "printf", "return"
        };
enum SYMBOL ksym[KEYNO] =//保留字
        {
                constsy, intsy, charsy, voidsy,
                mainsy, ifsy, elsesy, whilesy,
                switchsy, casesy, defaultsy, scanfsy,
                printfsy, returnsy
        };
char *symbolstr[] = {
        "EOFSY", "CONSTSY", "INTSY", "CHARSY", "VOIDSY",
        "MAINSY", "IFSY", "ELSESY", "WHILESY",
        "SWITCHSY", "CASESY", "DEFAULTSY", "SCANFSY",
        "PRINTFSY", "RETURNSY",
        "PLUS", "MINUS", "MUL", "DIVD", "LES", "GTR", "LEQ", "GEQ", "NEQ", "EQL",
        "BECOME", "SEMICOLON ",
        "COMMA", "LPARENT", "RPARENT", "LBRACK",
        "RBRACK", "LBRACE", "RBRACE", "COLON",
        "IDENT", "UNSIGNUM", "ZERO",
        "CHARCON", "STRCON",
        "NUL"
};

void getch();

void getsym();

#endif //COMPILE_LEX_H
