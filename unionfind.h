#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

typedef struct structUFNode {
    int key;
    int parent;
    int rank;
    int size;
} UFNode;

class UNIONFIND
{
    public:
    UNIONFIND(int size);
    UNIONFIND(void);
    ~UNIONFIND(void);
    
    void Init(int n);
    void Union(int x, int y);
    int Find(int x);
    
    private:
    int size;
    int nElements;
    UFNode *A;
    
    void resize(int newsize);
};

#endif