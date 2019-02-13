#include "head.h"
int fileExist(char *fname,off_t* pos){
    fname[strlen(fname)-1]=0;
    if(access(fname,F_OK)==0){
        printf("access\n");
        struct stat st;
        stat(fname,&st);
        *pos=st.st_size;
        return 0;
    }else{
        return -1;
    }
}
int downFile(int cfd){
    int len;
    char buf[1000]={0};
    recv_n(cfd,(char*)&len,sizeof(int));
    recv_n(cfd,buf,len);
    //  printf("%d %s\n",len,buf);
    int fd = open(buf,O_CREAT|O_RDWR,0666);
    struct stat st;
    fstat(fd,&st);
    off_t fileTotalSize,fileCmpSize=st.st_size;
    printf("curSize=%ld\n",fileCmpSize);
    recv_n(cfd,(char*)&len,sizeof(int));
    recv_n(cfd,(char*)&fileTotalSize,len);
    if(-1==fd){
        perror("open");
        return -1;
    }
    int ret;
    time_t start=time(NULL),now;
    //文件大于100M时,使用mmap
    if(fileTotalSize>104857600){
        ret=ftruncate(fd,fileTotalSize);
        
        if(-1==ret){
            perror("ftruncate");
            return -1;
        }
        printf("start mmap...\n");
        char *addr;
        addr=(char*)mmap(NULL,fileTotalSize,PROT_WRITE,MAP_SHARED,fd,0);
        if(addr==MAP_FAILED){
            perror("mmap");
            return -1;
        }
        while(1){
            ret =  recv_n(cfd,(char*)&len,sizeof(int));
//            printf("len=%d\n",len);
            if(ret==-1){
        ret=ftruncate(fd,fileCmpSize);
                    munmap(addr,fileTotalSize);
                  //  close(cfd);
                    goto end;
            }
            if(len>0){
                ret = recv_n(cfd,addr+fileCmpSize,len);
//                printf("ret=%d\n",ret);
                if(-1==ret){

        ret=ftruncate(fd,fileCmpSize);
//        printf("ftruncate suss..\n");
                    munmap(addr,fileTotalSize);
                  //  close(cfd);
                    goto end;
                }
                //addr+=len;
                fileCmpSize+=len;
                now = time(NULL);
                if(now-start>0){
                    printf("%5.2f%s\r",(double)fileCmpSize/fileTotalSize*100,"%");
                    fflush(stdout);
                    start=now;
                }
            }else{
                printf("100.00%s\n","%");
                close(fd);
                break;
            }
        }
        msync(addr,fileTotalSize,MS_SYNC);
        munmap(addr,fileTotalSize);
        printf("同步成功\n");
    }else{
        while(1){
            ret =  recv_n(cfd,(char*)&len,sizeof(int));
            if(ret==-1){
                break;
            }
            if(len>0){
                ret = recv_n(cfd,buf,len);
                if(-1==ret){
                    close(cfd);
                    goto end;
                }
                write(fd,buf,len);
                fileCmpSize+=len;
                now = time(NULL);
                if(now-start>0){
                    printf("%5.2f%s\r",(double)fileCmpSize/fileTotalSize*100,"%");
                    fflush(stdout);
                    start=now;
                }
            }else{
                printf("100.00%s\n","%");
                close(fd);
                break;
            }
        }}
end:
    close(fd);
        //close(cfd);
        return 0;
    }
