#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"
typedef struct node{
    int cfd;
    struct node *next;
}node,*pNode;

typedef struct {
    pNode head,tail;
    int size;
    int queCapacity;
    pthread_mutex_t queMutex;
    pthread_cond_t queCond;
}que,*pQue;
typedef void* (*pthreadfunc)(void*);
void queInsert(pQue pq,pNode p);
int queGet(pQue pq,pNode *p);

#endif
