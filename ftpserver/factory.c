#include "factory.h"
void facInit(pFactory pf,int capacity,pthreadfunc threadfunc,int threadnum)
{
    pf->q.head=NULL;
    pf->q.tail=NULL;
    pf->q.queCapacity=capacity;
    pf->q.size=0;
    pthread_mutex_init(&pf->q.queMutex,NULL);
    pthread_cond_init(&pf->q.queCond,NULL);
    pf->threadfunc = threadfunc;
    pf->pthid = (pthread_t*)malloc(threadnum*sizeof(pthread_t));
    pf->startFlag=0;
    pf->threadNum=threadnum;
    printf("init...\n");
}
void facStart(pFactory pf){
    int i;
    int num=pf->threadNum;
    for(i=0;i<num;i++){
        pthread_create(pf->pthid+i,NULL,pf->threadfunc,(void*)pf);
        printf("%d start...\n",i);
    }
    pf->startFlag=1;
}
