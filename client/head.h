#ifndef __HEAD_H__
#define __HEAD_H__
#include<func.h>
#include<crypt.h>
#include<openssl/md5.h>
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define error_check(argc) {if(argc!=0)printf("error happened\n");return -1}
typedef struct{
    int dataLen;
    char buf[1000];
}train;
int init(int*,char*,char*);
int recv_fd(int,int *);
int send_fd(int,int);
int upFile(int,char*);
int downFile(int);
int send_n(int,char*,int);
int recv_n(int,char*,int);
int fileExist(char*,off_t*);
#endif
