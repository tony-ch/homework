#include "global.h"
void releaseCmd(SimpleCmd *);
char *envPath[10], cmdBuff[40];  //外部命令的存放路径及读取外部命令的缓冲空间
History history;                 //历史命令
Job *head = NULL;                //作业头指针
/*******************************************************
                  工具以及辅助方法
********************************************************/
/*判断命令是否存在*/
int exists(char *cmdFile){
    int i = 0;
    if((cmdFile[0] == '/' || cmdFile[0] == '.') && access(cmdFile, F_OK) == 0){ //命令在当前目录
        strcpy(cmdBuff, cmdFile);
        return 1;
    }else{  //查找ysh.conf文件中指定的目录，确定命令是否存在
        while(envPath[i] != NULL){ //查找路径已在初始化时设置在envPath[i]中
            strcpy(cmdBuff, envPath[i]);
            strcat(cmdBuff, cmdFile);

            if(access(cmdBuff, F_OK) == 0){ //命令文件被找到
                return 1;
            }

            i++;
        }
    }

    return 0;
}

/*将字符串转换为整型的Pid*/
int str2Pid(char *str, int start, int end){
    int i, j;
    char chs[20];

    for(i = start, j= 0; i < end; i++, j++){
        if(str[i] < '0' || str[i] > '9'){
            return -1;
        }else{
            chs[j] = str[i];
        }
    }
    chs[j] = '\0';

    return atoi(chs);
}

/*调整部分外部命令的格式*/
void justArgs(char *str){
    int i, j, len;
    len = strlen(str);

    for(i = 0, j = -1; i < len; i++){
        if(str[i] == '/'){
            j = i;
        }
    }

    if(j != -1){ //找到符号'/'
        for(i = 0, j++; j < len; i++, j++){
            str[i] = str[j];
        }
        str[i] = '\0';
    }
}


/*释放环境变量空间*/
void release(){
    int i;
    for(i = 0; strlen(envPath[i]) > 0; i++){
        free(envPath[i]);
    }
}

/*******************************************************
                  信号以及jobs相关
********************************************************/
/*添加新的作业*/
void addJob(pid_t pid, pid_t pid2, int isBack){
    Job *now = NULL, *last = NULL, *job = (Job*)malloc(sizeof(Job));

	//初始化新的job
    job->pid = pid;
	job->pid2= pid2;//neal 23:08 +pid2$
	job->isBack=isBack;//neal$
    strcpy(job->cmd, inputBuff);
    strcpy(job->state, RUNNING);
    job->next = NULL;

    if(head == NULL){ //若是第一个job，则设置为头指针
        head = job;
    }else{ //否则，根据pid将新的job插入到链表的合适位置
		now = head;
		while(now != NULL && now->pid < pid){
			last = now;
			now = now->next;
		}
        last->next = job;
        job->next = now;
    }
    
	//return job;
}

/*移除一个作业*/
void rmJob(pid_t pid){
    Job *now = NULL, *last = NULL;
/*//$
    if(ingnore == 1){
        ingnore = 0;
        return;
    }

    pid = sip->si_pid;
*/
    now = head;
	while(now != NULL && now->pid < pid){
		last = now;
		now = now->next;
	}

    if(now == NULL){ //作业不存在，则不进行处理直接返回
        return;
    }

	//开始移除该作业
    if(now == head){
        head = now->next;
    }else{
        last->next = now->next;
    }
    free(now);
}

/*组合键命令ctrl+z*/
/*
void ctrl_Z(){
    Job *now = NULL;

    if(fgPid == 0){ //前台没有作业则直接返回
        return;
    }

    //SIGCHLD信号产生自ctrl+z
    ingnore = 1;

	now = head;
	while(now != NULL && now->pid != fgPid)
		now = now->next;

    if(now == NULL){ //未找到前台作业，则根据fgPid添加前台作业
        now = addJob(fgPid);
    }

	//修改前台作业的状态及相应的命令格式，并打印提示信息
    strcpy(now->state, STOPPED);
    now->cmd[strlen(now->cmd)] = '&';
    now->cmd[strlen(now->cmd) + 1] = '\0';
    printf("[%d]\t%s\t\t%s\n", now->pid, now->state, now->cmd);

	//发送SIGSTOP信号给正在前台运作的工作，将其停止
    kill(fgPid, SIGSTOP);
    fgPid = 0;
}
*/
/*打印暂停的作业 原名ctrl_Z*/
void stopJob(pid_t fgPid){
    Job *now = NULL;

    if(fgPid == 0){ //前台没有作业则直接返回 TODO
        return;
    }

	now = head;
	while(now != NULL && now->pid != fgPid)
		now = now->next;

    if(now == NULL){ //未找到前台作业，则根据fgPid添加前台作业
		return;
    	#ifdef MY_DBG
        printf("DEBUG:job not found.\n");
        #endif
    }

	//修改前台作业的状态及相应的命令格式，并打印提示信息
    strcpy(now->state, STOPPED);
    now->cmd[strlen(now->cmd)] = '&';
    now->cmd[strlen(now->cmd) + 1] = '\0';
    printf("[%d]\t%s\t\t%s\n", now->pid, now->state, now->cmd);
}

/*SIGCHLD_Handler*/
void SIGCHLD_Handler()
{
	//本函数检测结束运行的Job，并调用rmJob函数除去作业
	Job *now;
	pid_t toRemove;
	
	for(now=head; now!=NULL; ){//对jobs列表中的每一项
		waitpid(now->pid, NULL, WNOHANG);//$$$
		if(now->pid2)//$$$
			waitpid(now->pid2, NULL, WNOHANG);//$$$
		//(kill(now->pid,0)<0) && ( (now->pid2==0)||(kill(now->pid,0)<0) 
		if( kill(-now->pid,0)<0 ){//$
			toRemove=now->pid;
			now=now->next;
			rmJob(toRemove);
		}else{
			now=now->next;
		}
	}
}


/*fg命令*/
void fg_exec(int pid){
    Job *now = NULL;
	int i;

    //SIGCHLD信号产生自此函数
    //ingnore = 1;

	//根据pid查找作业
    now = head;
	while(now != NULL && now->pid != pid)
		now = now->next;

    if(now == NULL){ //未找到作业
        printf("pid为%7d 的作业不存在！\n", pid);
        return;
    }

    //记录前台作业的pid，修改对应作业状态
    //fgPid = now->pid;
    now->isBack=0;//
    strcpy(now->state, RUNNING);

    //signal(SIGTSTP, ctrl_Z); //设置signal信号，为下一次按下组合键Ctrl+Z做准备
    i = strlen(now->cmd) - 1;
    while(i >= 0 && now->cmd[i] != '&')
		i--;
    now->cmd[i] = '\0';

    printf("%s\n", now->cmd);
	tcsetpgrp(TERM_FILENO,now->pid);//neal 将其进程组提到前台
    kill(-now->pid, SIGCONT); //向对象作业发送SIGCONT信号，使其运行//$$
    waiting(now->pid, now->pid2);//neal
}

/*bg命令*///$
void bg_exec(int pid){
    Job *now = NULL;

    /*//SIGCHLD信号产生自此函数 neal
    ingnore = 1;//TODO*/

	//根据pid查找作业
	now = head;
    while(now != NULL && now->pid != pid)
		now = now->next;

    if(now == NULL){ //未找到作业
        printf("pid为%7d 的作业不存在！\n", pid);
        return;
    }

	now->isBack=1;//neal
    strcpy(now->state, RUNNING); //修改对象作业的状态
    printf("[%d]\t%s\t\t%s\n", now->pid, now->state, now->cmd);

    kill(-now->pid, SIGCONT); //向对象作业发送SIGCONT信号，使其运行$$
}

/*******************************************************
                    命令历史记录
********************************************************/
void addHistory(char *cmd){
    if(history.end == -1){ //第一次使用history命令
        history.end = 0;
        strcpy(history.cmds[history.end], cmd);
        return;
	}

    history.end = (history.end + 1)%HISTORY_LEN; //end前移一位
    strcpy(history.cmds[history.end], cmd); //将命令拷贝到end指向的数组中

    if(history.end == history.start){ //end和start指向同一位置
        history.start = (history.start + 1)%HISTORY_LEN; //start前移一位
    }
}

/*******************************************************
                     初始化环境
********************************************************/
/*通过路径文件获取环境路径*/
void getEnvPath(int len, char *buf){
    int i, j, pathIndex = 0, temp;//
    char path[40];

    for(i = 0, j = 0; i < len; i++){
        if(buf[i] == ':'){ //将以冒号(:)分隔的查找路径分别设置到envPath[]中
            if(path[j-1] != '/'){
                path[j++] = '/';
            }
            path[j] = '\0';
            j = 0;

            temp = strlen(path);
            envPath[pathIndex] = (char*)malloc(sizeof(char) * (temp + 1));
            strcpy(envPath[pathIndex], path);

            pathIndex++;
        }else{
            path[j++] = buf[i];
        }
    }

    envPath[pathIndex] = NULL;
}

/*初始化操作*/
void init(){
    int fd, len;
    char c, buf[80];

	//打开查找路径文件ysh.conf
    if((fd = open("ysh.conf", O_RDONLY, 660)) == -1){
        perror("init environment failed\n");
        exit(1);
    }

	//初始化history链表
    history.end = -1;
    history.start = 0;

    len = 0;
	//将路径文件内容依次读入到buf[]中
    while(read(fd, &c, 1) != 0){
        buf[len++] = c;
    }
    buf[len] = '\0';

    //将环境路径存入envPath[]
    getEnvPath(len, buf);
	
	//检测是否为前台
    while(getpgrp()!=tcgetpgrp(TERM_FILENO))
		kill(0,SIGTTIN);
    //将shell放入自己的进程组
    if(setpgid(0,0)<0){
      perror("setpgid():");
      exit(EXIT_FAILURE);
    }
    
    //注册信号
    /*struct sigaction action;
    action.sa_sigaction = rmJob;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO|SA_RESTART;
    sigaction(SIGCHLD, &action, NULL);
    signal(SIGTSTP, ctrl_Z);*/
    signal(SIGCHLD, SIG_IGN);//neal
    signal(SIGTSTP, SIG_IGN);//neal

    //neal 16:54 注册其他信号$
    signal(SIGTTOU, SIG_IGN);//为了成功调用tcsetpgrp()
    signal(SIGTTIN, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    //neal 16:56 将shell提q到前台$
    if(tcsetpgrp(TERM_FILENO, getpgid(0))<0){
      perror("tcsetpgrp():");
      exit(EXIT_FAILURE);
    }
}


/*******************************************************
                      命令执行
********************************************************/
/*执行外部命令*/
/*
void execOuterCmd(SimpleCmd *cmd){
    pid_t pid;
    int pipeIn, pipeOut;

    if(exists(cmd->args[0])){ //命令存在
		if(cmd->isBack){
			signal(SIGUSR1, sonBegin);
		}
        if((pid = fork()) < 0){
            perror("fork failed");
            return;
        }

        if(pid == 0){ //子进程
            if(cmd->input != NULL){ //存在输入重定向
                if((pipeIn = open(cmd->input, O_RDONLY, S_IRUSR|S_IWUSR)) == -1){
                    printf("不能打开文件 %s！\n", cmd->input);
                    return;
                }
                if(dup2(pipeIn, 0) == -1){
                    printf("重定向标准输入错误！\n");
                    return;
                }
            }

            if(cmd->output != NULL){ //存在输出重定向
                if((pipeOut = open(cmd->output, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1){
                    printf("不能打开文件 %s！\n", cmd->output);
                    return ;
                }
                if(dup2(pipeOut, 1) == -1){
                    printf("重定向标准输出错误！\n");
                    return;
                }
            }

            if(cmd->isBack){ //若是后台运行命令，等待父进程增加作业
                kill(getppid(), SIGUSR1);//通知父进程子进程可以开始工作
				signal(SIGUSR2, fatherCatch); //父进程收到信号，跳出下面的循环
                while(fatherCatchFlag == 0) ; //等待父进程SIGUSR2信号，表示作业已加到链表中
                fatherCatchFlag = 0; //置0，为下一命令做准备

                printf("[%d]\t%s\t\t%s\n", getpid(), RUNNING, inputBuff);

				kill(getppid(), SIGUSR2);//通知父进程子进程完成工作
            }

            justArgs(cmd->args[0]);
            if(execv(cmdBuff, cmd->args) < 0){ //执行命令
                printf("execv failed!\n");
                return;
            }
        }
		else{ //父进程
            if(cmd ->isBack){ //后台命令
                //signal(SIGUSR1, sonBegin);
                while(sonBeginFlag == 0) ;//等待子进程通知开始
                sonBeginFlag = 0;

                fgPid = 0; //pid置0，为下一命令做准备
                addJob(pid); //增加新的作业
                kill(pid, SIGUSR2); //向子进程发信号，表示作业已加入


                signal(SIGUSR2, sonEnd);//等待子进程通知结束
                while(sonEndFlag == 0) ;
                sonEndFlag = 0;
            }else{ //非后台命令
                fgPid = pid;
                waitpid(pid, NULL, WUNTRACED);//CTRL+Z!!!!
            }
		}
    }else{ //命令不存在
        printf("找不到命令 %15s\n", inputBuff);
    }
}
*/

/*neal 前台等待函数，仅负责检测停止*/
void waiting(pid_t fgPid1,pid_t fgPid2){
    int stopped=0;//printf("waiting()\n");
    if(fgPid2==0){//非管道，只需要等待子进程结束或停止
        int status;
        waitpid(fgPid1, &status, WUNTRACED);
        stopped=WIFSTOPPED(status);
    }
    else{//管道指令，需要等待两个子进程都结束或停止
        int status1,status2;//不能赋值为-1$$
		while(1){
			waitpid(fgPid1, &status1, WUNTRACED);
			waitpid(fgPid2, &status2, WUNTRACED);
			
			waitpid(fgPid1, &status1, WUNTRACED|WNOHANG);
			waitpid(fgPid2, &status2, WUNTRACED|WNOHANG);
			if( (WIFEXITED(status1)||WIFSIGNALED(status1)||WIFSTOPPED(status1))&&
				(WIFEXITED(status1)||WIFSIGNALED(status1)||WIFSTOPPED(status1)) )
				break;
		}
		stopped=WIFSTOPPED(status1)&&WIFSTOPPED(status2);//###
    }
    if(stopped)
        stopJob(fgPid1);
	//前台进程等待完毕，将shell放入前台
	if(tcsetpgrp(TERM_FILENO, getpgid(0))<0){
		perror("tcsetpgrp():");
		exit(EXIT_FAILURE);
    }
}

/*执行命令*/
void execSimpleCmd(SimpleCmd *cmd){
    int i, pid;
    char *temp;
    Job *now = NULL;

	//neal 22:42 确认管道不是内部命令
    if(cmd->isPipe)
    {
        if(
            strcmp(cmd->args[0], "exit") == 0 ||
            strcmp(cmd->args[0], "history") == 0 ||
            strcmp(cmd->args[0], "jobs") == 0 ||
            strcmp(cmd->args[0], "cd") == 0 ||
            strcmp(cmd->args[0], "fg") == 0 ||
            strcmp(cmd->args[0], "bg") == 0 ||
            strcmp(cmd->args[0], "type") == 0||//$$
            strcmp(cmd->args[0], "pwd") == 0||//$$
            
            strcmp(cmd->pipeEnd->args[0], "exit") == 0 ||
            strcmp(cmd->pipeEnd->args[0], "history") == 0 ||
            strcmp(cmd->pipeEnd->args[0], "jobs") == 0 ||
            strcmp(cmd->pipeEnd->args[0], "cd") == 0 ||
            strcmp(cmd->pipeEnd->args[0], "fg") == 0 ||
            strcmp(cmd->pipeEnd->args[0], "bg") == 0 ||        
            strcmp(cmd->pipeEnd->args[0], "type") == 0||//$$
            strcmp(cmd->pipeEnd->args[0], "pwd") == 0       )//$$
        {
            fprintf(stderr,"内部命令不支持管道");
            return;
        }
    }//$

    if(strcmp(cmd->args[0], "exit") == 0) { //exit命令
    	if(head != NULL){
    		printf("尚有进程正在执行\n");//$$
    		return;
    	}
        exit(0);
    } else if (strcmp(cmd->args[0], "history") == 0) { //history命令
        if(history.end == -1){
            printf("尚未执行任何命令\n");
            return;
        }
        i = history.start;
        do {
            printf("%s\n", history.cmds[i]);
            i = (i + 1)%HISTORY_LEN;
        } while(i != (history.end + 1)%HISTORY_LEN);
    } else if (strcmp(cmd->args[0], "jobs") == 0) { //jobs命令
        if(head == NULL){
            printf("尚无任何作业\n");
        } else {
            printf("index\tpid\tstate\t\tcommand\n");
            for(i = 1, now = head; now != NULL; now = now->next, i++){
                printf("%d\t%d\t%s\t\t%s\n", i, now->pid, now->state, now->cmd);
            }
        }
    } else if (strcmp(cmd->args[0], "cd") == 0) { //cd命令
        temp = cmd->args[1];
        if(temp != NULL){
            if(chdir(temp) < 0){
                printf("cd; %s 错误的文件名或文件夹名！\n", temp);
            }
        }
    } else if (strcmp(cmd->args[0], "fg") == 0) { //fg命令
        temp = cmd->args[1];
        if(temp != NULL && temp[0] == '%'){
            pid = str2Pid(temp, 1, strlen(temp));
            if(pid != -1){
                fg_exec(pid);
            }
        }else{
            printf("fg; 参数不合法，正确格式为：fg %%<int>\n");
        }
    } else if (strcmp(cmd->args[0], "bg") == 0) { //bg命令
        temp = cmd->args[1];
        if(temp != NULL && temp[0] == '%'){
            pid = str2Pid(temp, 1, strlen(temp));

            if(pid != -1){
                bg_exec(pid);
            }
        }
		else{
            printf("bg; 参数不合法，正确格式为：bg %%<int>\n");
        }
    }else if(strcmp(cmd->args[0], "type") == 0)//type命令$$^
    {
       i=1;
       while(cmd->args[i]!=NULL)
     	{
		   if(strcmp(cmd->args[i], "type")==0||strcmp(cmd->args[i], "exit")==0||
		      strcmp(cmd->args[i], "history")==0||strcmp(cmd->args[i], "jobs")==0||
		      strcmp(cmd->args[i], "cd")==0||strcmp(cmd->args[i], "fg")==0||
		      strcmp(cmd->args[i], "bg")==0||strcmp(cmd->args[i], "pwd")==0)
		    	printf("%s is a shell builtin\n",cmd->args[i]);
		   else
				if(exists(cmd->args[i]))
				{
				   justArgs(cmd->args[i]);
				   printf("%s is %s\n",cmd->args[i],cmdBuff);
				}
				else 
		       		printf("bash: type: %s: 未找到\n",cmd->args[i]);
		   i++;
     	}
    }
    else   if(strcmp(cmd->args[0], "pwd") == 0)//pwd命令
    {
		char *dir=get_current_dir_name();
		printf("%s\n",dir);
		free(dir);
    } else{ //外部命令$$$
        execOuterCmd(cmd);
    }
}
void releaseCmd(SimpleCmd *cmd){
	int i;
	for(i = 0; cmd->args[i] != NULL; i++){
        free(cmd->args[i]);
    }
	if(cmd->input!=NULL){
		free(cmd->input);
	}
	if(cmd->output!=NULL){
		free(cmd->output);	
	}
	if(1==cmd->isPipe){
		for(i = 0; cmd->pipeEnd->args[i] != NULL; i++){
        	free(cmd->pipeEnd->args[i]);
		}
		if(cmd->pipeEnd->input!=NULL){
			free(cmd->pipeEnd->input);
		}
		if(cmd->pipeEnd->output!=NULL){
			free(cmd->pipeEnd->output);	
		}
		free(cmd->pipeEnd);
		#ifdef MY_DBG
			printf("free cmd->pipeEnd success\n");
		#endif
	}
	free(cmd);
	#ifdef MY_DBG
		printf("free cmd success\n");
	#endif
}
/*******************************************************
                     命令执行接口
********************************************************/
void execute(){
	SimpleCmd *cmd = parseSimpleCmdStr();
    execSimpleCmd(cmd);
	releaseCmd(cmd);
}
