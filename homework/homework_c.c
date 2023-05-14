#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h> // for socket(), bind(), connect(), accept(), etc.
#include <netinet/in.h> // for struct sockaddr_in

#define SERVER_PORT 6666
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[])
{
    /********处理main函数的参数*********/
    int sockfd;
    struct sockaddr_in server_addr;
    int n;
    char buf[256];

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
    while (1) // 开始通信
    {
        fgets(buf, sizeof(buf), stdin); // 讲自己想说的话，写入buf

        n = write(sockfd, buf, strlen(buf)); // strlen(message)遇到'\0'才停止计算长度,sizeof计算的容器大小
        printf(" 发送了 %d个字符\n", n);

        memset(buf, '\0', sizeof buf); // 收之前先把缓存区清空
        n = read(sockfd, buf, sizeof(buf) - 1);
        printf("本次读了n: %d个字符 \n", n);
        if (n == 0)
        { // 对方断开了连接
            close(sockfd);
            break;
        }
        buf[n] = '\0';
        printf("Server :%s\n", buf);
    }

    printf("client finished.\n");
    close(sockfd);
    return 0;
}
// gcc homework_c.c -o c