#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED
#define ERRMAX 40// max err num

extern char ch;//最新读入的字符!需要初始化
extern int lcnt;//line num
extern int ccnt;

//int err[ERRMAX][3];
char errPlace;
int ecnt = 0;//err cnt;
char *emsg[ERRMAX] = {
        "源文件不完整",//0 updatesymBuf
        "非法字符, 将之跳过",//1 getsym
        "数有前导0或整数太长",//2 getsym
        "应是 \' ",//3 getsym
        "应是 ; ",//4
        "函数重定义",//5 enter
        "标识符重定义",//6 enter
        "未知的常量类型标识",//7
        "未知的参数类型",//8
        "应是标识符",//9
        "应是 (",//10
        "应是 )",//11
        "应是{",//12
        "应是}",//13
        "应是]",//14
        "应是:",//15
        "函数未定义",//16
        "标识符未定义或标识符种类不正确",//17
        "常量必须有初始值",//18
        "int常量的初始值应为整数，char常量的初始值应为字符",//19
        "数组的大小应为非0无符号整数",//20
        "非法的整数 ",//21
        "main函数返回类型应为void",//22
        "必须有main函数",//23
        "非法因子",//24
        "非法语句",//25
        "switch至少有一个case",//26
        "case标签过多",//27
        "[]只用于访问数组",//28
        "应为无返回值return",//29
        "缺少return语句",//30
        "函数调用参数个数不正确",//31
        "不能对数组、常量或函数赋值",//32
        "不合法的变量定义",//33
        "函数应以void, int, 或char开始",//34
        "字符串非正常结束",//35
        "语句后的内容不合法",//36
        "位置不合法的常量定义",//37
        "位置不合法的变量定义"//38
};

char *wmsg[ERRMAX] = {
        "赋值语句两端类型不一致",//0
        "返回值类型应为int",//1
        "返回值类型应为char",//2
        "参数类型不匹配",//3
        "常量定义应该在变量定义之前"//4
};

//void testAfterStat(char pos);

void showPos();

//void warn(int n);

//void error(int n);

void printErr(int n);

#endif // ERR_H_INCLUDED
