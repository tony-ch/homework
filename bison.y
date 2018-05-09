%{
#include "global.h"
int yylex ();
void yyerror ();
SimpleCmd* parse(char[][CHARS_LEN],char[],char[],int,int,int);
void argsExtend(char*);
void ioExtend(char *,int);
char *extend(char *);
int commandDone;
void reset();

char inputBuff[100];//保存命令

char buff[BUFF_LEN][CHARS_LEN];
char inputFile[FNAME_LEN];
char outputFile[FNAME_LEN];

int args_num=0;
char buff_2[BUFF_LEN][CHARS_LEN];
char inputFile_2[FNAME_LEN];
char outputFile_2[FNAME_LEN];
int args_num_2=0;
int isBackFlag=0;
int isPipeFlag=0;
%}
%union{
	char *str;
}
%token <str> STRING
%%
line            :   /* empty */
                    |command                        {execute(); commandDone = 1;}//
;

command         :    fgCommand						
                    |bgCommand						
;

bgCommand		:	fgCommand '&'					{isBackFlag=1;}
;

fgCommand       :    simpleCmd
					|pipeCmd						
;

pipeCmd			:	simpleCmd '|' simpleCmd			{isPipeFlag=1;}
;

simpleCmd       :    args
					|args inputRedirect
					|args outputRedirect
					|args inputRedirect outputRedirect
;

inputRedirect   :   '<' STRING						{ioExtend($2,1);}//1 : input
;

outputRedirect  :   '>' STRING						{ioExtend($2,0);}//0 : output
;

args            :    STRING			{argsExtend($1);}
					|args STRING	{argsExtend($2);}
;

%%

/****************************************************************
                  词法分析函数
****************************************************************/
/*int yylex(){
    //这个函数用来检查inputBuff是否满足lex的定义，实际上并不进行任何操作，初期可略过不看
    int flag;
    char c;
    
	//跳过空格等无用信息
    while(offset < len && (inputBuff[offset] == ' ' || inputBuff[offset] == '\t')){ 
        offset++;
    }
    
    flag = 0;
    while(offset < len){ //循环进行词法分析，返回终结符
        c = inputBuff[offset];
        
        if(c == ' ' || c == '\t'){
            offset++;
            return STRING;
        }
        
        if(c == '<' || c == '>' || c == '&'){
            if(flag == 1){
                flag = 0;
                return STRING;
            }
            offset++;
            return c;
        }
        
        flag = 1;
        offset++;
    }
    
    if(flag == 1){
        return STRING;
    }else{
        return 0;
    }
}
*/
/****************************************************************
                  错误信息执行函数
****************************************************************/
void yyerror()
{
    printf("你输入的命令不正确，请重新输入！\n");
}

/****************************************************************
                  main主函数
****************************************************************/
int main(int argc, char** argv) {
    init(); //初始化环境
	reset();//初始化相应变量
    commandDone = 0;
    while(1){
        char c;
        SIGCHLD_Handler();
		//printf("yourname@computer:%s$ ", get_current_dir_name()); //打印提示符信息
		//printf("B529-sh: %s$", get_current_dir_name());
		printf("\033[1;32mB529-sh >\033[0m");
		//printf("B529-sh>");
		if((c=getchar())==EOF){
			continue;
		}else{
			ungetc(c,stdin);
		}
		//SIGCHLD_Handler();//检测是否有进程退出
        yyparse(); //调用语法分析函数，该函数由yylex()提供当前输入的单词符号

        if(commandDone == 1){ //命令已经执行完成后，添加历史记录信息
            commandDone = 0;
            addHistory(inputBuff);
        }
		reset();//重置相关变量，为下一次处理做准备
     }

    return (EXIT_SUCCESS);
}

void reset()
{
	int i,j;
	//初始化相应变量
    for(i = 0; i<BUFF_LEN; i++){
		for(j=0;j<CHARS_LEN;j++){
		    buff[i][j] = '\0';
			buff_2[i][j] = '\0';
		}
    }
	for(i=0;i<FNAME_LEN;i++){
    	inputFile[i] = '\0';
    	outputFile[i] = '\0';
		inputFile_2[i] = '\0';
		outputFile_2[i] = '\0';
	}
	args_num=0;
	args_num_2=0;
	isBackFlag=0;
	isPipeFlag=0;
	for(i=0;i<100;i++){
		inputBuff[i]='\0';
	}
}

/*******************************************************
                      命令解析
********************************************************/
SimpleCmd* parseSimpleCmdStr(){
	SimpleCmd *cmd;
	SimpleCmd *pipeEnd;
	cmd=parse(buff,inputFile,outputFile,args_num,isBackFlag,isPipeFlag);
	if(1==isPipeFlag){
		pipeEnd=parse(buff_2,inputFile_2,outputFile_2,args_num_2,isBackFlag,0);
		cmd->pipeEnd=pipeEnd;	
	}
	return cmd;
}


SimpleCmd* parse(char abuff[][CHARS_LEN],char ainputFile[],char aoutputFile[],int aargs_num,int aisBackFlag,int aisPipeFlag){
	int i,j;
    SimpleCmd *cmd = (SimpleCmd*)malloc(sizeof(SimpleCmd));
    
	//输入输出重定向为null
    cmd->input = cmd->output = NULL;

	//pipe指针为NULL
	cmd->pipeEnd=NULL;

	//依次为命令名及其各个参数赋值
    cmd->args = (char**)malloc(sizeof(char*) * (aargs_num+1));
    cmd->args[aargs_num] = NULL;//23:35
    for(i = 0; i<aargs_num; i++){//23:35
        j = strlen(abuff[i]);
        cmd->args[i] = (char*)malloc(sizeof(char) * (j + 1));   
        strcpy(cmd->args[i], abuff[i]);
		#ifdef MY_DBG
		printf("add args :cmd->args[%d]:%s\n",i,cmd->args[i]);
		#endif
    }
    
	//如果有输入重定向文件，则为命令的输入重定向变量赋值
    if(strlen(ainputFile) != 0){
        j = strlen(ainputFile);
        cmd->input = (char*)malloc(sizeof(char) * (j + 1));
        strcpy(cmd->input, ainputFile);
    }

    //如果有输出重定向文件，则为命令的输出重定向变量赋值
    if(strlen(aoutputFile) != 0){
        j = strlen(aoutputFile);
        cmd->output = (char*)malloc(sizeof(char) * (j + 1));   
        strcpy(cmd->output, aoutputFile);
    }
	
	//添加后台命令属性
	cmd->isBack=aisBackFlag;

	//添加isPipe属性
	cmd->isPipe=aisPipeFlag;
    #ifdef DEBUG
    printf("********************\n");
	printf("cmd info:\n");
    printf("    isBack: %d\n",cmd->isBack);
	printf("    isPipe: %d\n",cmd->isPipe);
    	for(i = 0; cmd->args[i] != NULL; i++){
    		printf("    args[%d]: %s\n",i,cmd->args[i]);
	}
    printf("    input: %s\n",cmd->input);
    printf("    output: %s\n",cmd->output);
    printf("********************\n");
    #endif
    return cmd;
}


//{if(isPipeFlag==0){strcpy(outputFile,$2);}else{strcpy(outputFile_2,$2);}}
//{if(isPipeFlag==0){strcpy(inputFile,$2);}else{strcpy(inputFile_2,$2);}}
void ioExtend(char * ioArg,int isInput){
	int i=0;
	int j=0;
	char c;
	char *extendArgs=extend(ioArg);
	if(extendArgs==NULL)
		return ;
	for(i=0;i<strlen(extendArgs);i++){		
		if(extendArgs[i]=='\n'){
			c='\0';
			if(i<strlen(extendArgs)-1){
				fprintf(stderr, "模糊的重定向\n");
				break;			
			}
		}else{
			c=extendArgs[i];
		}
		if(0==isPipeFlag && 0==isInput){
			outputFile[j]=c;
		}else if(1==isPipeFlag && 0==isInput){
			outputFile_2[j]=c;;
		}else if(0==isPipeFlag && 1==isInput){
			inputFile[j]=c;;
		}else{// if(1==isPipeFlag && 1==isInput){
			inputFile_2[j]=c;;
		}
		j++;
		if(j>=FNAME_LEN){
			j--;
			fprintf(stderr, "文件名过长\n");		
		}
	}
	#ifdef MY_DBG
	printf("inputFile: %s\n",inputFile);
	printf("outputFile: %s\n",inputFile);	
	#endif
	free(extendArgs);
	extendArgs=NULL;
}

//{if(isPipeFlag==0){strcpy(buff[args_num],$2);args_num++;}else{strcpy(buff_2[args_num_2],$2);args_num_2++;}}
void argsExtend(char *args)
{
	int i=0;
	int j=0;
	char *extendArgs=extend(args);
	if(extendArgs==NULL)
		return ;
	for(i=0;i<strlen(extendArgs);i++){		
		if(extendArgs[i]=='\n'){
			if(0==isPipeFlag){
				buff[args_num][j]='\0';//获得一个args，放到buff中
				args_num++;
				if(args_num>=BUFF_LEN){//如果args太多，不继续存放
					fprintf(stderr, "参数过多\n");
					break;
				}
			}else{
				buff_2[args_num_2][j]='\0';
				args_num_2++;
				if(args_num_2>=BUFF_LEN){//如果args太多，不继续存放
					fprintf(stderr, "参数过多\n");
					break;
				}
			}
			j=0;
		}else{
			if(0==isPipeFlag){
				buff[args_num][j]=extendArgs[i];
			}else{
				buff_2[args_num_2][j]=extendArgs[i];
			}
			j++;
			if(j>=CHARS_LEN){
				fprintf(stderr, "参数过长\n");
				j--;			
			}
		}
	}
	#ifdef MY_DBG
	for(i=0;i<args_num;i++)
		printf("buff[%d]: %s\n",i,buff[i]);	
	for(i=0;i<args_num_2;i++)
		printf("buff_2[%d]: %s\n",i,buff_2[i]);		
	#endif
	free(extendArgs);
	extendArgs=NULL;
}


char *extend(char *args)
{	
////////字符串处理
	int length =strlen(args); 
	char tmp[200];
	int i,j;
	char *patternArg;
	int fd1[2];
	int fd2[2];
	int pipe_pid[2];
	char *result;
	i=0,j=0;
	tmp[j++]='^';//开始符号
	for(i=0;i<length;i++)
	{
		switch(args[i]){
		case '.':tmp[j++]='\\';
			 tmp[j++]='.';
			 break;
		case '*':tmp[j++]='.';
			 tmp[j++]='*';
			 break;
		case '?':tmp[j++]='.';
			 break;
		case '\\':tmp[j++]='\\';//$$$
				 tmp[j++]='\\';//$$$
			 break;
		default :tmp[j++]=args[i];
		}
	}
	tmp[j++]='$';//结束符号
	tmp[j]='\0';
	patternArg=(char*)malloc(sizeof(char)*(j+1));
	memcpy(patternArg,tmp,j+1);
////////管道处理	
	if(pipe(fd1)<0)
	{
		printf("pipe1 failed");
	}
	if(pipe(fd2)<0)
	{
		printf("pipe2 failed");
	}
	if((pipe_pid[0]=fork())==-1)
	{
		printf("fork ls failed");
	}
	if(!pipe_pid[0])
	{
		close(fd1[0]);
		close(STD_OUTPUT);
		dup2(fd1[1],1);  //把标识符1作为管道的另一个写指针
		close(fd1[1]);	 //关闭管道1原有的写指针
		execl("/bin/ls","-a",NULL);
	}
	if((pipe_pid[1]=fork())==-1)
	{
		printf("fork grep failed");
	}
	if(!pipe_pid[1])
	{
		close(fd1[1]);
		close(STD_INPUT);
		dup2(fd1[0],0);  //把标识符0作为管道1的另一个读指针
		close(fd1[0]);	//关闭管道1原有的读指针
		/////////////
		close(fd2[0]);
		dup2(fd2[1],1);  //把标识符1作为管道2的另一个写指针
		close(fd2[1]);	 //关闭管道2原有的写指针
		/////////////
		execl("/bin/grep","-P",patternArg,NULL);
	}
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[1]);  //关闭管道2写端
	result = (char *)malloc(sizeof(char)*3000);
	for(i=0;i<3000;i++){//申请得到的内存里,内容是随机的
		result[i]='\0';
	}
	waitpid(pipe_pid[1],NULL,0);
	waitpid(pipe_pid[0],NULL,0);
	
	if(read(fd2[0],result,3000)>0){  //如果result[]倒数第二项是'\n'(result[]最后一项'\0')，则有扩展匹配项
		//printf("read result:%s\n",result);
		;
	}else{         				//无扩展匹配项,返回原来匹配项
		//strcpy(result,args);
		for(i=0;i<strlen(args);i++){
			result[i]=args[i];			
		}
		result[i]='\n';	
		i++;
		result[i]='\0';

	}
	free(patternArg);
	patternArg=NULL;
	return result;
}

