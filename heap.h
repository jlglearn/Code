#ifndef _HEAP_H_
#define _HEAP_H_


struct structHeapNode {
    int key;
    void *pData;
};

typedef structHeapNode HeapNode;

class Heap {
public:
    Heap(void);
    Heap(int size);
    ~Heap(void);
    
    int Insert(int key, void *pData);
    int Peek(void);
    void * Pop(void);
    int Length(void);
    void Dump(void);
    
private:
    HeapNode *pHeap;
    int heapSize;
    int heapLength;
    
    void Swap(int i, int j);
    int E(int i);
    void Heapify(int i);
    void resize(void);
};

#endif