#include "head.h"
//当客户端断开时，服务器往newfd写数据，
//第一次写，send函数返回-1，第二次写会收到内核的sigpipe信号，程序就挂了
//因此，在第一次返回-1时，就要捕捉并正常退出
int downFile(int newfd,char *name,char *curDir,off_t pos){
    // char fname[512]={0};
    // sprintf(fname,"%s/%s",curDir,name);
    chdir(curDir);
    //    printf("%s\n",fname);
    int fd = open(name,O_RDONLY);
    if(-1==fd){
        perror("open");
        return -1;
    }
    int ret;
    train t;
    t.dataLen = strlen(name);
    strcpy(t.buf,name);
    ret = send_n(newfd,(char*)&t,4+t.dataLen);
    if(-1==ret){
        return -1;
    }
    struct stat st; 
    fstat(fd,&st);
    t.dataLen = sizeof(off_t);
    memcpy(t.buf,&st.st_size,t.dataLen);
    ret=send_n(newfd,(char*)&t,4+t.dataLen);
    if(-1==ret){
        return -1;
    }
    int len;
    if(st.st_size>104857600){
        char *addr = (char*)mmap(NULL,st.st_size,PROT_READ,MAP_PRIVATE,fd,0);
        off_t curSize=pos,totalSize=st.st_size;
        printf("curSize=%ld\n",curSize);
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
        printf("gets end...\n");
        munmap(addr,totalSize);
    }else{




        while(len=read(fd,t.buf,sizeof(t.buf))){
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
    return 0;
}
