# Proxy Lab

### 问题分析

在测试的时候，我通过telnet来获取网页的信息，我们的程序需要监听4500或者是其他的端口的请求，并转发结果。用户的输入可以是：

```shell
telnet www.example.com 4500
GET http://www.example.com HTTP/1.0
```

这个实验需要我们写一个代理，通过“占领”端口，作为server和client传递信息的工具，我首先根据树上639-645页的实例代码大致了解了一下处理请求的基本思路。

```c
int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
	    doit(connfd);
	    Close(connfd);
    }
}
```

当我们的程序依据这个参数运行的时候，我们的任何通过端口4500的操作都会被代理。在实现上使用了一个死循环，一直监听着给定的端口。

```shell
./proxy 4500
```

书上的实例代码并不能实现基础的功能，因为缺少数据的下载，于是对于`void doit(int connfd)`这个函数进行了一些调整。

### 基本工作流程

#### 1. 从客户处接收数据

通过书上所介绍的RIO函数，我们可以从监听的端口得到用户的输入：

```c
char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
char filename[MAXLINE],cgiargs[MAXLINE];
rio_t rio;
Rio_readinitb(&rio,fd);
Rio_readlineb(&rio,buf,MAXLINE);
sscanf(buf, "%s %s %s", method, uri, version);
```

#### 2. 把用户的命令解析成请求（计算header）

通过字符串的操作，我们可以分离开URL和文件存储的地址。因为我对于这一操作不是很熟悉，借鉴了https://blog.csdn.net/u012336567/article/details/52056089 这里的以下代码：

```c
void parse_uri(char *uri,char *hostname,char *path,int *port)
{	
    char uri2[MAXLINE];
    strcpy(uri2, uri);
    *port = 80;
    char* pos = strstr(uri2,"//");
    pos = (pos!=NULL)? pos+2:uri2;
    char*pos2 = strstr(pos,":");
    if(pos2!=NULL){
        *pos2 = '\0';
        sscanf(pos,"%s",hostname);
        sscanf(pos2+1,"%d%s",port,path);
    }
    else{
        pos2 = strstr(pos,"/");
        if(pos2!=NULL){
            *pos2 = '\0';
            sscanf(pos,"%s",hostname);
            *pos2 = '/';
            sscanf(pos2,"%s",path);
        }
        else{
            sscanf(pos,"%s",hostname);
        }
    }
    return;
}
```

然后我们就可以通过这些数据生成请求：

```c
char *get_header(char *hostname,char *path,int port,rio_t *client_rio)
{
    char buf[MAXLINE], request_hdr[MAXLINE], other_hdr[MAXLINE], host_hdr[MAXLINE], header[MAXLINE];
    sprintf(request_hdr, "GET %s HTTP/1.0\r\n", path);
    while(Rio_readlineb(client_rio, buf, MAXLINE)>0){
        if(strcmp(buf, "\r\n")==0)
            break;
        else if(!strncasecmp(buf, "Host", 4)){
            strcpy(host_hdr,buf);
        }
        else if(!strncasecmp(buf, "Connection", 10) && !strncasecmp(buf, "Proxy-Connection", 16) && !strncasecmp(buf, "User-Agent",10)){
            strcat(other_hdr,buf);
        }
    }
    if(strlen(host_hdr)==0){
        sprintf(host_hdr,"Host: %s\r\n",hostname);
    }
    sprintf(header,"%s%s%s%s%s%s", request_hdr, host_hdr, "Connection: close\r\nProxy-Connection: close\r\n", user_agent_hdr, other_hdr, "\r\n");
    return header;
}
```

#### 3. 访问资源处理请求

同样是使用RIO函数来发送数据：

```c
char portStr[100];
sprintf(portStr,"%d",port);
int server_connfd = Open_clientfd(filename, portStr);
rio_t server_rio;
Rio_readinitb(&server_rio, server_connfd);
Rio_writen(server_connfd, header, strlen(header));
```

#### 4. 给客户响应

使用RIO函数，逐行读取返回的内容，并返回给用户：

```c
char cachebuf[CONTENT_SIZE];
int sizebuf = 0;
size_t n;
while((n=Rio_readlineb(&server_rio, buf, MAXLINE))!=0){
    sizebuf+=n;
    strcat(cachebuf, buf);
    Rio_writen(fd,buf,n);
}
```

### 并发编程

根据书上12章的实例代码，我们可以这样写我们的proxy server，从而可以同时处理多个请求：

```c
Pthread_create(&pid, NULL, thread, (void *)connfd);
void *thread(void *vargp){
    int connfd = (int) vargp;
    Pthread_detach(pthread_self());
    doit(connfd);
    Close(connfd);
}
```

### 缓存

我保存了最近的20个请求的响应，来增加程序运行的效率。通过维护`global_clock`这一个全局变量，来给每个缓存内容打上时间戳，在没有空闲缓存的时候，剔除时间戳最小的一个数据。

```c
void cache_add(char *uri, char *buf){
    int oldest = 0;
    int i;
    for(i=0; i<CACHE_SIZE; i++){
        if(!cache.vaild[i]){
            strcpy(cache.content[i], buf);
            strcpy(cache.url[i], uri);
            cache.vaild[i]=1;
            cache.timestamp[i] = global_time++;
            return;
        }
        if(cache.timestamp[i]<cache.timestamp[oldest])
            oldest = i;
    }
    strcpy(cache.content[oldest], buf);
    strcpy(cache.url[oldest], uri);
    cache.vaild[oldest]=1;
    cache.timestamp[oldest] = global_time++;
}
```

### 总结和感受

- 我因为学习原因，经常用VPN，做这个项目的时候感觉很有趣，如果法律允许的话，我想尝试写更大型的proxy server。
- 字符串操作很容易产生漏洞，比如我们就曾经在bufflab利用缓冲区溢出来破坏程序。proxy server中有很多的字符操作，需要格外小心。
- 最后一个实验终于结束了QAQ 感谢老师和助教的指导，我有很多收获，这门课实验很多但是确实很好玩。理解了底层硬件后，可以帮助我们在顶层有更好的设计思维。![Screen Shot 2018-06-24 at 23.41.00](/Users/flyingturtle/Desktop/Screen Shot 2018-06-24 at 23.41.00.png)