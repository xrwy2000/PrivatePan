#ifndef __MANAGE_H__
#define __MANAGE_H__
#include "work_que.h"
typedef struct{
    char root[128];//~/ftpserver
    char usrName[512];//wang
    char serRoot[128];//~/ftpserver/wang
    char serCur[128];
    char cliRoot[128];//~/wang
    char cliCur[128];
    char buf[128];
    char filePool[128];
    pNode pnd;
}usrInfo,*pUsrInfo;
void getRealDir(char* root);
void createDir(char *root,char *cur,char *ch);//初始化目录
void makeDir(char*,char*);//创建目录
int showDir(char *cur,int cfd);
int uploadFile(char *fname);
int downloadFile(char *fname);
int removeFile(char *fnamei,char* cur);
void showPath(char *cur,int cfd);
void getUsrDir(char*,char*,char*);
int changeUsrDir(pUsrInfo pui,char *);
void rm_dir(char*,char*);
int remove_dir(const char*);
void initFilePool(char*,char*);
#endif
