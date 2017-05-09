#include "httpd.h"

int startup(const char* _ip,int _port)
{
    assert(_ip);
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        print_log("socket",FATAL);
        exit(2);
    }

    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(_port);
    local.sin_addr.s_addr=inet_addr(_ip);
    socklen_t len=sizeof(local);

    if(bind(sock,(struct sockaddr*)&local,len)<0)
    {
        print_log("bind",FATAL);
        exit(3);
    }

    if(listen(sock,5)<0)
    {
        print_log("listen",FATAL);
        exit(4);
    }
    print_log("listen_sock access success!",NORMAL);
    return sock;
}


void print_log(const char* erro_msg,int level)
{
 #ifdef DEBUG
    const char* erro_list[10]={
     "NORMAL",
     "WARNING",
     "FATAL",
    };
    printf("[%s] [%s]\n",erro_msg,erro_list[level]);
#endif
}


static void show_404(int sock)
{
    clear_header(sock);
    char* msg="HTTP/1.0 404 Not Found\r\n";
    send(sock,msg,strlen(msg),0);
    send(sock,"\r\n",strlen("\r\n"),0);

    struct stat st;
    stat("wwwroot/404.html",&st);
    int fd=open("wwwroot/404.html",O_RDONLY);
    sendfile(sock,fd,NULL,st.st_size);
    close(fd);

}
void echo_error(int sock,int err_code)
{
    switch(err_code)
    {
        case 403:
             break;
        case 404:
             show_404(sock);
             break;
        case 405:
             break;
        case 500:
             break;
        default:
             break;
    }  
}

void clear_header(int sockfd)
{
    int ret=0;
    char buf[_SIZE_];
    do{
        ret= get_line(sockfd,buf,_SIZE_);
    }while(ret!=1 && strcmp(buf,"\n")!=0);
}

static void  excu_cgi(int sockfd,const char* path,const char* method,const char* query_string)
{
    printf("query_string :%s\n",query_string);
    char buf[_SIZE_];
    int content_len=-1;
     
    char method_env[_SIZE_/8];
    char query_string_env[_SIZE_/4];
    char content_len_env[_SIZE_/8];
    
    printf("11111111111\n");
    if(strcasecmp(method,"GET")==0)
    {
        printf ("GET\n");
        clear_header(sockfd);
    }
    else       //POST
    {
        int ret=0;
        do{
            ret=get_line(sockfd,buf,sizeof(buf));
            if(ret>0 && strncmp(buf,"Content_len: ",16)==0)
            {
                content_len=atoi(buf+16);   
            }
        }while(ret!=1 && strcmp(buf,"\n")!=0);
        
        if(content_len<0)
        {
            echo_error(sockfd,404);
            return ;   
        }
    }
    
    const char* status_line="HTTP/1.0 200 OK\r\n";
    send(sockfd,status_line,strlen(status_line),0);
    const char* content_type="Content-Type:text/html;charset=ISO-8859-1\r\n";
    send(sockfd,content_type,strlen(content_type),0);
    send(sockfd,"\r\n",strlen("\r\n"),0);

    //create pipe,and dup2
    int input_pipe[2];
    int output_pipe[2];
    pipe(input_pipe);
    pipe(output_pipe);
    pid_t id=fork();
    if(id<0)
    {
        print_log("create child proc failed!",FATAL);
        echo_error(sockfd,404);
        return ;
    }
    else if(id==0)
    {
        //child proc
        close(input_pipe[1]);
        close(output_pipe[0]);

        dup2(input_pipe[0],0);
        dup2(output_pipe[1],1);
        printf("path %s\n",path); 
        sprintf(method_env,"METHOD=%s",method);
        putenv(method_env);
        printf("putenv: %s\n",getenv("METHOD"));   //added to the environment
        if(strcasecmp(method,"GET")==0)
        {
            sprintf(query_string_env,"QUERY_STRING=%s",query_string);
            putenv(query_string_env);   //added to the environment
        }
        else{
            //POST
            sprintf(content_len_env,"CONTENT_LEN=%d",content_len);
            putenv(content_len_env);   //added to the environment
        }

        execl(path,path,0);   //ls and  /bin/ls ,put two path same
        printf("execl");
        exit(1);
    }
    else{
        //parent proc
        close(input_pipe[0]);
        close(output_pipe[1]);
        char ch='\0';
        //is read or write ,decided by the method
        //if method==POST,web http have content_len character that isn't read from client
        if(strcasecmp(method,"POST")==0)
        {
            int i=0;
            for(;i<content_len;i++)
            {
                ssize_t s=recv(sockfd,&ch,1,0);
                if(s>0)
                {
                    write(input_pipe[1],&ch,1);
                }
                else
                {
                    print_log("recv failed",WARNING);
                    echo_error(sockfd,404);
                    return ;
                }
            }
        }
        ch='\0';
        while(read(output_pipe[0],&ch,1)>0)
        {
            send(sockfd,&ch,1,0);
        }
        waitpid(id,NULL,0);  
    }
}
static int www_echo(int sockfd,const char*path,int _size)
{
    int fd=open(path,O_RDONLY);
    if(fd<0)
    {
        print_log("open fd failed",FATAL);
        echo_error(sockfd,404);
        return 7;
    }
    
    printf("aaaaaaaaaaaaaaa\n");
    char* msg= "HTTP/1.0 200 OK\r\n";
    send(sockfd,"HTTP/1.0 200 OK\r\n",strlen(msg),0);
    send(sockfd,"\r\n",strlen("\r\n"),0);  //send blank line
    int ret=sendfile(sockfd,fd,NULL,_size);
    if(ret<0)
    {
        print_log("write to sockfd failed!",WARNING);
        echo_error(sockfd,404);
        return 8;
    }
    
    close(sockfd);
    return 0;
}

int get_line(int sockfd,char buf[],int len)
{
    assert(buf);
    char ch='\0';
    int i=0;
    while(i<len-1 && ch!='\n')
    {
        ssize_t s=recv(sockfd,&ch,1,0);
        if(s>0 && ch=='\r')
        {
            recv(sockfd,&ch,1,MSG_PEEK);    //check next character,don't get it
            if(ch=='\n')
            {
                recv(sockfd,&ch,1,0);
            }
            else   //next character is effective
            {
                ch='\n';
            }
        }
        buf[i++]=ch;
    }
    buf[i]='\0';
    return i;
}
static int request_handler(int acc_fd)
{
    char method[10];
    char uri[_SIZE_];
    char path[_SIZE_];
    char buf[_SIZE_];
    int i=0;
    int j=0;
    int cgi=0;
    char* query_string=NULL;
    int ret=0;
    //get first line
    ret=get_line(acc_fd,buf,_SIZE_); 
    for(;i<sizeof(method)-1 && j<sizeof(buf) && !isspace(buf[j]);i++,j++)
    {
        method[i]=buf[j];
    }
    method[i]='\0';
    //judge method
    if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
    {
        ret=5;
        echo_error(acc_fd,404);
        goto end;
    }
    //must be GET/POST
    if(strcasecmp(method,"POST")==0)
    {
        cgi=1;
    }
    while(j<sizeof(buf) && buf[j]==' ') j++;
    i=0;
    for(;i<sizeof(uri)-1 && j<sizeof(buf) && buf[j]!=' ' ;i++,j++)
    {
        uri[i]=buf[j];   
    }
    uri[i]='\0';

    if(strcasecmp(method,"GET")==0)
    {
        query_string=uri;
        while(*query_string != '\0' && *query_string != '?')
        {
            query_string++;
        }
        if(*query_string=='?')
        {
            cgi=1;
            *query_string='\0';
            query_string++;
        }
    }
    printf("query_string :%s\n",query_string);

    sprintf(path,"wwwroot%s",uri);
    if(path[strlen(path)-1]=='/')
    {
        strcat(path,"index.html");
    }
    struct stat st;
    printf("path:%s\n",path);
    if(stat(path,&st)<0)
    {
        printf("path:%s\n",path);
        ret=6;
        echo_error(acc_fd,404);
        goto end;
    }
    else   //resource exist
    {
        if(S_ISDIR(st.st_mode))
        {
            strcat(path,"/index.html");   
        }
        else if(st.st_mode & S_IXUSR ||  st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
        {
            cgi=1;
        }
    }
    
    if(cgi)
    {
        printf("bbbbbbbb\n");
        excu_cgi(acc_fd,path,method,query_string);
    }
    else
    {
        // isn't cgi
        clear_header(acc_fd);
        ret=www_echo(acc_fd,path,st.st_size);
    }
       
    end:
        close(acc_fd);
        return ret;
}

void* accept_request(void* arg)
{
    int acc_sock=(int)arg;
    pthread_detach(pthread_self());
    printf("%d\n", request_handler(acc_sock));
    return NULL;
}

