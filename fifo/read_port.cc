/*
 * *功能描述：通过有名管道实现两个进程之间的双向通信，实现对话功能。本部分从fifo1中写入信息，
 * *       发送给另一个程序，同时用子进程从另一个管道fifo2中等待读取信息.
 * */
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define N 20
int main(void)
{
    int i;
    char getstr[N];
    char redstr[N];
    char fname1[]="./fmsg1";
    int fid1, fifo_create_status[2];
    pid_t pid;
    
	// 注意：读程序中并不需要创建有名管道，该管道是在写程序
	// 中打开，并且，只有在写程序启动后，读程序才能成功
	// 启动。

//	// 根据文件名创建有名管道，第二个参数为权限设置.
//    fifo_create_status[0] = mkfifo(fname1, 0666);
//    
//	// 判断是否建成fifo管道
//    if (fifo_create_status[0] < 0) {
//        perror("mkfifo");
//        exit(-1);       //错误并退出
//    }
//    else {
//        printf("fifo made succeed!\n"); //test
//    }
    
	fid1 = open(fname1, O_RDONLY);
    if (fid1 < 0) {
    	perror("file opened error");
    	return 0;
    }
    printf("file opend succeed!\n");

    int count = 0;
    do {
        printf("第%d次进入子进程，读取父进程发送来的消息\n", ++count);
        memset(redstr, 0, N);
        if((read(fid1, redstr, N*sizeof(char))) <= 0) {
            perror("child read error");
            exit(-1);
        } else {
            // 写入时，带有回车符。 
            printf("收到来自父进程发送的信息：%s", redstr);
            // strlen不会算入最后的结束符\0,此处清除掉最后的回车符
            // 回车即是确定提交上次输入，又是下次输入的一个字符，所以需要清楚掉。
            redstr[strlen(redstr)-1] = '\0';
                        if((strcmp(redstr,"END") == 0) || (strcmp(redstr,"end")==0))    //输入end退出
            {
            	printf("read进程收到write的结束信号，退出程序.\n");
            	break;
            }
        }
    } while(1);

    close(fid1);
    unlink(fname1);
    return 0;
}
