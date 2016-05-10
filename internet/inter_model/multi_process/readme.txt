**多进程并行处理

**核心代码：
bind(srvfd);
listen(srvfd);
for(;;){
    clifd = accept(srvfd,...); //开始接受客户端来的连接
    ret = fork();
    switch( ret )
    {
        case -1 :
            do_err_handler();
            break;
        case 0  :   // 子进程
            client_handler(clifd);
            break ;
        default :   // 父进程
            close(clifd);
            continue ; 
    }
}
//======================================================
void client_handler(clifd){
    read(clifd,buf,...);       //从客户端读取数据
    dosomthingonbuf(buf);  
    write(clifd,buf)          //发送数据到客户端
}


**上述程序在accept系统调用时，如果没有客户端来建立连接，则会阻塞在accept处。
  一旦某个客户端连接建立起来，则立即开启一个新的进程来处理与这个客户的数据交
  互。避免程序阻塞在read调用，而影响其他客户端的连接。

