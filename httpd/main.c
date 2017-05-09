#include"httpd.h"

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
       printf("Usage:%s [local_ip] [local_port]\n",argv[0]);
       exit(1);
    }
    int listen_sock=startup(argv[1],atoi(argv[2]));

    struct sockaddr_in peer;
    socklen_t len=sizeof(peer);
    while(1)
    {
    	len = sizeof(peer);
        int acc_sock=accept(listen_sock,(struct sockaddr*)&peer,&len);
        if(acc_sock<0)
        {
            print_log("accept failed!",WARNING);
            continue;
        }

        pthread_t tid;
        int ret= pthread_create(&tid,NULL,accept_request,(void*)acc_sock);
        if(ret!=0)    //pthread create failed
        {
            print_log("pthread create failed!",FATAL);
            close(acc_sock);
            continue;
        }
        //pthread_detach(tid);
    }
    close(listen_sock);
    return 0;
}
