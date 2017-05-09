#ifndef _HTTPD_H_
#define _HTTPD_H_

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/sendfile.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>

#define _SIZE_ 4096

#define NORMAL 0
#define WARNING 1
#define FATAL 2

int startup(const char* _ip,int _port);
void print_log(const char* erro_msg,int level);
void* accept_request(void* arg);

#endif    //_HTTPD_H_
