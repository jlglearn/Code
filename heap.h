#ifndef _HEAP_H_
#define _HEAP_H_

struct structHeapElement {
    int key;
    void *pData;
};

typedef struct structHeapElement HeapElement;

class HEAP {
    public:
    HEAP(void);
    ~HEAP(void);
    
    void insertElement(int key, void *pData);
    void *getData(void);
    int Pop(void);
    
    private:
    HeapElement *pE;
    int size;
    int nElements;
    
    void resize(int newsize);
    void HeapifyUp(int i);
    void HeapifyDown(int i);
    
}

#endif