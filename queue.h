#ifndef _QUEUE_H_
#define _QUEUE_H_

struct structQNode {
    void *pData;
    structQNode *pNext, *pPrev;
};

typedef structQNode QNode;

class Queue {
public:
    Queue(void);
    ~Queue(void);
    int Add(void *pData);
    void *Remove();
    int Empty();
    int Size();
private:
    int n;
    QNode *pHead, *pTail;
};

#endif