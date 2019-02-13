#include "head.h"
//当客户端断开时，服务器往newfd写数据，
//第一次写，send函数返回-1，第二次写会收到内核的sigpipe信号，程序就挂了
//因此，在第一次返回-1时，就要捕捉并正常退出
int upFile(int newfd,char *name){
    name[strlen(name)-1]=0;
    int fd = open(name,O_RDONLY,0);
    
    if(-1==fd){
        perror("open");
        close(fd);
        send_n(newfd,(char*)&fd,4);
        return -1;
    }
    struct stat st;
    fstat(fd,&st);
    char *addr = (char*)mmap(NULL,st.st_size,PROT_READ,MAP_SHARED,fd,0);
    char *md = (char*)MD5((unsigned char*)addr,st.st_size,NULL);
    printf("md5=%s\n",md);
    int ret;
    int len;
    len = strlen(md);
    send_n(newfd,(char*)&len,4);
    send_n(newfd,md,len);
    len = strlen(name);
    send_n(newfd,(char*)&len,4);
send_n(newfd,name,len);
    recv_n(newfd,(char*)&ret,4);
    if(-1==ret){
    train t;

    memset(&t,0,sizeof(t));
    t.dataLen = sizeof(off_t);
    memcpy(t.buf,&st.st_size,t.dataLen);
    ret=send_n(newfd,(char*)&t,4+t.dataLen);
    if(-1==ret){
        return -1;
    }
    if(st.st_size>104857600){
        char *addr = (char*)mmap(NULL,st.st_size,PROT_READ,MAP_PRIVATE,fd,0);
        off_t curSize=0,totalSize=st.st_size;
        t.dataLen=1000;
        time_t past=time(NULL),now;
        while(curSize<totalSize){
            if(totalSize-curSize<1000){
                t.dataLen=totalSize-curSize;
            }
            ret=send_n(newfd,(char*)&t.dataLen,4);
            if(-1==ret){
                munmap(addr,totalSize);

                return -1;
            }
            ret = send_n(newfd,addr+curSize,t.dataLen);
            curSize+=t.dataLen;
            now=time(NULL);
            if(now-past>0){
                printf("%5.2f%s\r",(double)curSize/totalSize*100,"%");
                fflush(stdout);
                past = now;
            }

        }
        printf("100.00%s\n","%");
        t.dataLen=0;
        ret=send_n(newfd,(char*)&t,4);
        if(-1==ret){
            close(fd);
            munmap(addr,totalSize);
            return 0;
        }
        printf("puts end...\n");
        munmap(addr,totalSize);
    }else{//没有上传进度条，因为速度太快了
        while(len=read(fd,t.buf,sizeof(t.buf))){
            //        printf("len=%d\n",len);
            t.dataLen=len;
            ret=send_n(newfd,(char*)&t,t.dataLen+4);
            if(-1==ret){
                return -1;
            }
        }
        t.dataLen=0;
        ret=send_n(newfd,(char*)&t,4);
        if(-1==ret){
            return -1;
        }}
    close(fd);
    }else{
        len = strlen(name);
        send_n(newfd,(char*)&len,4);
        send_n(newfd,name,len);
        recv_n(newfd,(char*)&ret,4);
        if(0==ret){
            printf("upfile success\n");
        }
    }
    //   printf("puts success\n");
    return 0;
}
