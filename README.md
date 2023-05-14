# echo_server
回声服务器

# 将字符串转换成大写的
```c
//将字符串转换成大写的
for(int i = 0 ; i < len ; i ++)
{
  if(buf[i]>'a'&&buf[i]<'z')//是小写字符
  {
    buf[i] = buf[i] - 32;
    //buf[i] = toupper(buf[i]);
  }
}
```
# 杀死霸占当前项目端口号的进程
![image](https://github.com/1AoB/echo_server/assets/78208268/a4f636bd-0918-4b73-b070-987ec1d0f207)
# 大端与小端
>注:网络字节数统一使用**大端字节序**
![image](https://github.com/1AoB/echo_server/assets/78208268/386b1945-7e43-429f-882a-19c4719575a8)
```cpp
// 通过代码检测当前主机的字节序
#include <stdio.h>

int main() {

    union {
        short value;    // 2字节
        char bytes[sizeof(short)];  // char[2]
    } test;

    test.value = 0x0102;
    if((test.bytes[0] == 1) && (test.bytes[1] == 2)) {
        printf("大端字节序\n");
    } else if((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("小端字节序\n");
    } else {
        printf("未知\n");
    }

    return 0;
}
```
