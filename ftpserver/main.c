#include "factory.h"
#include "manage.h"
#include "reg.h"
void cleanfunc(void* p){
    pthread_mutex_unlock(&((pQue)p)->queMutex);
}
void* pthfunc(void* p){
    usrInfo ui;
    train t;
    memset(&t,0,sizeof(t));
    memset(&ui,0,sizeof(usrInfo));
    pFactory pf = (pFactory)p;
    pQue pq = &pf->q;
    int ret;
    int flag;
    pthread_cleanup_push(cleanfunc,pq);
    while(1){
        printf("欢迎光临...\n");
        memset(&ui,0,sizeof(ui));
        pthread_mutex_lock(&pq->queMutex);
        if(pq->size==0){
            pthread_cond_wait(&pq->queCond,&pq->queMutex);
        }
        ret = queGet(pq,&ui.pnd);

        pthread_mutex_unlock(&pq->queMutex);
load:       
        printf("0注册，1登录\n");  
        recv_n(ui.pnd->cfd,(char*)&flag,4);

        if(flag==1){
            int len;
            ret = login(ui.pnd->cfd,ui.usrName);
            if(ret!=0){
                len = -1;
                send_n(ui.pnd->cfd,(char*)&len,4);


            }else{
                len = 0;
                send_n(ui.pnd->cfd,(char*)&len,4);
                getRealDir(ui.root);//~/ftpserver
                createDir(ui.root,ui.serCur,ui.usrName);
                strcpy(ui.serRoot,ui.serCur);//服务器的用户根目录
                printf("用户目录创建成功\n");
                getUsrDir(ui.serRoot,ui.cliCur,ui.cliRoot);
                printf("服务器当前目录:%s\n",ui.serCur);
                printf("用户当前目录:%s\n",ui.cliCur);
                initFilePool(ui.filePool,ui.root);
                while(1){
                    memset(&t,0,sizeof(t));
                    //           printf("循环回来...\n");
                    ret=recv_n(ui.pnd->cfd,(char*)&t.dataLen,sizeof(int));
                    //          printf("%d\n",ret);
                    if(ret==-1){
                        printf("client byebye...\n");
                        break;
                    }
                    if(t.dataLen==0) break;
                    recv_n(ui.pnd->cfd,t.buf,t.dataLen);
                    //         printf("%d   %s\n",t.dataLen,t.buf);
                    if(strcmp(t.buf,"ls")==0){
                        showDir(ui.serCur,ui.pnd->cfd);
                    }else if(strcmp(t.buf,"pwd")==0){
                        showPath(ui.cliCur,ui.pnd->cfd);
                    }else if(strncmp("cd",t.buf,2)==0){
                        changeUsrDir(&ui,t.buf+3);
                    }else if(strncmp("gets",t.buf,4)==0){
                        recv_n(ui.pnd->cfd,(char*)&t.dataLen,4);
                        off_t s;
                        if(t.dataLen==-1){
                            s=0;
                        }else{
                            recv_n(ui.pnd->cfd,(char*)&s,t.dataLen);
                            printf("%ld\n",s);
                        }
                        printf("start...gets\n");
                        downFile(ui.pnd->cfd,t.buf+5,ui.serCur,s);
                    }else if(strncmp("puts",t.buf,4)==0){
                        printf("start...puts\n");
                        upFile(ui.pnd->cfd,ui.serCur,ui.filePool);
                        printf("end...puts\n");
                    }else if(strncmp("rm",t.buf,2)==0){
                        printf("start...remove\n");
                        removeFile(t.buf+3,ui.serCur);
                    }else if(strncmp("mkdir",t.buf,5)==0){
                        makeDir(ui.serCur,t.buf+6);
                    }else{

                    }
                }
            }
        }else{
            ret=reg(ui.pnd->cfd);
            if(-1==ret){
                flag = -1;
                send_n(ui.pnd->cfd,(char*)&flag,4);
                printf("client bye...\n");
                close(ui.pnd->cfd);

            }else{
                flag=0;
                send_n(ui.pnd->cfd,(char*)&flag,4);
                goto load;
            }
        }
    }
    pthread_cleanup_pop(1);
    return NULL;
}
int exitFd[2];
void sigHandler(int no){
    write(exitFd[1],"1",1);
}
int main(int argc,char **argv)
{
    args_check(argc,5);
    pipe(exitFd);
    if(fork()){
        close(exitFd[0]);
        signal(SIGUSR1,sigHandler);
        wait(NULL);
        exit(0);
    }
    close(exitFd[1]);

    factory f;
    facInit(&f,atoi(argv[3]),pthfunc,atoi(argv[4]));
    if(f.startFlag==0){
        facStart(&f);
    }
    int sfd;
    int ret = init(&sfd,argv[1],argv[2]);
    if(-1==ret){
        exit(0);
    }
    int epfd = epoll_create(1);
    struct epoll_event event,evs[2];
    event.events = EPOLLIN;
    event.data.fd=sfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
    event.data.fd=exitFd[0];
    epoll_ctl(epfd,EPOLL_CTL_ADD,exitFd[0],&event);
    int fdReadyNum;
    int cfd;
    pNode pnd;
    int i;
    while(1){
        memset(evs,0,2*sizeof(struct epoll_event));
        fdReadyNum=epoll_wait(epfd,evs,2,-1);
        if(fdReadyNum>0){
            for(i=0;i<fdReadyNum;i++){
                if(evs[i].data.fd==sfd){

                    cfd = accept(sfd,NULL,NULL);
                    pnd = (pNode)calloc(1,sizeof(node));
                    pnd->cfd=cfd;
                    pnd->next=NULL;
                    pthread_mutex_lock(&f.q.queMutex);
                    queInsert(&f.q,pnd);
                    pthread_mutex_unlock(&f.q.queMutex);
                    pthread_cond_signal(&f.q.queCond);
                }

                if(evs[i].data.fd==exitFd[0]){
                    close(sfd);
                    char buf;
                    read(exitFd[0],&buf,1);
                    for(i=0;i<f.threadNum;i++){
                        pthread_cancel(f.pthid[i]);
                    }

                    for(i=0;i<f.threadNum;i++){
                        pthread_join(f.pthid[i],NULL);
                    }
                    exit(0);
                }
            }

        }
    }}

