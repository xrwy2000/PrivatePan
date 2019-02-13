#ifndef __HEAD_H__
#define __HEAD_H__
#include<func.h>
#include<mysql/mysql.h>
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define error_check(argc) {if(argc!=0)printf("error happened\n");return -1}
typedef struct{
    int dataLen;
    char buf[1000];
}train;
typedef struct{
    int id;
    char name[128];
    char salt[128];
    char pwd[128];
}Account,*pAccount;


int init(int*,char*,char*);
void recv_fd(int,int *);
int send_fd(int,int);
int downFile(int,char*,char*,off_t);
int upFile(int,char*,char*);
int send_n(int,char*,int);
int recv_n(int,char*,int);
int insertUser(char *query);
int insertFile(char*);
int queryUser(char* name,pAccount pac);
int queryFile(char*);

#endif
