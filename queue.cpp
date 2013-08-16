#include <memory>
#include <cstdlib>
#include "debug.h"
#include "queue.h"

Queue::Queue(void)
{
    pTail = pHead = (QNode *)0;
    n = 0;
}

Queue::~Queue(void)
{
}

int Queue::Add(void *pData)
{
    QNode *p = new(QNode);
    p->pData = pData;
    p->pNext = p->pPrev = (QNode *)0;
       
    if (pHead == (QNode *)0)
    {
        pHead = pTail = p;
    }
    else
    {
        pTail->pNext = p;
        p->pPrev = pTail;
        pTail = p;
    }
    
    n++;
    return n;
}

int Queue::Size()
{
    return n;
}

int Queue::Empty()
{
    return (n == 0);
}

void * Queue::Remove()
{
    void *p;
    QNode *q;
    
    ASSERT(n>0,"Attempt to Remove() on empty queue.\n");
    ASSERT(pHead!=(QNode *)0,"NULL pHead in Queue::Remove()\n");
    
    p = pHead->pData;
    q = pHead;
    
    pHead = pHead->pNext;
    if (pHead == (QNode *)0)
    {
        pTail = (QNode *)0;
    }
    else
    {
        pHead->pPrev = (QNode *)0;
    }

    n--;
    
    delete(q);
    return p;
}