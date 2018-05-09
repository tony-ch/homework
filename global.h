#ifndef _global_H
#define _global_H

#ifdef	__cplusplus
extern "C" {
#endif   
    //#define DEBUG
	//#define MY_DBG
	#define _GNU_SOURCE
    #define HISTORY_LEN 10
    
	#define BUFF_LEN 30
	#define CHARS_LEN 50
	#define FNAME_LEN 30
	#define STD_INPUT 0
	#define STD_OUTPUT 1
	#define TERM_FILENO STDOUT_FILENO
    
	#define STOPPED "stopped"
    #define RUNNING "running"
    #define DONE    "done"

    #include <stdio.h>
    #include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <ctype.h>
	#include <fcntl.h>
	#include <math.h>
	#include <errno.h>
	#include <signal.h>
	#include <stddef.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/wait.h>
	#include <sys/ioctl.h>
	#include <sys/termios.h>
    typedef struct SimpleCmd {
        int isBack;     // 是否后台运行
        char **args;    // 命令及参数
        char *input;    // 输入重定向
        char *output;   // 输出重定向
		int isPipe;		// 标记是否带管道
		struct SimpleCmd *pipeEnd; //管道命令中的后一个
    } SimpleCmd;

    typedef struct History {
        int start;                    //首位置
        int end;                      //末位置
        char cmds[HISTORY_LEN][100];  //历史命令
    } History;

    typedef struct Job {
        pid_t pid;          //进程号
		pid_t pid2;         //改pid_t
		int isBack;			//neal仅用于SIGCHLD的Handler来判断终止的进程是否为后台进程
        char cmd[100];    //命令名
        char state[10];   //作业状态
        struct Job *next; //下一节点指针
    } Job;
    
    extern char inputBuff[100];  //存放输入的命令
    SimpleCmd* parseSimpleCmdStr();
    void init();
    void addHistory(char *history);
    void execute();
    void bg_exec(int pid);
    void fg_exec(int pid);
    void SIGCHLD_Handler();
    void execOuterCmd(SimpleCmd *);
    void execSimpleCmd(SimpleCmd *cmd);
    void getEnvPath(int len, char *buf);
    void waiting(pid_t fgPid1,pid_t fgPid2);
    void releaseCmd(SimpleCmd *);
    void stopJob(pid_t fgPid);
    void rmJob(pid_t pid);
    void addJob(pid_t pid, pid_t pid2, int isBack);
    void justArgs(char *str);
    int str2Pid(char *str, int start, int end);
    int exists(char *cmdFile);

#ifdef	__cplusplus
}
#endif

#endif	/* _global_H */
