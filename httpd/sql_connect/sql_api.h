#ifndef _SQL_CONNECT_H
#define _SQL_CONNECT_H_
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<cstring>
#include"mysql_lib/include/mysql.h"
#include"mysql_lib/include/my_global.h"
using namespace std;


class sql_api{
public:
    sql_api();
    bool sql_connect();
    void insert_sql(const string& data);
    void delete_sql(const string& data);
    void update_sql(const string& data);
    void select_sql(const string& data);
    void close_sql();
    void show_info();
    ~sql_api();
private:
    MYSQL mysql;
    MYSQL_RES* res;
    string host;
    string user;
    string passwd;
    string db;
};

#endif
