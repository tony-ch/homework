完成功能：
    从控制台或者文件读入逻辑表达式（可自定义逻辑连接词）
        可以有多个语句，多个自定义连接词，自定义连接词的位置不作限定
        自定义逻辑连接词的格式# 名字 元数 真值表 名字 元数 真值表 ...
        会进行语法检查，输出行号和列号
    输出读到的自定义连接词
    判断逻辑语句成立的条件
        会输出成立的条件
    对于输入中不超过3元的自定义逻辑，进行完全集判断，输出能推导出的所有真值表，达到16种即为完全集
        会输出每个真值表的推导公式

各个目录的作用：
    bin/ CodeBlocks环境下用MinGW GCC编译得到的可执行文件，运行时需要加载KERNEL32.dll和msvcrt.dll，Windows操作系统应该已经自带
    src/ 源代码文件和Cmake文件
    codeblocks/ CodeBlocks工程文件
    vsproject/ VS2017工程文件

运行bin/logic.exe的注意事项：
    可以将输入文件的路径通过命令行参数传入，如
        > logic.exe ../input_test.txt
    如果路径没有通过命令行传入，最开始会要求输入文件路径，每次运行只读入一个文件
    如果路径没有通过命令行传入，直接输入回车则从控制台读入
    注意：从windows的cmd控制台无法读入¬和↔，读入程序的会是?,会被当做非法字符，这种情况下从文件读入没有问题


测试所有样例的简单方法：
    运行bin/目录下的run.bat可以运行所有测试样例(bin/batch/目录的test1.txt~test19.txt)
    一个样例的输出如下：（输入回车则继续运行下一个样例，Ctrl+C中止运行）
    ####################################### LOGIC ########################################

    ######## inputs ########
    (p)⊕(qr) f((p),g(q, r)) # f 2 0110 g 2 1001
    ###### functions #######
    FUNC: name:f,para_num:2
    0 1 1 0
    FUNC: name:g,para_num:2
    1 0 0 1

    ######### exps #########
    (p)⊕(qr)
    p: 0  q: 0  r: 0
    p: 1  q: 1  r: 0
    p: 1  q: 0  r: 1
    p: 0  q: 1  r: 1
    [INFO] RUN: 4 true, 8 condition total
    f((p),g(q,r))
    p: 0  q: 0  r: 0
    p: 1  q: 1  r: 0
    p: 1  q: 0  r: 1
    p: 0  q: 1  r: 1
    [INFO] RUN: 4 true, 8 condition total

    ### check functions ####
    0 0000
    p 0011
    q 0101
    f(p,q) 0110
    f(q,f(p,1)) 1001
    f(q,1) 1010
    f(p,1) 1100
    1 1111
    get 8, 16 total
    not complete set
    press enter to end process ...