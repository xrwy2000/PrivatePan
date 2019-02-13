#include "head.h"
int queryUser(char *name,pAccount pac)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char* server="localhost";
    const char* user="root";
    const char* password="123456";
    const char* database="test";//要访问的数据库名称
    char query[300]="select * from User where name='";
    sprintf(query,"%s%s%s",query, name,"'");
    puts(query);
    unsigned int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //	printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {	
                //printf("num=%d\n",mysql_num_fields(res));//列数
                pac->id=atoi(row[0]);
                strcpy(pac->name,row[1]);
                strcpy(pac->salt,row[2]);
                strcpy(pac->pwd,row[3]);
            }
        }else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
int queryFile(char *md)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char* server="localhost";
    const char* user="root";
    const char* password="123456";
    const char* database="test";//要访问的数据库名称
    char query[300]="select * from File where md='";
    sprintf(query,"%s%s%s",query, md,"'");
    puts(query);
    unsigned int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //	printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(row==NULL){
                printf("no data\n");
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }else{
                printf("have data\n");
                mysql_free_result(res);mysql_close(conn);
                return 0;
            }}else{
                mysql_free_result(res);
                mysql_close(conn);
                printf("Don't find data\n");
                return -1;
            }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
