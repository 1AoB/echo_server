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
    int sock;                       // 代表邮箱
    struct sockaddr_in server_addr; // 检查bug的能力非常重要

    // 1.美女创建邮箱
    sock = socket(AF_INET, SOCK_STREAM, 0);
    // 2.请空标签,写上地址和端口号
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;                // 选择协议族->IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听本地所以IP地址
    server_addr.sin_port = htons(SERVER_PORT);       // 绑定端口号

    // 实现标签贴到收信的邮箱上
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 把信箱挂到传达室,这样,就可以接收信件了
    listen(sock, 128); // 最多一次性接收128封信

    // 万事具备,只等来信
    printf("Waiting Client 's connection ...\n");

    int done = 1;
    while (done)
    {
        struct sockaddr_in client;
        int client_sock;    // 用于存储新的套接字描述符
        char client_ip[64]; // 装ip

        char buf[256]; // 读取的内容
        int len;       // 读取的长度

        socklen_t client_addr_len;
        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
        // 因为第三个参数是传入传出参数,所以不能下成下面这样:
        // accept(sock, (struct sockaddr*)&client, &sizeof(client));

        // 打印客户端IP地址和端口号
        printf("client ip:%s\t port:%d\n",
               inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)),
               ntohs(client.sin_port));

        // while (1)
        //{
        //  读取客户端发送的数据
        len = read(client_sock, buf, sizeof(buf) - 1); // 留一格存放字符串结束符,read函数只执行一次

        buf[len] = '\0';
        printf("recive[%d] :%s\n", len, buf);

        // 将buf全部改成大写再写回去
        for (int i = 0; i < len; i++)
        {
            if (isalpha(buf[i])) // 如果是字符
            {
                buf[i] = toupper(buf[i]);
            }
        }
        len = write(client_sock, buf, len);
        printf("write finished.has written data:%s,has written len : %d\n", buf, len);
        close(client_sock);
        //}
    }
    return 0;
}

// gcc -std=c99 echo_server.c -o echo_server
// 在cmd或者其他linux终端中输入和服务器进行连接: telnet 192.168.8.129 6666
/*
查看端口为6666的进程:
sudo su
netstat -tlnp | grep :6666
kill 12345
*/