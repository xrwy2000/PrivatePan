#include "work_que.h"
void queInsert(pQue pq,pNode p)
{
    if(pq->head==NULL&&pq->tail==NULL)
    {
        pq->head=pq->tail=p;
        pq->size++;
    }
    else
    {
        pq->tail->next=p;
        pq->tail=p;
        pq->size++;
    }
}
int queGet(pQue pq,pNode *p)
{
    if(pq->head==NULL&&pq->tail==NULL)
        return -1;
    *p=pq->head;
    pq->head=pq->head->next;
    if(pq->head==NULL)
        pq->tail=NULL;
    pq->size--;
    return 0;
}

