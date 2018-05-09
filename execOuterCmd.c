#include "global.h"
extern char cmdBuff[40];
/*执行外部命令*/
void execOuterCmd(SimpleCmd *cmd){
    pid_t pid=0,pid2=0;//neal +pid2
    int pipeIn, pipeOut;
	int pipe_fd[2];
	
	//如果命令不存在
	if(!(
		exists(cmd->args[0]) &&
		((!cmd->isPipe)||exists(cmd->pipeEnd->args[0])) 
		)){
		printf("找不到命令 %15s\n", inputBuff);
		return;
	}
	
	//如果是管道命令，申请管道
	if(pipe(pipe_fd)<0)
	{
		perror("pipe error.\n");
		return;
	}

	if((pid = fork()) < 0){
		perror("fork failed");
		if(cmd->isPipe){
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		return;
	}

	if(pid == 0){ //子进程1
		if(cmd->input != NULL){ //存在输入重定向
			if((pipeIn = open(cmd->input, O_RDONLY, S_IRUSR|S_IWUSR)) == -1){
				printf("不能打开文件 %s！\n", cmd->input);
				exit(EXIT_FAILURE);
			}
			if(dup2(pipeIn, 0) == -1){
				printf("重定向标准输入错误！\n");
				exit(EXIT_FAILURE);
			}
		}

		if(cmd->output != NULL){ //存在输出重定向
			if((pipeOut = open(cmd->output, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1){
				printf("不能打开文件 %s！\n", cmd->output);
				exit(EXIT_FAILURE);
			}
			if(dup2(pipeOut, 1) == -1){
				printf("重定向标准输出错误！\n");
				exit(EXIT_FAILURE);//neal 17:28 不应该是return;
			}
		}
		
		if(cmd->isPipe){
			if(cmd->output != NULL){
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}else{
				close(pipe_fd[0]);
				if(dup2(pipe_fd[1], STDOUT_FILENO) == -1){
					printf("重定向到管道错误！\n");
					exit(EXIT_FAILURE);
				}
				close(pipe_fd[1]);
			}
		}
		
		kill(getpid(),SIGSTOP);//初始化完毕，暂停运行
		
		signal(SIGCHLD, SIG_DFL);//$$
    	signal(SIGTSTP, SIG_DFL);//$$
    	signal(SIGTTOU, SIG_DFL);//$$为了成功调用tcsetpgrp()
		signal(SIGTTIN, SIG_DFL);//$$
		signal(SIGQUIT, SIG_DFL);//$$
		signal(SIGINT, SIG_DFL);//$$
		
		exists(cmd->args[0]);
		justArgs(cmd->args[0]);
		if(execv(cmdBuff, cmd->args) < 0){ //执行命令
			printf("execv failed!\n");
			exit(EXIT_FAILURE);//neal 17:28 不应该是return;
		}
	}
	
	//父进程等待子进程初始化
	int status1;
	waitpid(pid,&status1,WUNTRACED);
	if(WIFEXITED(status1)||WIFSIGNALED(status1)){
		if(cmd->isPipe){
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		return;
	}
	
	//如果是管道命令，父进程继续fork()
	if(cmd->isPipe){
		if((pid2 = fork()) < 0){
			perror("fork failed");
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			kill(pid,SIGKILL);
			return;
		}
		if(pid2==0){//子进程2
			if(cmd->pipeEnd->input != NULL){ //存在输入重定向
				if((pipeIn = open(cmd->pipeEnd->input, O_RDONLY, S_IRUSR|S_IWUSR)) == -1){
					printf("不能打开文件 %s！\n", cmd->pipeEnd->input);
					exit(EXIT_FAILURE);
				}
				if(dup2(pipeIn, 0) == -1){
					printf("重定向标准输入错误！\n");
					exit(EXIT_FAILURE);
				}
			}

			if(cmd->pipeEnd->output != NULL){ //存在输出重定向
				if((pipeOut = open(cmd->pipeEnd->output, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1){
					printf("不能打开文件 %s！\n", cmd->pipeEnd->output);
					exit(EXIT_FAILURE);
				}
				if(dup2(pipeOut, 1) == -1){
					printf("重定向标准输出错误！\n");
					exit(EXIT_FAILURE);//neal 17:28 不应该是return;
				}
			}
			
			if(cmd->isPipe){
				if(cmd->pipeEnd->input != NULL){
					close(pipe_fd[0]);
					close(pipe_fd[1]);
				}else{
					close(pipe_fd[1]);
					if(dup2(pipe_fd[0], STDIN_FILENO) == -1){
						printf("重定向到管道错误！\n");
						exit(EXIT_FAILURE);
					}
					close(pipe_fd[0]);
				}
			}
			
			kill(getpid(),SIGSTOP);//初始化完毕，暂停运行
			
			signal(SIGCHLD, SIG_DFL);//$$恢复默认信号处理方式
			signal(SIGTSTP, SIG_DFL);//$$
			signal(SIGTTOU, SIG_DFL);//$$为了成功调用tcsetpgrp()
			signal(SIGTTIN, SIG_DFL);//$$
			signal(SIGQUIT, SIG_DFL);//$$
			signal(SIGINT, SIG_DFL);//$$
			
			exists(cmd->pipeEnd->args[0]);
			justArgs(cmd->pipeEnd->args[0]);
			if(execv(cmdBuff, cmd->pipeEnd->args) < 0){ //执行命令
				printf("execv failed!\n");
				exit(EXIT_FAILURE);//neal 17:28 不应该是return;
			}
		}
		
		int status2;
		waitpid(pid2,&status2,WUNTRACED);
		if(WIFEXITED(status2)||WIFSIGNALED(status2)){
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			kill(pid,SIGKILL);
			return;
		}
	}
	
	//父进程 设置子进程进程组
	setpgid(pid,pid);
	if(cmd->isPipe){
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		setpgid(pid2,pid);
	}

	if(!cmd->isBack){//前台任务，设为前台进程组
		if(tcsetpgrp(TERM_FILENO, pid)<0){
			perror("tcsetpgrp():");
			kill(pid,SIGKILL);
			kill(pid2,SIGKILL);
			return;
		}
	}else{//后台任务
		printf("[%d]\t%s\t\t%s\n", pid, RUNNING, inputBuff);
	}
	
	addJob(pid,pid2,cmd->isBack);
	kill(pid,SIGCONT);
	if(cmd->isPipe){
		kill(pid2,SIGCONT);
	}
	
	if(!cmd->isBack)
		waiting(pid,pid2);
}
