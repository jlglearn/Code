#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "debug.h"
#include "uheap.h"

#define HEAPPARENT(i) ((i)/2)
#define HEAPLEFT(i)   ((i)*2)
#define HEAPRIGHT(i)  (((i)*2)+1)
#define HEAPPRIORITY(i) H[(i)-1].p
#define UHMINSIZE 256
#define UHTBUFSIZE 8

UHEAP::UHEAP(void)
{
    nElements = 0;
    nSize = 0;
    H = (UHeapNode *)0;
    T = (UHeapNode *) malloc(sizeof(UHeapNode)*UHTBUFSIZE);
    if (!T) throw new std::bad_alloc();
    memset((void *)T, 0, sizeof(UHeapNode)*UHTBUFSIZE);
    iT = 0;
    nT = UHTBUFSIZE;
}

UHEAP::~UHEAP(void)
{
    if (H) free(H);
    if (T) free(T);
}

void UHEAP::insert(int id, int p, void *pData)
{
    if (nElements >= nSize)
    {
        int newsize = ((nSize == 0) ? UHMINSIZE : nSize*2);
        resize(newsize);
    }
    
    H[nElements].id = id;
    H[nElements].p = p;
    H[nElements].pData = pData;
    nElements++;
    
    heapifyUp(nElements);
        
}

UHeapNode *UHEAP::pop(void)
{
    if (nElements == 0)
        throw new std::underflow_error("Attempted pop() on empty heap.");
        
    UHeapNode *p = &T[iT];
    iT = (iT + 1) % nT;    
    memcpy((void *) p, (void *) &H[0], sizeof(UHeapNode));
    swap(1, nElements);
    nElements--;    
    heapifyDown(1);
    return p;
}

int UHEAP::isEmpty(void)
{   return nElements == 0;  }

void UHEAP::update(int id, int p)
{
}

void UHEAP::resize(int newsize)
{
    int byteSize = sizeof(UHeapNode) * newsize;
    UHeapNode *pnew = (UHeapNode *) malloc(byteSize);
    if (!pnew) throw new std::bad_alloc();
    memset((void *) pnew, 0, byteSize);
    
    if (nElements)
    {
        memcpy((void *)pnew, (void *)H, sizeof(UHeapNode) * nElements);
        free(H);
    }
    
    H = pnew;
    nSize = newsize;
}

// NOTE: swap(i, j): indices are 1-based
void UHEAP::swap(int i, int j)
{
    UHeapNode t;
    memcpy((void *) &t, (void *) &H[i-1], sizeof(UHeapNode));
    memcpy((void *) &H[i-1], (void *) &H[j-1], sizeof(UHeapNode));
    memcpy((void *) &H[j-1], (void *) &t, sizeof(UHeapNode));
}

void UHEAP::heapifyUp(int i)
{
    while (i > 1)
    {
        int parent = HEAPPARENT(i);
        if (HEAPPRIORITY(i) < HEAPPRIORITY(parent))
            swap(i, parent);
        i = parent;
    }
}

void UHEAP::heapifyDown(int i)
{
    while (i < nElements)
    {
        int left = HEAPLEFT(i);
        int right = HEAPRIGHT(i);
        int m = i;
        
        if ((left <= nElements) && (HEAPPRIORITY(left) < HEAPPRIORITY(i)))
            m = left;
            
        if ((right <= nElements) && (HEAPPRIORITY(right) < HEAPPRIORITY(m)))
            m = right;
            
        if (i == m) break;
        
        swap(i, m);
        i = m;
    }
}

