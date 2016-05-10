#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#define  PORT 1234
#define  MAXDATASIZE 100

int main(int argc, char *argv[])
{
    int  sockfd, num;
    char  buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in server;
    // 第一个参数默认为应用程序名称。此处第二个参数设计为传入服务器端的地址
    if (argc!=2) {
        printf("Usage:%s <IP Address>\n",argv[0]);
        exit(1);
    }
    // 通过ip找服务器
    if((he=gethostbyname(argv[1]))==NULL){
        printf("gethostbyname()error\n");
        exit(1);
    }
    // 创建客户端的socket套接字，并返回文件描述符
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1){
        printf("socket()error\n");
        exit(1);
    }
    bzero(&server,sizeof(server));
    server.sin_family= AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr =*((struct in_addr *)he->h_addr);  //拿到server地址，此处需要进行一个简单的转化
    // 向server端发送链接请求，触发三次握手的过程。三次握手完毕，服务器端的accept才能从
    // 内核的已完成链接队列中取出该链接。
    if(connect(sockfd,(struct sockaddr *)&server,sizeof(server))==-1){
        printf("connect()error\n");
        exit(1);
    }
    // 此处接受服务端的响应信息，在未拿到信息时，进入阻塞状态。
    if((num=recv(sockfd,buf,MAXDATASIZE,0)) == -1){
        printf("recv() error\n");
        exit(1);
    }
    buf[num-1]='\0';
    printf("Server Message: %s\n",buf);
    close(sockfd);
    return 0;
}

