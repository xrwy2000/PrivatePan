#include "reg.h"
#include <crypt.h>
void generateStr(char *str){
    int i,flag;
    srand(time(NULL));
    for(i=0;i<10;i++){
        flag=rand()%3;
        switch(flag){
        case 0:
            str[i]=rand()%26+'a';
            break;
        case 1:
            str[i]=rand()%26+'A';
            break;
        case 2:
            str[i]=rand()%26+'0';
            break;
        }
    }
}
int reg(int cfd){
    printf("this is reg\n");
    char name[128]={0};
    int len;
    char pwd[1000]={0};
    int ret;
    ret=recv_n(cfd,(char*)&len,4);
    if(ret==-1){
        return -1;
    }
    ret=recv_n(cfd,name,len);
    if(ret==-1){
        return -1;
    }

    printf("用户名:%s\n",name);
    char salt[11]={0};
    generateStr(salt);
    ret=recv_n(cfd,(char*)&len,4);
    if(ret==-1){
        return -1;
    }

    ret=recv_n(cfd,pwd,len);
    if(ret==-1){
        return -1;
    }

    printf("密码:%s\n",name);
    strcpy(pwd,crypt(pwd,salt));
    char qry[256]={0};
    sprintf(qry,"insert into User(name,salt,pwd) values('%s','%s','%s')",name,salt,pwd);
    ret =insertUser(qry);
    if(ret==0){
        // printf("insert success\n");
    }
    return 0;

}
int login(int cfd,char* usrName){
    int len;
    Account ac;
    memset(&ac,0,sizeof(ac));
    char pwd[128]={0};
    int ret;
    ret=recv_n(cfd,(char*)&len,4);
    if(ret==-1){
        return -1;
    }

    ret=recv_n(cfd,usrName,len);
    if(ret==-1){
        return -1;
    }

    printf("用户名:%s\n",usrName);
    queryUser(usrName,&ac);
    printf("%s\n%s\n%s\n",ac.name,ac.salt,ac.pwd);
    len = strlen(ac.salt);
    ret=    send_n(cfd,(char*)&len,4);
    if(ret==-1){
        return -1;
    }

    ret= send_n(cfd,ac.salt,len);
    if(ret==-1){
        return -1;
    }

    ret=recv_n(cfd,(char*)&len,4);
    if(ret==-1){
        return -1;
    }

    ret=recv_n(cfd,pwd,len);
    if(ret==-1){
        return -1;
    }

    if(strcmp(ac.pwd,pwd)==0){
        return 0;
    }else{
        return -1;
    }


}
