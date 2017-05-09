#include"sql_api.h"
#include<strings.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 1024

int main()
{
    char method[SIZE];
    char content_data[SIZE];
    int content_len=-1;

    memset(method,0,SIZE);
    memset(content_data,0,SIZE);

    if(getenv("METHOD"))
    {
        strcpy(method,getenv("METHOD"));
    }
    else
    {
        exit(6);
    }

    if(strcasecmp(method,"GET")==0)
    {
        if(getenv("QUERY_STRING"))
        {
            strcpy(content_data,getenv("QUERY_STRING"));
        }
        else
        {
            //error
            exit(7);
        }
    }

    else   //strcasecmp(method,"POST")==0
    {
        if(getenv("CONTENT_LEN"))
        {
            char len[SIZE];
            strcpy(len,getenv("CONTENT_LEN"));
            content_len=atoi(len);
            for(int i=0;i<content_len;++i)
            {
                read(0,content_data,1);
            }
        }
        else
        {
            exit(8);
        }
    }

    char* arr[5];
    int i=0;
    int j=0;
    char* start=content_data;
    while(*start)
    {
        if(*start=='=')
        {
            arr[j]=start+1;
            j++;
        }
        if(*start=='&')
        {
           // cout<<"start=='\0'"<<endl;
            *start=0;
        }
            start++;
    }
    //for(;i<content_len;++i)
    //{
    //    if(*start=='=')
    //    {
    //        arr[j]=start+1;
    //        j++;
    //    }
    //    else if(*start=='&')
    //    {
    //        *start=='\0';
    //        break;
    //    }
    //    else
    //    {
    //        start++;
    //    }
    //}
    arr[4]=NULL;
    string data="";
    data+=arr[0];
    //cout<<data.c_str()<<endl;

    sql_api s;
    s.sql_connect();
    s.update_sql(data);
   // s.select_sql(data);

    return 0;
}
