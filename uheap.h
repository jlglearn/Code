#ifndef _UHEAP_H_
#define _UHEAP_H_

typedef struct structUHeapNode {
    int id;
    int p;
    void *pData;
} UHeapNode;

class UHEAP {
    public:
    UHEAP(void);
    ~UHEAP(void);
    void insert(int id, int p, void *pData);
    void update(int id, int p);
    UHeapNode *pop(void);
    int isEmpty(void);
    
    private:
    UHeapNode *H;
    UHeapNode *T;
    int nSize;
    int nElements;
    int iT;
    int nT;
    
    void resize(int newsize);
    void swap(int i, int j);
    void heapifyUp(int i);
    void heapifyDown(int i);
};

#endif