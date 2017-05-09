#include"sql_api.h"
#include<string.h>
#include<stdlib.h>


#define _SIZE_ 1024

static void deal_data(char* data)
{
    if(data==NULL)
        return ;
    char* arr[5];
    char* start=data;
    int i=0;
    while(*start)
    {
       if(*start=='=')
        {
            arr[i]=start+1;
            i++;
        }
        if(*start=='&')
        {
            *start='\0';
        }
        start++;
    }
    arr[i]=NULL;

    string str_data="";
    for(int j=0;j<4;j++)
    {
        if(j==2)
        {
            str_data+=arr[j];
            str_data+=",";
            continue;
        }
        str_data+="\'";
        str_data+=arr[j];
        str_data+="\'";
        if(j<3)
        {
            str_data+=",";
        }
    }

    cout<<"3344"<<endl;

    sql_api a;
    a.sql_connect();
    a.show_info();
    a.insert_sql(str_data);
   // a.select_sql(str_data);
}
int main()
{

    cout<<"ld"<<endl;
    char method[_SIZE_];
    int content_len=-1;
    char query_string[_SIZE_];
    char content_data[_SIZE_];
    memset(method,'\0',_SIZE_);
    memset(content_data,'\0',_SIZE_);
    memset(query_string,'\0',_SIZE_);

    if(getenv("METHOD"))
    {
        strcpy(method,getenv("METHOD"));
    }
    else{
        cout<<"lmetd2"<<endl;
        exit(2);
    }

    cout<<method<<endl;
    if(strcasecmp(method,"GET")==0)
    {
       cout<<"2233"<<endl;
       strcpy(query_string,getenv("QUERY_STRING"));
       deal_data(query_string);
    }
    else if(strcasecmp(method,"POST")==0) //POST
    {
        cout<<"la4d"<<endl;
        char len[_SIZE_];
        strcpy(len,getenv("CONTNT_LEN"));
        content_len=atoi(len);
        int i=0;
        for(;i<content_len;++i)
        {
            read(0,content_data,1);
        }
        cout<<"ld4"<<endl;
        deal_data(content_data);
    }
    else
    {
        cout<<"M4"<<endl;
        //do nothing
    }
    return 0;
}
