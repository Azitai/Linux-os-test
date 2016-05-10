#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  PORT 1234
#define  BACKLOG 1

int main()
{
    int  listenfd, connectfd;  // listenfd为监听的socket文件描述符。connectfd为具体链接的标识符。
    struct  sockaddr_in server;  // 定义server端的地址信息
    struct  sockaddr_in client;
    socklen_t  addrlen;  //
    // 调用socket系统调用创建一个套接字文件，并返回文件描述符，等同于创建一个文件。
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Creating  socket failed.");
        exit(1);
    }
    int opt = SO_REUSEADDR;
    setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bzero(&server,sizeof(server));  // 初始化server
    server.sin_family=AF_INET;
    server.sin_port= htons(PORT);
    server.sin_addr.s_addr= htonl (INADDR_ANY);
    // 调用bind函数，将套接字与本地IP和端口绑定在一起，当外界信息到来时，内核会将请求
    // 分发到该套接字上。
    if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binderror.");
        exit(1);
    }   
    // 调用listen函数，开始监听该文件描述符上是否有请求到来
    // 第二个参数规定了内核为此套接口排队的最大连接个数
    // 内核要维护两个队列：以完成连接队列和未完成连接队列。
    // 注：在此阶段，当客户端有请求到来时，网络内核会完成三次握手，并将完成
    // 握手的连接放入已完成队列中。
    if(listen(listenfd, BACKLOG)== -1){  /* calls listen() */
        perror("listen()error\n");
        exit(1);
    }
    addrlen = sizeof(client);
    // accept函数从已完成链接队列中，拿出一个链接，将该连接客户端的ip及端口等信息填写
    // 在第二个参数中，返回一个全新的套接口描述字用于标识该唯一tcp链接，下面会通过该fd标识与客户端进行通信。
    // 如果已完成链接队列为空，则进入睡眠状态。
    // 也就是说，在服务器端，一旦完成了三次握手，就会为该链接创建一个单独的套接字，用于和客户端进行通信。
    // 内核会为connectfd与客户端的地址信息建立映射关系。那么，不同客户端的请求又是如何到达不同套接字的呢？？
    // 是先到listenfd套接字，然后再分发给不同的套接字吗？
    // accept函数负责从链接队列中获取一个链接。如果要继续从该listenfd获取链接，则需要使用多线程技术？？
    // 在多线程中通过accept从队列中拿链接？？
    if((connectfd = accept(listenfd,(struct sockaddr*)&client,&addrlen))==-1) {
        perror("accept()error\n");
        exit(1);
    }
    printf("Yougot a connection from cient's ip is %s, prot is %d\n",inet_ntoa(client.sin_addr),htons(client.sin_port));
    send(connectfd, "Welcometo my server.\n",22,0);
    close(connectfd);
    close(listenfd);
    return 0;
}

