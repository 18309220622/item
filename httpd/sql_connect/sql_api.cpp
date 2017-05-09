/*************************************************************************
	> File Name: sql_connect.c
	> Author: 
	> Mail: 
	> Created Time: Thu 13 Apr 2017 03:43:57 AM PDT
 ************************************************************************/

#include"sql_api.h"

sql_api::sql_api()
{
    cout<<"mysql version"<< mysql_get_client_info()<<endl;
    mysql_init(&mysql);
}
bool sql_api::sql_connect()
{
    if(mysql_real_connect(&mysql,"127.0.0.1","root","123456","class",3306,NULL,0)!=NULL)
    {
        cout<<"sql connect success!"<<endl;
        return true;
    }
    else
    {
        cout<<"sql connect failed!"<<endl;
        return false;
    }
}
void sql_api::insert_sql(const string& data)
{
    cout<<"data:"<<data<<endl;
    cout<<"enter insert_sql"<<endl;
    string sql="INSERT INTO student_info(name,sex,age,id) values(";
    sql+=data;
    sql+=")";
    mysql_query(&mysql,sql.c_str());
//    if(mysql_query(&mysql,sql.c_str())==0)
//    {
//        cout<<"query data success!"<<endl;
//        return true;
//    }
//    else
//    {
//        cerr<<"query data failed!"<<endl;
//        return false;
//    }
}

void sql_api::delete_sql(const string& data)
{
    //cout<<data<<endl;
    char query[1024];
    cout<<"<br/>"<<endl;
    sprintf(query,"delete from student_info where name='%s';",data.c_str());
    cout<<query<<endl;
    cout<<"<br/>"<<endl;
   // sql+=data;
    mysql_query(&mysql,query);   
}

void sql_api::update_sql(const string& data)
{
    char query[1024];
    cout<<"<br/>"<<endl;
    sprintf(query,"update student_info SET name='zhaomin' where name='%s';",data.c_str());
   //string sql="update student_info set";
   // sql+=data;
   // sql+=";";
   // cout<<query<<endl;
    cout<<"<br/>"<<endl;
    mysql_query(&mysql,query);   
}
void sql_api::select_sql(const string& data)
{
    //cout<<"<br/>"<<endl;
    //cout<<"data:"<<data<<endl;
    //cout<<"<br/>"<<endl;
    char query[1024];
    sprintf(query,"select * from student_info where name='%s';",data.c_str());
    //int num=mysql_query(&mysql,query);
    //cout<<"<br/>"<<endl;
    //cout<<"num:"<<num<<endl;
    //cout<<"<br/>"<<endl;

    //string sql="select * from student_info";
    if(mysql_query(&mysql,query)==0)
    {
       //cout<<"select"<<endl;
      // cout<<"<br/>"<<endl;
       res = mysql_store_result(&mysql);
       if(res==NULL)
       {
          exit(1);
       }
       int row = mysql_num_rows(res);
       int field = mysql_num_fields(res);
       cout<<"row:"<<row<<endl;
        MYSQL_ROW line;
        for(int i=0;i<row;++i)
        {
            cout<<"line1"<<endl;
            line=mysql_fetch_row(res);
            for(int j=0;j<field-1;++j)
            {
                cout<<line[j]<<" ";
            }
            if(i!=row-1)
            {
                cout<<",";
            }
        }
            cout<<"line2"<<endl;
    }
}

sql_api::~sql_api()
{
    mysql_free_result(res);
    mysql_close(&mysql);
//    if(res!=NULL)
//    {
//        free(res);
//        res=NULL;
//    }
}

void sql_api::show_info()
{
    std::cout<<mysql_get_client_info()<<std::endl;
}
