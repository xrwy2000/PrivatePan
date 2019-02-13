#include "manage.h"
void initFilePool(char *pool,char* root){
    sprintf(pool,"%s/%s",root,"file_pool");
}
void getRealDir(char* root){

    getcwd(root,1000);
    printf("%s\n",root);
}//获取当前目录
void createDir(char *root,char *cur,char *ch){
    sprintf(cur,"%s/%s",root,ch);
    if(access(cur,F_OK)!=0)
        mkdir(cur,0775);
}//根据用户名创建用户dir
void makeDir(char *cur,char *ch){
    char buf[128];
    sprintf(buf,"%s/%s",cur,ch);
    if(access(buf,F_OK)!=0){
        mkdir(buf,0775);
    }
}
int showDir(char *cur,int cfd){
    struct dirent *pDirInfo;
    DIR *pDir;
    struct stat st;
    pDir = opendir(cur);
    train t;
    memset(&t,0,sizeof(t));
    if(NULL==pDir){
        perror("opendir");
        t.dataLen=-1;
        send_n(cfd,(char*)&t,4);
        return -1;
    }
    printf("%s\n",cur);
    memset(&t,0,sizeof(t));
    sprintf(t.buf,"文件名\t\t文件类型\t\t文件大小\n");
    t.dataLen=strlen(t.buf);
    send_n(cfd,(char*)&t,t.dataLen+4);
    char type[128]={0};
    char abPath[512]={0};
    while((pDirInfo=readdir(pDir))!=NULL){

        if(strcmp(".",pDirInfo->d_name)==0||strcmp("..",pDirInfo->d_name)==0){
            continue;
        }

        if(pDirInfo->d_type==4){
            strcpy(type,"目录");
        }else{
            strcpy(type,"文件");
        }
        memset(abPath,0,sizeof(abPath));
        sprintf(abPath,"%s/%s",cur,pDirInfo->d_name);
        memset(&t,0,sizeof(t));
        stat(abPath,&st);//根据绝对路径获取stat
        sprintf(t.buf,"%6s\t\t%10s\t\t%8ldB\n",pDirInfo->d_name,type,st.st_size);
        t.dataLen=strlen(t.buf);
        send_n(cfd,(char*)&t,t.dataLen+4);
    }
    memset(&t,0,sizeof(t));
    t.dataLen=0;
    send_n(cfd,(char*)&t,t.dataLen+4);
    //chdir("-");
    return 0;
}//获取用户当前目录
int uploadFile(char *fname);
int downloadFile(char *fname);
int removeFile(char *fname,char *cur){
    int ret= remove(fname);
    if(-1==ret){
        // perror("remove");
        rm_dir(fname,cur);
        return -1;
    }
    return 0;
}
void rm_dir(char *dname,char* cur){
    char buf[512];
    sprintf(buf,"%s/%s",cur,dname);
    remove_dir(buf);
}
int remove_dir(const char *dir)
{
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[512];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    // 参数传递进来的目录不存在，直接返回
    if ( 0 != access(dir, F_OK) ) {
        return 0;
    }

    // 获取目录属性失败，返回错误
    if ( 0 > stat(dir, &dir_stat) ) {
        perror("get directory stat error");
        return -1;
    }

    if ( S_ISREG(dir_stat.st_mode) ) {  // 普通文件直接删除
        remove(dir);
    } else if ( S_ISDIR(dir_stat.st_mode) ) {   // 目录文件，递归删除目录中内容
        dirp = opendir(dir);
        while ( (dp=readdir(dirp)) != NULL ) {
            // 忽略 . 和 ..
            if ( (0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name)) ) {
                continue;
            }

            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            remove_dir(dir_name);   // 递归调用
        }
        closedir(dirp);

        rmdir(dir);     // 删除空目录
    } else {
        perror("unknow file type!");
    }

    return 0;
}
void showPath(char *cur,int cfd){
    train t;
    strcpy(t.buf,cur);
    t.dataLen=strlen(t.buf);
    send_n(cfd,(char*)&t,t.dataLen+4);
}
void getUsrDir(char *serRootDir,char *cliRootDir,char *cliCurDir){
    strncpy(cliRootDir,serRootDir+20,5);
    strcpy(cliCurDir,cliRootDir);
}
int changeUsrDir(pUsrInfo pui,char *ch){
    int flag=0; 
    if(strcmp(ch,"..")==0){
        flag=1;
        if(strcmp(pui->serCur,pui->serRoot)==0){
            //报错

        }else{
            flag=1;
            int i;
            for(i=strlen(pui->serCur)-1;pui->serCur[i]!='/';i--){
                pui->serCur[i]=0;
            }
            pui->serCur[i]=0;
            for(i=strlen(pui->cliCur)-1;pui->cliCur[i]!='/';i--){
                pui->cliCur[i]=0;
            }        
            pui->cliCur[i]=0;
        }

    }else if(strcmp(ch,"~")==0){
        flag=1;
        memset(pui->cliCur,0,sizeof(pui->cliCur));
        strcpy(pui->cliCur,pui->cliRoot);
        memset(pui->serCur,0,sizeof(pui->serCur));
        strcpy(pui->serCur,pui->serRoot);
    }else if(strncmp(ch,"./",2)==0){
        ch+=2;
        char p[512]={0};
        char q[512]={0};
        sprintf(p,"%s/%s",pui->serCur,ch);
        sprintf(q,"%s/%s",pui->cliCur,ch);
        if(access(p,F_OK)==0){
            strcpy(pui->serCur,p);
            strcpy(pui->cliCur,q);
            flag=1;
        }
    }else{
        char p[512]={0};
        char q[512]={0};
        sprintf(p,"%s/%s",pui->serCur,ch);
        sprintf(q,"%s/%s",pui->cliCur,ch);
        if(access(p,F_OK)==0){
            strcpy(pui->serCur,p);
            strcpy(pui->cliCur,q);
            flag=1;
        }

    }

    train t;
    if(flag==1){
        memset(&t,0,sizeof(t));
        t.dataLen=strlen(pui->cliCur);
        strcpy(t.buf,pui->cliCur);
        send_n(pui->pnd->cfd,(char*)&t,t.dataLen+4);
        printf("当前服务器路径:%s\n",pui->serCur);
        printf("当前用户路径:%s\n",pui->cliCur);}
    else{
        memset(&t,0,sizeof(t));
        t.dataLen=-1;
        send_n(pui->pnd->cfd,(char*)&t,4);

    }
    return 0;
}
