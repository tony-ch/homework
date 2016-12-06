#include "global.h"
#include "err.h"

char ch=' ';//最新读入的字符!需要初始化
char line[LLEN];
int lcnt=0;//line num
int lleng=0;
int ccnt=0;
//char punc[SPSN];//分隔符号
//struct SYMSTRC symBuf[3];
int symBufIdx=0;
char* key[KEYNO]=//保留字
{
    "const",    "int",      "char",     "void",
    "main",     "if",       "else",     "while",
    "switch",   "case",     "default",  "scanf",
    "printf",   "return"
};
enum SYMBOL ksym[KEYNO]=//保留字
{
    constsy,    intsy,      charsy,     voidsy,
    mainsy,     ifsy,       elsesy,     whilesy,
    switchsy,   casesy,     defaultsy,  scanfsy,
    printfsy,   returnsy
};
char* symbolstr[]={
    "CONSTSY",  "INTSY",    "CHARSY",   "VOIDSY",
    "MAINSY",   "IFSY",     "ELSESY",   "WHILESY",
    "SWITCHSY", "CASESY",   "DEFAULTSY","SCANFSY",
    "PRINTFSY", "RETURNSY",
    "PLUS",     "MINUS",    "MUL",      "DIVD",
    "LES",      "GTR",      "BECOME",   "SEMICOLON ",
    "COMMA",    "LPARENT",  "RPARENT",  "LBRACK",
    "RBRACK",   "LBRACE",   "RBRACE",   "COLON",
    "LEQ",      "GEQ",      "NEQ",      "EQL",
    "IDENT",    "UNSIGNUM", "ZERO",
    "CHARCON",  "STRCON",
    "NUL"
};

void getch(){//读取下一个字符，存放到ch中
    if(ccnt==lleng){//一行结束
        if(feof(fin)){
            ch=EOF;
            return;
        }
        lcnt=lcnt+1;
        ccnt=0;
        lleng=0;
        while((ch=fgetc(fin))!=EOF && ch!='\n'){
            line[lleng]=ch;
            lleng=lleng+1;
        }
        line[lleng]=' ';
        lleng=lleng+1;
    }
    ch=line[ccnt];
    ccnt=ccnt+1;
}

void getsym(){
    while(isspace(ch)){
        getch();
    }
    symBuf[symBufIdx].lin=lcnt;
    symBuf[symBufIdx].col=ccnt;
    if(isalpha(ch)||ch=='_'){
        int i=0,j=0;
        do{
            if(i<=ALENMAX-3){
                symBuf[symBufIdx].token[i]=tolower(ch);//!to_lower
                i++;
            }
            getch();
        }while(isalpha(ch) || isdigit(ch) || ch=='_' );
        symBuf[symBufIdx].token[i]=0;
        for(j=0;j<KEYNO;j++){
            if(strcmp(symBuf[symBufIdx].token,key[j])==0){
                symBuf[symBufIdx].id=ksym[j];
                break;
            }
        }
        if(j==KEYNO){
            symBuf[symBufIdx].id=ident;
        }
    }else if(ch=='0'){
        symBuf[symBufIdx].id=zero;
        symBuf[symBufIdx].token[0]='0';symBuf[symBufIdx].token[1]=0;
        //num=0;
        getch();
        if(isdigit(ch)){
            error(2);//! "0 before unsigned or num too long"(T)
        }
    }else if(isdigit(ch)){
        int i=0;
        symBuf[symBufIdx].id=unsignum;
        //num=0;
        while(isdigit(ch)){
            if(i<NMAX){//!NMAX
                symBuf[symBufIdx].token[i]=ch;
                i++;
                getch();
                //num=num*10+(ch-'0');
            }else{
                error(2);//! "0 before unsigned or num too long"(T)
            }
        }
        symBuf[symBufIdx].token[i]=0;
    }else if(ch=='\''){
        char con;
        getch();
        if(!isalpha(ch) && !isdigit(ch) && ch!='+' && ch!='*' && ch!='-' && ch!='/' && ch!='_'){
            error(1);//! invalid character(F)
            //symBuf[symBufIdx].id=nul;
            //symBuf[symBufIdx].token[0]='\'';symBuf[symBufIdx].token[1]=0;
            ch='0';
        }
        con=ch;
        getch();
        if(ch=='\''){
            symBuf[symBufIdx].id=charcon;
            symBuf[symBufIdx].token[0]='\'';symBuf[symBufIdx].token[1]=con;symBuf[symBufIdx].token[2]='\'';symBuf[symBufIdx].token[3]=0;
            getch();
        }else{
            error(3);//! 应是'(F)
            if(ch=='\"'){//!
                symBuf[symBufIdx].id=charcon;
                symBuf[symBufIdx].token[0]='\'';symBuf[symBufIdx].token[1]=con;symBuf[symBufIdx].token[2]='\'';symBuf[symBufIdx].token[3]=0;
                getch();
            }else{//!增加一个'
                symBuf[symBufIdx].id=charcon;
                symBuf[symBufIdx].token[0]='\'';symBuf[symBufIdx].token[1]=con;symBuf[symBufIdx].token[2]='\'';symBuf[symBufIdx].token[3]=0;
            }
        }
    }else if(ch=='\"'){
        int i=0;
        symBuf[symBufIdx].id=strcon;
        do{
            if(ch>=32 && ch<=126){// && ch!=34
                if(i<=STRMAX-3){
                    symBuf[symBufIdx].token[i]=ch;
                    i++;
                }
            }else{
                error(1);//! invalid character (T)
            }
            getch();
        }while(ch!='\"');
        symBuf[symBufIdx].token[i]='\"';
        getch();//！需要再读一个字符
        i++;
        symBuf[symBufIdx].token[i]=0;
    }else if(ch=='<'){
        getch();
        if(ch=='='){
            symBuf[symBufIdx].token[0]='<';symBuf[symBufIdx].token[1]='=';symBuf[symBufIdx].token[2]=0;
            symBuf[symBufIdx].id=leq;
            getch();
        }else{
            symBuf[symBufIdx].token[0]='<';symBuf[symBufIdx].token[1]=0;
            symBuf[symBufIdx].id=les;
        }
    }else if(ch=='>'){
        getch();
        if(ch=='='){
            symBuf[symBufIdx].token[0]='>';symBuf[symBufIdx].token[1]='=';symBuf[symBufIdx].token[2]=0;
            symBuf[symBufIdx].id=geq;
            getch();
        }else{
            symBuf[symBufIdx].token[0]='>';symBuf[symBufIdx].token[1]=0;
            symBuf[symBufIdx].id=gtr;
        }
    }else if(ch=='!'){
        getch();
        if(ch=='='){
            symBuf[symBufIdx].id=neq;
            symBuf[symBufIdx].token[0]='!';
            symBuf[symBufIdx].token[1]='=';
            symBuf[symBufIdx].token[2]=0;
            getch();
        }else{
            error(1);//! invalid character (F)
            symBuf[symBufIdx].id=neq;
            symBuf[symBufIdx].token[0]='!';
            symBuf[symBufIdx].token[1]='=';
            symBuf[symBufIdx].token[2]=0;
        }
    }else if(ch=='='){
        getch();
        if(ch=='='){
            symBuf[symBufIdx].id=eql;
            symBuf[symBufIdx].token[0]='=';
            symBuf[symBufIdx].token[1]='=';
            symBuf[symBufIdx].token[2]=0;
            getch();
        }else{
            symBuf[symBufIdx].id=become;
            symBuf[symBufIdx].token[0]='=';
            symBuf[symBufIdx].token[1]=0;
        }
    }else if(ch=='+'){
        symBuf[symBufIdx].id=plus;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='-'){
        symBuf[symBufIdx].id=minus;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='*'){
        symBuf[symBufIdx].id=mul;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='/'){
        symBuf[symBufIdx].id=divd;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch==';'){
        symBuf[symBufIdx].id=semicolon;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch==','){
        symBuf[symBufIdx].id=comma;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='('){
        symBuf[symBufIdx].id=lparent;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch==')'){
        symBuf[symBufIdx].id=rparent;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='['){
        symBuf[symBufIdx].id=lbrack;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch==']'){
        symBuf[symBufIdx].id=rbrack;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='{'){
        symBuf[symBufIdx].id=lbrace;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch=='}'){
        symBuf[symBufIdx].id=rbrace;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else if(ch==':'){
        symBuf[symBufIdx].id=colon;
        symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
        getch();
    }else{
        if(ch==EOF){
            symBuf[symBufIdx].id=eofs;
        }else{
            error(1);//! invalid character (T)
            symBuf[symBufIdx].id=nul;
            symBuf[symBufIdx].token[0]=ch;symBuf[symBufIdx].token[1]=0;
            getch();
        }
    }
}

void initSymBuf(){
    for(symBufIdx=0;symBufIdx<3;symBufIdx++){
        do{
            getsym();
        }while(symBuf[symBufIdx].id==nul);
    }
	symBufIdx=0;
}

void updateSymBuf(){
	printSym();
	//printf("%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
    do{
        getsym();
    }while(symBuf[symBufIdx].id==nul);
    symBufIdx+=1;
    if(symBuf[symBufIdx].id==eofs){
        error(0);//!incomplete source file, exit (T)
    }
    if(symBufIdx==3){
        symBufIdx=0;
    }
}

int reachEof(){
    return(symBuf[(symBufIdx+1)%3].id==eofs);
}

void printSym(){
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[symBuf[symBufIdx].id],symBuf[symBufIdx].token);
}