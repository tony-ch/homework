#include "global.h"

#define KEYNO 14//number of key words
#define LLEN 200//line len limit
#define NMAX 10//num max len

void getch();

void getsym();

char ch = ' ';//最新读入的字符!需要初始化
char line[LLEN];
int lcnt = 0;//line num
int lleng = 0;
int ccnt = 0;
//char punc[SPSN];//分隔符号
//struct SYMSTRC symBuf[3];
int symBufIdx = 0;
char key[KEYNO][10] =//保留字
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
char symbolstr[][20] = {
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

void getch() {//读取下一个字符，存放到ch中
    if (ccnt == lleng) {//一行结束
        if (feof(fin)) {
            ch = EOF;
            return;
        }
        lcnt = lcnt + 1;
        ccnt = 0;
        lleng = 0;
        while ((ch = fgetc(fin)) != EOF && ch != '\n') {
            line[lleng] = ch;
            lleng = lleng + 1;
        }
        line[lleng] = '\n';
        lleng = lleng + 1;
    }
    ch = line[ccnt];
    ccnt = ccnt + 1;
}

void getsym() {
    while (isspace(ch)) {
        getch();
    }
    symBuf[symBufIdx].lin = lcnt;
    symBuf[symBufIdx].col = ccnt;
    if (isalpha(ch) || ch == '_') {
        int i = 0, j = 0;
        do {
            if (i <= ALENMAX - 3) {
                symBuf[symBufIdx].token[i] = tolower(ch);//!to_lower
                i++;
            }
            getch();
        } while (isalpha(ch) || isdigit(ch) || ch == '_');
        symBuf[symBufIdx].token[i] = 0;
        for (j = 0; j < KEYNO; j++) {
            if (strcmp(symBuf[symBufIdx].token, key[j]) == 0) {
                symBuf[symBufIdx].id = ksym[j];
                break;
            }
        }
        if (j == KEYNO) {
            symBuf[symBufIdx].id = identsy;
        }
    } else if (ch == '0') {
        symBuf[symBufIdx].id = zerosy;
        symBuf[symBufIdx].token[0] = '0';
        symBuf[symBufIdx].token[1] = 0;
        //num=0;
        getch();
        if (isdigit(ch)) {
            error(2);//! "0 before unsigned or num too long"(T)
            while (isdigit(ch)) {
                getch();
            }
        }
    } else if (isdigit(ch)) {
        int i = 0;
        symBuf[symBufIdx].id = unsignumsy;
        //num=0;
        while (isdigit(ch)) {
            if (i < NMAX) {//!NMAX
                symBuf[symBufIdx].token[i] = ch;
                i++;
                getch();
                //num=num*10+(ch-'0');
            } else {
                error(2);//! "0 before unsigned or num too long"(T)
                while (isdigit(ch)) {
                    getch();
                }
            }
        }
        symBuf[symBufIdx].token[i] = 0;
    } else if (ch == '\'') {
        char con;
        getch();
        if (!isalpha(ch) && !isdigit(ch) && ch != '+' && ch != '*' && ch != '-' && ch != '/' && ch != '_') {
            error(1);//! invalid character(F)
            //symBuf[symBufIdx].id=nul;
            //symBuf[symBufIdx].token[0]='\'';symBuf[symBufIdx].token[1]=0;
            ch = '0';
        }
        con = ch;
        getch();
        if (ch == '\'') {
            symBuf[symBufIdx].id = charconsy;
            symBuf[symBufIdx].token[0] = '\'';
            symBuf[symBufIdx].token[1] = con;
            symBuf[symBufIdx].token[2] = '\'';
            symBuf[symBufIdx].token[3] = 0;
            getch();
        } else {
            error(3);//! 应是'(F)
            if (ch == '\"') {//!
                symBuf[symBufIdx].id = charconsy;
                symBuf[symBufIdx].token[0] = '\'';
                symBuf[symBufIdx].token[1] = con;
                symBuf[symBufIdx].token[2] = '\'';
                symBuf[symBufIdx].token[3] = 0;
                getch();
            } else {//!增加一个'
                symBuf[symBufIdx].id = charconsy;
                symBuf[symBufIdx].token[0] = '\'';
                symBuf[symBufIdx].token[1] = con;
                symBuf[symBufIdx].token[2] = '\'';
                symBuf[symBufIdx].token[3] = 0;
            }
        }
    } else if (ch == '\"') {
        int i = 0;
        symBuf[symBufIdx].id = strconsy;
        do {
            if (ch >= 32 && ch <= 126) {// && ch!=34
                if (i <= STRMAX - 3) {
                    symBuf[symBufIdx].token[i] = ch;
                    i++;
                }
            } else {
                error(1);//! invalid character (T)
            }
            getch();
        } while (ch != '\"' && ch != EOF && ch != '\n');
        symBuf[symBufIdx].token[i] = '\"';
        if (ch != '\"') {
            error(35);
        }
        getch();//！需要再读一个字符
        i++;
        symBuf[symBufIdx].token[i] = 0;
    } else if (ch == '<') {
        getch();
        if (ch == '=') {
            symBuf[symBufIdx].token[0] = '<';
            symBuf[symBufIdx].token[1] = '=';
            symBuf[symBufIdx].token[2] = 0;
            symBuf[symBufIdx].id = leqsy;
            getch();
        } else {
            symBuf[symBufIdx].token[0] = '<';
            symBuf[symBufIdx].token[1] = 0;
            symBuf[symBufIdx].id = lessy;
        }
    } else if (ch == '>') {
        getch();
        if (ch == '=') {
            symBuf[symBufIdx].token[0] = '>';
            symBuf[symBufIdx].token[1] = '=';
            symBuf[symBufIdx].token[2] = 0;
            symBuf[symBufIdx].id = geqsy;
            getch();
        } else {
            symBuf[symBufIdx].token[0] = '>';
            symBuf[symBufIdx].token[1] = 0;
            symBuf[symBufIdx].id = gtrsy;
        }
    } else if (ch == '!') {
        getch();
        if (ch == '=') {
            symBuf[symBufIdx].id = neqsy;
            symBuf[symBufIdx].token[0] = '!';
            symBuf[symBufIdx].token[1] = '=';
            symBuf[symBufIdx].token[2] = 0;
            getch();
        } else {
            error(1);//! invalid character (F)
            symBuf[symBufIdx].id = neqsy;
            symBuf[symBufIdx].token[0] = '!';
            symBuf[symBufIdx].token[1] = '=';
            symBuf[symBufIdx].token[2] = 0;
        }
    } else if (ch == '=') {
        getch();
        if (ch == '=') {
            symBuf[symBufIdx].id = eqlsy;
            symBuf[symBufIdx].token[0] = '=';
            symBuf[symBufIdx].token[1] = '=';
            symBuf[symBufIdx].token[2] = 0;
            getch();
        } else {
            symBuf[symBufIdx].id = becomesy;
            symBuf[symBufIdx].token[0] = '=';
            symBuf[symBufIdx].token[1] = 0;
        }
    } else if (ch == '+') {
        symBuf[symBufIdx].id = plussy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '-') {
        symBuf[symBufIdx].id = minussy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '*') {
        symBuf[symBufIdx].id = mulsy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '/') {
        symBuf[symBufIdx].id = divsy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == ';') {
        symBuf[symBufIdx].id = semicolonsy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == ',') {
        symBuf[symBufIdx].id = commasy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '(') {
        symBuf[symBufIdx].id = lparentsy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == ')') {
        symBuf[symBufIdx].id = rparentsy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '[') {
        symBuf[symBufIdx].id = lbracksy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == ']') {
        symBuf[symBufIdx].id = rbracksy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '{') {
        symBuf[symBufIdx].id = lbracesy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == '}') {
        symBuf[symBufIdx].id = rbracesy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else if (ch == ':') {
        symBuf[symBufIdx].id = colonsy;
        symBuf[symBufIdx].token[0] = ch;
        symBuf[symBufIdx].token[1] = 0;
        getch();
    } else {
        if (ch == EOF) {
            symBuf[symBufIdx].id = eofsy;
            symBuf[symBufIdx].token[0] = 0;
        } else {
            error(1);//! invalid character (T)
            symBuf[symBufIdx].id = nulsy;
            symBuf[symBufIdx].token[0] = ch;
            symBuf[symBufIdx].token[1] = 0;
            getch();
        }
    }
}

void initSymBuf() {
    for (symBufIdx = 0; symBufIdx < 3; symBufIdx++) {
        do {
            getsym();
        } while (symBuf[symBufIdx].id == nulsy);
    }
    symBufIdx = 0;
}

void updateSymBuf() {
    printSym();
    //printf("%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
    do {
        getsym();
    } while (symBuf[symBufIdx].id == nulsy);
    symBufIdx += 1;
    if (symBufIdx == 3) {
        symBufIdx = 0;
    }
    if (symBuf[symBufIdx].id == eofsy) {
        printf("%d,%d", eofsy, symBuf[symBufIdx].id);
        error(0);//!incomplete source file, exit (T)
    }
}

int reachEof() {
    return (symBuf[(symBufIdx + 1) % 3].id == eofsy);
}

void printSym() {
    fprintf(fout, "%-10s:\t\t%s\n", symbolstr[symBuf[symBufIdx].id], symBuf[symBufIdx].token);
}