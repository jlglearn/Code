#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

#include <vector>

typedef enum enumUnionFindError {
    UNIONFIND_ERR_INDEXOUTOFRANGE,
    UNIONFIND_ERR_INVALIDSIZE
    } UnionFindError;
    
class UFNode {
    int parent;
    int size;
    
    friend class UnionFind;
};


class UnionFind {

    public:
    UnionFind(int size);
    ~UnionFind(void);
    
    void Join(int i, int j);            // join elements i and j
    int  Find(int i);                   // retrieve root of element i
    int  Size(int i);                   // return size of element i's component
    
    private:
    std::vector<UFNode> items;
};

#endif