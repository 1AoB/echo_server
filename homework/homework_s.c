#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h> // for socket(), bind(), connect(), accept(), etc.
#include <netinet/in.h> // for struct sockaddr_in
#include <arpa/inet.h>  // for inet_addr(), inet_ntoa(), etc.

#include <stdio.h>

#include <ctype.h>

// bzero
#include <strings.h>

#define SERVER_PORT 6666

int main(void)
{
    int sock;                       // 代表邮箱fd(连接的套接字)
    struct sockaddr_in server_addr; // 检查bug的能力非常重要

    // 1.使用 socket() 函数创建一个套接字，该套接字用于与客户端进行通信。
    //  美女创建邮箱
    sock = socket(AF_INET, SOCK_STREAM, 0);
    // 2.使用 bzero() 函数将 server_addr 结构体清零，以便初始化套接字地址。
    //  请空标签,写上地址和端口号
    bzero(&server_addr, sizeof(server_addr));

    // 3.将 server_addr 结构体赋值，指定套接字的地址和端口号。
    server_addr.sin_family = AF_INET;                // 选择协议族->IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听本地所以IP地址  long       转换结果是32 位uint32_t整数
    server_addr.sin_port = htons(SERVER_PORT);       // 绑定端口号          short

    // 端口复用
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); // sock监听的文件描述符

    // 4.使用 bind() 函数将套接字绑定到指定地址。
    //  实现标签贴到收信的邮箱上
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 5.使用 listen() 函数将套接字设置为监听状态，等待客户端连接。
    //  把信箱挂到传达室,这样,就可以接收信件了
    listen(sock, 128); // 最多一次性接收128封信

    // 万事具备,只等来信
    printf("Waiting Client 's connection ...\n");

    ///////////////////////上面的对服务端的代码都是模板,根本不用动////////////////////////
    int done = 1;
    while (done) // 需要在循环中不断接受客户端连接请求，并为每个连接创建新的线程或进程进行处理
    {
        ///////////////////////下面的对客户端的代码////////////////////////
        struct sockaddr_in client;
        int client_sock;    // 用于存储新的套接字描述符fd(通信的套接字)
        char client_ip[64]; // 装连接进来的客户端的ip

        char buf[256]; // 读取的内容
        int len;       // 读取的长度

        socklen_t client_addr_len;
        client_addr_len = sizeof(client);

        // 6.使用 accept() 函数接受客户端的连接请求，并返回一个新的套接字与客户端进行通信。
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
        // 因为第三个参数是传入传出参数,所以不能下成下面这样:
        // accept(sock, (struct sockaddr*)&client, &sizeof(client));报错,不能&sizeof()这么写
        // 此时,客户端与服务端已建立好了连接!!!

        // 打印连接到的客户端IP地址和端口号
        printf("client ip:%s\t port:%d\n",
               inet_ntop(AF_INET, // inet_ntop转换结果是字符串
                         &client.sin_addr.s_addr,
                         client_ip,
                         sizeof(client_ip)),
               ntohs(client.sin_port));

        while (1)
        {
            memset(buf, '\0', sizeof buf); // 收之前先把缓存区清空
            //  读取连接到的客户端发送的数据(读进来)
            len = read(client_sock, buf, sizeof(buf) - 1); // 留一格存放字符串结束符,read函数只执行一次
            printf("本次读了len: %d个字符\n", len);
            if (len == 0)
            { // 对方断开了连接
                close(client_sock);
                break;
            }
            buf[len] = '\0';
            printf("Clinet :%s\n", buf); // 打印读到的信息

            fgets(buf, sizeof(buf), stdin); // 讲自己想说的话，写入buf

            len = write(client_sock, buf, strlen(buf));
            printf(" 发送了 %d个字符\n", len);
        }
    }
    return 0;
}
// gcc homework_s.c -o s
// gcc homework_c.c -o c

// Do you like C++?
// Yes, I do.
// Why do you like C++?
// Because I can use it to write programs.
// And you?

// Me too.