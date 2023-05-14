# echo_server
回声服务器


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
