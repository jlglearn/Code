#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

class UNIONFIND
{
    public:
    UNIONFIND(void);
    UNIONFIND(int size);
    ~UNIONFIND(void);
    
    void Init(int size);
    void Union(int x, int y);
    int Find(int x);
};

#endif