#include "head.h"
int insertUser(char *query){
     MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW row;
      const char* server="localhost";
      const char* user="root";
      const char* password="123456";
      const char* database="test";
      int t,r;
       conn=mysql_init(NULL);
      if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
      {
          printf("Error connecting to database:%s\n",mysql_error(conn));
      }else{
          printf("Connected...\n");
      }
 t=mysql_query(conn,query);
      if(t)
      {
          printf("Error making query:%s\n",mysql_error(conn));
      }else{
          printf("insert success\n");
      }
      mysql_close(conn);
      return 0;

}
int insertFile(char *query){
     MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW row;
      const char* server="localhost";
      const char* user="root";
      const char* password="123456";
      const char* database="test";
      int t,r;
       conn=mysql_init(NULL);
      if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
      {
          printf("Error connecting to database:%s\n",mysql_error(conn));
      }else{
          printf("Connected...\n");
      }
 t=mysql_query(conn,query);
      if(t)
      {
          printf("Error making query:%s\n",mysql_error(conn));
      }else{
          printf("insert success\n");
      }
      mysql_close(conn);
      return 0;

}
