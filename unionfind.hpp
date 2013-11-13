#ifndef _UNIONFIND_HPP_
#define _UNIONFIND_HPP_

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
    UnionFind(void);
    UnionFind(int size);
    ~UnionFind(void);
    void Reset(int size);
    
    void Join(int i, int j);            // join elements i and j
    int  Find(int i);                   // retrieve root of element i
    int  Size(int i);                   // return size of element i's component
    
    private:
    std::vector<UFNode> items;
};


UnionFind::UnionFind(void)
{   Reset(0);   }

UnionFind::UnionFind(int size)
{   Reset(size);    }



void UnionFind::Reset(int size)
{
    if (size < 0) throw UNIONFIND_ERR_INVALIDSIZE;
    
    if (items.size() > 0) 
        items.clear();
        
    items.resize(size);         // make room
    
    for (int i = 0; i < items.size(); i++)
    {
        items[i].parent = i;    // initialize each element's parent to point to itself
        items[i].size = 1;      // initialize each element's size to 1 (itself)
    }
}

UnionFind::~UnionFind(void)
{
}

// join i to j
void UnionFind::Join(int i, int j)
{
    if ((i < 0) || (i >= items.size()) || (j < 0) || (j >= items.size()))
        throw UNIONFIND_ERR_INDEXOUTOFRANGE;
        
    int pi = Find(i);           // retrieve root of i's component
    int pj = Find(j);           // retrieve root of j's component
    
    // do nothing if already joined
    if (pi != pj)
    {        
        // make smaller component point to root of larger component
        if (items[pi].size <= items[pj].size)
        {
            items[pi].parent = pj;
            items[pj].size += items[pi].size;
        }
        else
        {
            items[pj].parent = pi;
            items[pi].size += items[pj].size;
        }
    }
}


// retrieve root of given element
int UnionFind::Find(int i)
{
    if ((i < 0) || (i >= items.size()))
        throw UNIONFIND_ERR_INDEXOUTOFRANGE;
        
    int q = i;
    
    // iterate until we find the root of i's component, compacting path along the way
    while (items[q].parent != q)
    {
        int p = items[q].parent;            // p points to q's parent
        items[q].parent = items[p].parent;  // make q point to p's parent (eliminating intermediate hop through p)
        q = p;                              // move pointer upward
    }
    
    return q;
}

// return size of given element's component
int UnionFind::Size(int i)
{
    if ((i < 0) || (i >= items.size()))
        throw UNIONFIND_ERR_INDEXOUTOFRANGE;
        
    int pi = Find(i);                       // find root of i's component
    return items[pi].size;                  // and return component's size
}

#endif