#include <string>
#include <cstring>
#include <cstdlib>
#include "debug.h"
#include "unionfind.h"

#define UFDEFSIZE 512

UNIONFIND::UNIONFIND(int newsize)
{
    A = (UFNode *)0;
    size = 0;
    nElements = 0;
    resize(newsize);
}

UNIONFIND::UNIONFIND(void)
{
    A = (UFNode *)0;
    size = 0;
    nElements = 0;
    resize(0);
}

void UNIONFIND::Init(int n)
{
    if (n < size) resize(n);
    for (int i = 0; i < n; i++)
    {
        A[i].key = i;
        A[i].parent = i;
        A[i].rank = 0;
        A[i].size = 1;
    }
    nElements = n;
}

int UNIONFIND::Find(int k)
{
    ASSERT((k >= 0) && (k < nElements), "UNIONFIND::Find() k out of range.");
    
    if ((k < 0) || (k >= nElements))
        return -1;
    
    int p = k;
    while (p != A[p].parent)
    {
        p = A[p].parent;
    }
    
    return p;
}

void UNIONFIND::Union(int x, int y)
{
    int px = Find(x);
    int py = Find(y);
    
    if (A[px].rank < A[py].rank)
    {
        A[px].parent = py;
        A[py].size += A[px].size;
    }
    else
    {
        A[py].parent = px;
        A[px].size += A[py].size;
        
        if (A[px].rank == A[py].rank)
        {
            A[px].rank++;
        }
    }
}

void UNIONFIND::resize(int newsize)
{
    if (newsize == 0)
    {
        newsize = ((size == 0) ? UFDEFSIZE : size * 2);
    }
    else
    {
        int k;
        for (k = UFDEFSIZE; k < newsize; k *= 2) ;
        newsize = k;
    }
    
    UFNode *p = (UFNode *) malloc(sizeof(UFNode) * newsize);
    memset((void *) p, 0, sizeof(UFNode) * newsize);
    
    if (A) 
    {
        memcpy((void *)p, (void *)A, size * sizeof(UFNode));
        free(A);
    }
    
    A = p;
    size = newsize;
}