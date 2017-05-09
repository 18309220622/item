#include"sql_api.h"
#include<strings.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 1024

int main()
{
    cout<<"delete1" <<endl;
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
        return 1;
    }

    cout<<"delete2" <<endl;
    if(strcasecmp(method,"GET")==0)
    {
        if(getenv("QUERY_STRING"))
        {
            strcpy(content_data,getenv("QUERY_STRING"));
            cout<<"<br/>"<<endl;
            cout<<"content_data"<<content_data<<endl;
            cout<<"<br/>"<<endl;
        }
        else
        {
            //error
            exit(4);
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
            exit(5);
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
    arr[4]=NULL;
    string data="";
   // printf("hello world\n");
   // cout<<"arr[] " <<endl;
   // cout<<"<br/>"<<endl;

    //cout<<arr[0]<<endl;
   // cout<<"<br/>"<<endl;
    data+=arr[0];
   // cout<<data<<endl;

    sql_api s;
    s.sql_connect();
    s.delete_sql(data);
   // s.select_sql(data);

    return 0;
}
