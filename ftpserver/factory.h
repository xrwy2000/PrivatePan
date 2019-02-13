#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "work_que.h"
typedef struct{
    pthread_t *pthid;
    pthreadfunc threadfunc;
    que q;
    int startFlag;
    int threadNum;
}factory,*pFactory;
void facInit(pFactory,int,pthreadfunc,int);
void facStart(pFactory);
#endif
