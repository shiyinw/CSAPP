#include <stdio.h>
#include "csapp.h"

#define MAX_CACHE_SIZE 1048576
#define CONTENT_SIZE 102400
#define CACHE_SIZE 20

static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void *thread(void *vargp);
void doit(int connfd);
void parse_uri(char *uri,char *hostname,char *path,int *port);
char *get_header(char *hostname,char *path,int port,rio_t *client_rio);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

void cache_init();
int cache_find(char *url);
void cache_add(char *uri, char *buf);

int global_time;
struct Cache{
    char content[CACHE_SIZE][CONTENT_SIZE];
    char url[CACHE_SIZE][MAXLINE];
    int timestamp[CACHE_SIZE];
    short vaild[CACHE_SIZE];
}cache;

int main(int argc,char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE],port[MAXLINE];
    socklen_t  clientlen;
    struct sockaddr_storage clientaddr;

    pthread_t pid;
    cache_init();

    if(argc != 2){
        fprintf(stderr,"usage :%s <port> \n",argv[0]);
        exit(1);
    }
    Signal(SIGPIPE,SIG_IGN);
    listenfd = Open_listenfd(argv[1]);
    while(1){
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s %s).\n",hostname,port);
        Pthread_create(&pid, NULL, thread, (void *)connfd);
    }
    return 0;
}

void *thread(void *vargp){
    int connfd = (int) vargp;
    Pthread_detach(pthread_self());
    doit(connfd);
    Close(connfd);
}

void doit(int fd)
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE],cgiargs[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio,fd);
    Rio_readlineb(&rio,buf,MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET")){
        clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
        return;
    } 
    
    // check if the uri is in the cache
    int x = cache_find(uri);
    if(x != -1){
         Rio_writen(fd, cache.content[x], strlen(cache.content[x]));
         return;
    }
    
    
    //send request to the server
    int port;
    parse_uri(uri, filename, cgiargs, &port);
    char header[MAXLINE];
    strcpy(header, get_header(filename, cgiargs, port, &rio));

    //connect server
    char portStr[100];
    sprintf(portStr,"%d",port);
    int server_connfd = Open_clientfd(filename, portStr);
    
    //receive content from the server
    rio_t server_rio;
    Rio_readinitb(&server_rio, server_connfd);
    Rio_writen(server_connfd, header, strlen(header));
    char cachebuf[CONTENT_SIZE];
    int sizebuf = 0;
    size_t n;
    while((n=Rio_readlineb(&server_rio, buf, MAXLINE))!=0){
        sizebuf+=n;
        strcat(cachebuf, buf);
        Rio_writen(fd,buf,n);
    }
    Close(server_connfd);
    cache_add(uri,cachebuf);
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

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


void cache_init(){
    int i;
    for(i=0;i<CACHE_SIZE;i++){
        cache.timestamp[i]=1;
        cache.vaild[i]=0;
    }
}

int cache_find(char *url){
    int i;
    for(i=0;i<CACHE_SIZE;i++){
        if((cache.vaild[i]==1) && (strcmp(url,cache.url[i])==0)){
            cache.timestamp[i] = global_time++;
            return i;
        }
    }
    return -1;
}


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














