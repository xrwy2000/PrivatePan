#include "head.h"
int upFile(int cfd,char*cur,char *filePool){
    char md[1000]={0};
    int len;
    char dest[1000]={0};
    char src[1000]={0};
    char buf[1000]={0};//再发一次文件名
   //记得判断返回值 
    recv_n(cfd,(char*)&len,4);
    if(-1==len){
        printf("upload failed\n");
        return -1;
    }
    recv_n(cfd,md,len);
    sprintf(dest,"%s/%s",filePool,md);
    int ret;
    ret = queryFile(md);
    recv_n(cfd,(char*)&len,4);
    recv_n(cfd,buf,len);
    sprintf(src,"%s/%s",cur,buf);
    send_n(cfd,(char*)&ret,4);
    memset(buf,0,sizeof(buf));
    if(-1==ret){
     int len;

  //  recv_n(cfd,(char*)&len,sizeof(int));
  //  if(len==-1){
  //      printf("客户上传失败...\n");
  //      return -1;
  //  }
  //  recv_n(cfd,buf,len);

    //  printf("%d %s\n",len,buf);

     int fd = open(dest,O_CREAT|O_RDWR,0666);
    printf("dest=%s\n",dest);
    off_t fileTotalSize,fileCmpSize=0;
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
                    munmap(addr,fileTotalSize);
        ret=ftruncate(fd,fileCmpSize);
                  //  close(cfd);
                    close(fd);
                    return -1;
            }
            if(len>0){
                ret = recv_n(cfd,addr+fileCmpSize,len);
//                printf("ret=%d\n",ret);
                if(-1==ret){
                    munmap(addr,fileTotalSize);
        ret=ftruncate(fd,fileCmpSize);
        close(fd);
        return -1;
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
                    close(fd);
                    return -1;
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
        }
    }
    //添加硬链接
   // sprintf(src,"%s/%s",cur,buf);//cur:当前路径,buf:接受文件名
    ret = link(dest,src);//添加链接
    printf("dest=%s\nsrc=%s\n",dest,src);
    if(-1==ret){
        printf("链接添加失败\n");
        return -1;
    }
    memset(src,0,sizeof(src));
    sprintf(src,"insert into File(md) values('%s')",md);
    insertFile(src);
    return 0;
    }else{
        
        recv_n(cfd,(char*)&len,4);
        recv_n(cfd,buf,len);
        sprintf(src,"%s/%s",cur,buf);
        ret = link(dest,src);
        len=0;
        send_n(cfd,(char*)&len,4);

        return 0;
    }
   
}
