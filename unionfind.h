#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

struct structUFNode {
    int id;
    int parent;
    int rank;
    int size;
};

typedef structUFNode UFNode;

class UNIONFIND
{
    public:
    UNIONFIND(void);
    UNIONFIND(int size);
    ~UNIONFIND(void);
    
    void Init(int size);
    void Union(int x, int y);
    int Find(int x);
    int ClusterCount(void);
    
    private:
    UFNode *A;
    int nElements;
    int nClusters;
    int size;
    
    void resize(int newsize);
};

#endif