#include "global.h"
FILE* fin=NULL;//源文件
FILE* fout=NULL;//结果文件
char ch=' ';//最新读入的字符!需要初始化
char line[LLEN];
int lcnt=0;//line num
int lleng=0;//line index
int ccnt=0;//ch cnt;
int ecnt=0;//err cnt;
char* key[KEYNO];//保留字
enum symbol ksym[KEYNO];//保留字
char punc[SPSN];//分隔符号
char* emsg[ERRMAX];
int err[ERRMAX][3];
char token[STRMAX];
int num=0;

char* symbolstr[]={"CONSTSY","INTSY","CHARSY","VOIDSY","MAINSY","IFSY","ELSESY","WHILESY","SWITCHSY","CASESY","DEFAULTSY","SCANFSY","PRINTFSY","RETURNSY",
"PLUS","MINUS","MUL","DIVD","LES","GTR","BECOME","SEMICOLON ","COMMA","LPARENT","RPARENT","LBRACK","RBRACK","LBRACE","RBRACE","COLON",
"LEQ","GEQ","NEQ","EQL",
"IDENT","UNSIGNUM","ZERO",
"CHARCON","STRCON",
"NUL"};

void getch(){//读取下一个字符，存放到ch中
    if(ccnt==lleng){//一行结束
        if(feof(fin)){
            //fputs("program ends",fout);
            fclose(fin);
            fclose(fout);
            exit(0);
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
    if(isalpha(ch)||ch=='_'){
        int i=0,j=0;
        do{
            if(i<=ALENMAX-3){
                token[i]=tolower(ch);//!to_lower
                i++;
            }
            getch();
        }while(isalpha(ch) || isdigit(ch) || ch=='_' );
        token[i]=0;
        for(j=0;j<KEYNO;j++){
            if(strcmp(token,key[j])==0){
                sym=ksym[j];
                break;
            }
        }
        if(j==KEYNO){
            sym=ident;
        }
    }else if(ch=='0'){
        sym=zero;
        token[0]='0';token[1]=0;
        num=0;
        getch();
        if(isdigit(ch)){
            error(1);//todo
            while(isdigit(ch)){//todo skip
                getch();
            }
        }
    }else if(isdigit(ch)){
        int i=0;
        sym=unsignum;
        num=0;
        while(isdigit(ch)){
            if(i<NMAX){//!NMAX
                token[i]=ch;
                i++;
                num=num*10+(ch-'0');
            }else{
                error(1);//todo
            }
            getch();
        }
        token[i]=0;
    }else if(ch=='\''){
        char con=' ';
        getch();
        if(isalpha(ch) || isdigit(ch)|| ch=='+' || ch=='*'){
            con=ch;
            getch();
            if(ch=='\''){
                sym=charcon;
                token[0]='\'';token[1]=con;token[2]='\'';token[3]=0;
                getch();
            }else{
                error(1);//todo;
                if(ch=='\"'){//!
                    sym=charcon;
                    token[0]='\'';token[1]=con;token[2]='\'';token[3]=0;
                    getch();
                }else{
                    sym=nul;
                    token[0]='\'';
                    token[1]=ch;
                    token[2]=0;
                }
            }
        }else{
            error(1);//todo
            sym=nul;
            token[0]='\'';token[1]=0;
        }
    }else if(ch=='\"'){
        int i=0;
        sym=strcon;
        do{
            if(ch>=32 && ch<=126 && ch!=33){
                if(i<=STRMAX-3){
                    token[i]=ch;
                    i++;
                }
            }else{
                error(1);//todo
            }
            getch();
        }while(ch!='\"');
        token[i]='\"';
        getch();//！需要再读一个字符
        i++;
        token[i]=0;
    }else if(ch=='<'){
        getch();
        if(ch=='='){
            token[0]='<';token[1]='=';token[2]=0;
            sym=leq;
            getch();
        }else{
            token[0]='<';token[1]=0;
            sym=les;
        }
    }else if(ch=='>'){
        getch();
        if(ch=='='){
            token[0]='>';token[1]='=';token[2]=0;
            sym=geq;
            getch();
        }else{
            token[0]='>';token[1]=0;
            sym=gtr;
        }
    }else if(ch=='!'){
        getch();
        if(ch=='='){
            sym=neq;
            token[0]='!';
            token[1]='=';
            token[2]=0;
            getch();
        }else{
            error(1);//todo
            sym=nul;
            token[0]=ch;token[1]=0;
        }
    }else if(ch=='='){
        getch();
        if(ch=='='){
            sym=eql;
            token[0]='=';
            token[1]='=';
            token[2]=0;
            getch();
        }else{
            sym=become;
            token[0]='=';
            token[1]=0;
        }
    }else if(ch=='+'){
        sym=plus;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='-'){
        sym=minus;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='*'){
        sym=mul;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='/'){
        sym=divd;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch==';'){
        sym=semicolon;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch==','){
        sym=comma;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='('){
        sym=lparent;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch==')'){
        sym=rparent;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='['){
        sym=lbrack;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch==']'){
        sym=rbrack;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='{'){
        sym=lbrace;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch=='}'){
        sym=rbrace;
        token[0]=ch;token[1]=0;
        getch();
    }else if(ch==':'){
        sym=colon;
        token[0]=ch;token[1]=0;
        getch();
    }else{
        error(0);//todo
        sym=nul;
        token[0]=ch;token[1]=0;
        getch();
    }
    fprintf(fout,"%-10s:\t\t%s\n",symbolstr[sym],token);
}
