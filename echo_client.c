#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h> // for socket(), bind(), connect(), accept(), etc.
#include <netinet/in.h> // for struct sockaddr_in

#define SERVER_PORT 6666
#define SERVER_IP "127.0.0.1"

/*
 * 客户端:在客户端中，连接套接字和通信套接字是同一个变量
 * 连接的套接字:sockfd,使用socket()创建
 * 通信的套接字:sockfd,connect() 函数将会使用 sockfd 连接到服务器端的套接字
 */

// argc 是命令行参数的个数，其值包括程序名称和命令行参数。

/*
argv 是一个指向指针的数组，每个指针指向一个命令行参数的字符串，其中 argv[0] 指向程序名称，argv[1] 指向第一个命令行参数，以此类推。
例如，在命令行运行程序时输入 ./example arg1 arg2 arg3，则 argc 的值为 4，argv 的值为：
argv[0] = "./example"
argv[1] = "arg1"
argv[2] = "arg2"
argv[3] = "arg3"
*/
int main(int argc, char *argv[])
{
    /********处理main函数的参数*********/
    int sockfd;
    char *message;
    struct sockaddr_in server_addr;
    int n;
    char buf[64];

    if (argc != 2)
    {
        fputs("Usage: ./echo_client message\n", stderr);
        exit(1);
    }

    message = argv[1];
    printf("message : %s\n", message);
    /************************/
    // 创建连接的套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM:流式传输协议,0:表示使用默认协议。

    memset(&server_addr, '\0', sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    // inet_ntop这个函数名,n指的是网络直接序的ip,p指的是主机字节序的ip
    // inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip))
    inet_pton(AF_INET,
              "127.0.0.1",
              &server_addr.sin_addr);
    // inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    // 更改为通信的套接字
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        exit(1);
    }

    if (write(sockfd, message, strlen(message)) < 0) // strlen(message)遇到'\0'才停止计算长度,sizeof计算的容器大小
    {
        perror("write failed");
        exit(1);
    }

    n = read(sockfd, buf, sizeof(buf) - 1);

    if (n > 0)
    {
        buf[n] = '\0';
        printf("receive :%s\n", buf);
    }
    else
    {
        perror("error!!!");
    }

    printf("client finished.\n");
    close(sockfd);
    return 0;
}
// gcc echo_client.c -o echo_client
