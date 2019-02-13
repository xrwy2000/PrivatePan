#include "head.h"
int main(int argc, char* argv[]){
    if(argc < 3) {printf("usage:./clienttcp ip port\n");
        exit(-1);
    }
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    addr.sin_port=htons(atoi(argv[2]));

    int ret = connect(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
    if(ret==-1){

        perror("connect");
        return -1;
    }
    int flag;
load:
    printf("0注册，1登录\n");

    scanf("%d",&flag);
    send_n(sfd,(char*)&flag,4);
    if(flag==1){
        char name[128]={0};
        printf("登录用户名:\n");
        scanf("%s",name);
        char salt[128]={0};
        int len;
        len=strlen(name);
        send_n(sfd,(char*)&len,4);
        send_n(sfd,name,len);
        recv_n(sfd,(char*)&len,4);
        recv_n(sfd,salt,len);
        printf("salt:%s\n",salt);
        char pwd[128]={0};
        strcpy(pwd,getpass("登陆密码:\n"));
       // printf("登录密码:\n");
       // scanf("%s",pwd);
        strcpy(pwd,crypt(pwd,salt));
        printf("%s\n",pwd);
        len = strlen(pwd);
        send_n(sfd,(char*)&len,4);
        send_n(sfd,pwd,len);
        recv_n(sfd,(char*)&len,4);
        if(len==-1){
            printf("error account\n");
            close(sfd);
            exit(0);
        }else{
            train t;
            memset(&t,0,sizeof(t)); 
            while(1){
                memset(&t,0,sizeof(t));
                read(STDIN_FILENO,t.buf,sizeof(t.buf));
                t.dataLen=strlen(t.buf)-1;
                ret =send_n(sfd,(char*)&t,t.dataLen+4);
                if(-1==ret){
                    goto end;
                }
                system("clear");
                if(strncmp(t.buf,"ls",2)==0){
                    while(1){
                        memset(&t,0,sizeof(t));
                        ret=recv_n(sfd,(char*)&t.dataLen,sizeof(int));
                        //                printf("ret=%d,t.dataLen=%d\n",ret,t.dataLen);
                        if(t.dataLen==-1){
                            printf("路径有误,请回到根目录\n");
                            break;
                        }
                        if(ret==-1)
                            goto end;
                        if(t.dataLen==0) break;
                        ret=recv_n(sfd,t.buf,t.dataLen);
                        if(-1==ret)
                            goto end;
                        printf("%s",t.buf);
                    }
                }
                else if(strncmp(t.buf,"pwd",3)==0){
                    memset(&t,0,sizeof(t));
                    ret= recv_n(sfd,(char*)&t.dataLen,sizeof(int));
                    if(-1==ret)
                        goto end;
                    ret= recv_n(sfd,t.buf,t.dataLen);
                    if(-1==ret)
                        goto end;
                    printf("%s\n",t.buf);
                }
                else if(strncmp("cd",t.buf,2)==0){
                    memset(&t,0,sizeof(t));
                    ret= recv_n(sfd,(char*)&t.dataLen,sizeof(int));
                    if(-1==ret)
                        goto end;
                    if(t.dataLen!=-1)
                        ret= recv_n(sfd,t.buf,t.dataLen);
                    if(-1==ret)
                        goto end;
                    if(t.dataLen!=-1)
                        printf("当前路径:%s\n",t.buf);
                    else
                        printf("输入路径有误!\n");
                }
                else if(strncmp("puts",t.buf,4)==0){
                    ret=upFile(sfd,t.buf+5);
                    if(-1==ret){
                        goto end;
                    }
                    if(-2==ret){

                    }else{
                        printf("upload success...\n");
                    }}
                else if(strncmp("gets",t.buf,4)==0){
                    //判断是否文件已经存在i
                    off_t curSize=0;
                    ret=fileExist(t.buf+5,&curSize);
                    if(ret==0){
                        printf("send ok...\n");
                        t.dataLen=sizeof(off_t);
                        memcpy(t.buf,(char*)&curSize,sizeof(off_t));
                        send_n(sfd,(char*)&t,4+t.dataLen);
                    }else{
                        t.dataLen=-1;
                        send_n(sfd,(char*)&t,4);
                    }
                    //存在则先send过去文件大小
                    //再下载
                    printf("start...\n");

                    //打开文件，偏移到尾端
                    ret=downFile(sfd);

                    if(-1==ret){
                        printf("down failed...\n");
                    }else{
                        printf("\ndown success!\n");
                    }
                }else if(strcmp("bye\n",t.buf)==0){
                    close(sfd);
                    exit(0);
                }
                else{
                    
                    printf("命令有误,重新输入\n");
                }
            }
        }
    }else{
        int len;
        char usrName[128];
        printf("请输入用户名:\n");
        scanf("%s",usrName);
        len = strlen(usrName);
        send_n(sfd,(char*)&len,4);
        send_n(sfd,usrName,len);
        memset(usrName,0,sizeof(usrName));
        strcpy(usrName,getpass("请输入密码:\n")); 
        //printf("请输入密码:\n");
        //scanf("%s",usrName);
        len = strlen(usrName);
        send_n(sfd,(char*)&len,4);
        send_n(sfd,usrName,len);
        recv_n(sfd,(char*)&flag,4);
        if(-1==flag){
            goto end;
        }else{
            goto load;
        }

    }
end:
    close(sfd);
    exit(0);
}


