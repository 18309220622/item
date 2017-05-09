#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>

#define _SIZE_ 1024

static void math_data(char* data)
{
    assert(data);
    char* arr[3];   
    char *start=data;
    int i=0;
    while(*start)
    {
        if(*start=='=')
        {
           arr[i]=start+1;
           i++;
        }
        else if(*start=='&')
        {
            *start='\0';
        }
        start++;
    }
    arr[i]=NULL;


    int data1=atoi(arr[0]);
    int data2=atoi(arr[1]);
    printf("<html>");
    printf("<h2>");
    printf("<tr>%d+%d=%d</tr><br/>",data1,data2,data1+data2);
    printf("<tr>%d-%d=%d</tr><br/>",data1,data2,data1-data2);
    printf("<tr>%d*%d=%d</tr><br/>",data1,data2,data1*data2);
    printf("<tr>%d/%d=%d</tr><br/>",data1,data2,data1/data2);
    printf("<tr>%d%%d=%d</tr><br/>",data1,data2,data1%data2);
    printf("</h2>");
    printf("</html>");
    //printf("data1+data2=%d\n",data1+data2);
    //printf("data1-data2=%d\n",data1-data2);
    //printf("data1*data2=%d\n",data1*data2);
    //printf("data1/data2=%d\n",data1/data2);
    //printf("data1%%data2=%d\n",data1%data2);
}

int main()
{
    printf("cgi_math\n''");
    int content_len=-1;
    char method[_SIZE_];
    char query_string[_SIZE_];
    char content_data[_SIZE_]; 
   
    memset(method,'\0',sizeof(method));
    memset(query_string,'\0',sizeof(query_string));
    memset(content_data,'\0',sizeof(content_data));

    strcpy(method,getenv("METHOD"));
    if(strcasecmp(method,"GET")==0)
    {
        strcpy(query_string,getenv("QUERY_STRING"));
        math_data(query_string);
    }
    else if(strcasecmp(method,"POST")==0)
    {
        content_len=atoi(getenv("CONTENT_LEN"));
        int i=0;
        for(;i<content_len;i++)
        {  
            read(0,&content_data[i],1); 
        }
        content_data[i]='\0';
        math_data(content_data);
    }
    else
    {
        //do nothing
    }
   return 0;   
}
