#include <memory>
#include <cstdlib>
#include <iostream>
#include "debug.h"
#include "heap.h"

#define HEAPLeft(i) ((i)*2)
#define HEAPRight(i) ((i)*2+1)
#define HEAPParent(i) ((i)/2)

Heap::Heap(int size)
{
    int n;

    // round up size to an even power of 2
    for( n = 1024; n < size; n *= 2 );
    
    pHeap = (HeapNode *)malloc(n*sizeof(HeapNode));
    heapSize = n;
    heapLength = 0;
}

Heap::Heap(void)
{
    pHeap = (HeapNode *)malloc(1024*sizeof(HeapNode));
    heapSize = 1024;
    heapLength = 0;
}

Heap::~Heap(void)
{
    if ( pHeap )
    {
        free(pHeap);
    }
}

int Heap::Insert(int key, void *pData )
{
    int i, p;
    
    if (heapLength >= heapSize)
        return 0;
        
    pHeap[heapLength].key = key;
    pHeap[heapLength].pData = pData;
    heapLength++;
    
    for( i = heapLength; i != 1; i = HEAPParent(i) )
    {
        int p = HEAPParent(i),
            ki = E(i),
            kp = E(p);
        
        if (ki > kp)
        {
            // child strictly bigger than parent, done
            return 1;
        }
        
        if (ki < kp)
        {
            // child less than parent, swap
            Swap(i,p);
            // and continue checking upward
        }
        else
        {
            // child equal to parent, make sure it's parent's left child
            if (i != HEAPLeft(p))
            {            
                // child is equal to parent, and is parent's right child.  swap children
                // so that equal values are on left subtree
                Swap(i,HEAPLeft(p));
                // and make sure we still have a valid heap on the right branch
                Heapify(i);
            }
            
            return 1;
        }
    }
    
    return 1;
}

int Heap::Length(void)
{
    return heapLength;
}

int Heap::Peek(void)
{
    ASSERT(heapLength>0,"Attempting to peek empty heap");
    return pHeap[0].key;
}

void * Heap::Pop(void)
{
    HeapNode t;
    ASSERT(heapLength>0,"Attempting to pop empty heap");
    t = pHeap[0];
    Swap(1,heapLength);
    heapLength--;
    Heapify(1);
    
    return t.pData;
}

int Heap::E(int i)
{
    ASSERT(i <= heapLength, "Invalid index in Heap::E");
    ASSERT(i > 0, "Invalid index 0 in Heap::E");
    return pHeap[i-1].key;
}

void Heap::Swap(int i, int j)
{
    HeapNode t;
    
    ASSERT((i>0)&&(i<=heapLength),"Invalid index i in Heap::Swap");
    ASSERT((j>0)&&(j<=heapLength),"Invalid index j in Heap::Swap");
    
    t = pHeap[i-1];
    pHeap[i-1] = pHeap[j-1];
    pHeap[j-1] = t;
}

void Heap::Heapify(int i)
{
    int m, l, r;
    
    ASSERT((i>0) && (i <= heapLength),"Invalid index in Heapify");
    
    l = HEAPLeft(i);
    r = HEAPRight(i);
    m = i;
    
    if ((l<=heapLength) && (E(l) < E(i)))
        m = l;
    if ((r<=heapLength) && (E(r) < E(m)))
        m = r;
        
    if (m != i)
    {
        Swap(i,m);
        Heapify(m);
    }
}

void Heap::Dump()
{
    for( int i = 0; i < heapLength; i++ )
    {
        std::cout << pHeap[i].key << " ";
    }
    std::cout << "\n";
}