#ifndef _IHEAP_HPP_
#define _IHEAP_HPP_

#include <iostream>
#include <vector>
#include "debug.h"


typedef enum enumIndexedHeapError {
    INDEXEDHEAP_ERR_INVALIDINDEX,
    INDEXEDHEAP_ERR_INVALIDSIZE,
    INDEXEDHEAP_ERR_EMPTYHEAP
    } IndexedHeapError;

template <class NUMERICWEIGHT, class DATATYPE>
class IndexedHeapNode {
    public:
    int index;
    NUMERICWEIGHT w;
    DATATYPE data;
};

typedef void * HNDEFDATATYPE;

template <class NUMERICWEIGHT, class DATATYPE = HNDEFDATATYPE>
class IndexedHeap {

    public:
    IndexedHeap(void);
    IndexedHeap(int expectedSize);
    ~IndexedHeap(void);
    
    bool empty(void);
    
    void addItem(int clientIndex, NUMERICWEIGHT w);
    void addItem(int clientIndex, NUMERICWEIGHT w, DATATYPE d);
    
    void getMin(int &indexRef);                           
    void getMin(int &indexRef, NUMERICWEIGHT &wRef);  
    void getMin(int &indexRef, NUMERICWEIGHT &wRef, DATATYPE &dRef); 
    
    bool find(int index, NUMERICWEIGHT &wRef);
    bool find(int index, NUMERICWEIGHT &wRef, DATATYPE &dRef); 
    
    void print(void);
    
    private:
    std::vector<int> vI;
    std::vector<IndexedHeapNode<NUMERICWEIGHT, DATATYPE> > vH;

    int maxIndex;
    
    void heapifyUp(int i);
    void heapifyDown(int i);
    void swap(int i, int j);
};

static inline int heapParent(int i) { return i / 2; }
static inline int heapLeft(int i)   { return i * 2; }
static inline int heapRight(int i)  { return i * 2 + 1; }
static inline int heapIndex(int i)  { return i - 1; }

template <class NUMERICWEIGHT, class DATATYPE>
IndexedHeap<NUMERICWEIGHT, DATATYPE>::IndexedHeap(void)
{
    maxIndex = 0;
}

template <class NUMERICWEIGHT, class DATATYPE>
IndexedHeap<NUMERICWEIGHT, DATATYPE>::IndexedHeap(int expectedSize)
{
    if (expectedSize < 0) throw INDEXEDHEAP_ERR_INVALIDSIZE;
    
    maxIndex = 0;
    if (expectedSize > 0)
    {
        vI.reserve(expectedSize);
        maxIndex = expectedSize - 1;
    }
}

template <class NUMERICWEIGHT, class DATATYPE>
IndexedHeap<NUMERICWEIGHT, DATATYPE>::~IndexedHeap(void)
{
}

template <class NUMERICWEIGHT, class DATATYPE>
bool IndexedHeap<NUMERICWEIGHT, DATATYPE>::empty(void)
{   return (vH.size() == 0); }

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::addItem(int clientIndex, NUMERICWEIGHT w)
{   addItem(clientIndex, w, (HNDEFDATATYPE)0);  }

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::addItem(int clientIndex, NUMERICWEIGHT w, DATATYPE d)
{
    
    if (clientIndex < 0) throw INDEXEDHEAP_ERR_INVALIDINDEX;
    
    if (clientIndex > maxIndex)
    {
        vI.resize(clientIndex+1, 0);
        maxIndex = clientIndex;
    }
    
    int cIndex = vI[clientIndex];
    
    if (cIndex != 0)
    {
        // item already in heap, update values
        
        ASSERT(vH[heapIndex(cIndex)].index == clientIndex, "Unexpected: vH[heapIndex(cIndex)].index != clientIndex");
        
        NUMERICWEIGHT oldW = vH[heapIndex(cIndex)].w;
        
        vH[heapIndex(cIndex)].w = w;
        vH[heapIndex(cIndex)].data = d;
        
        // if w == oldW no need to do anything, heap property holds.
        
        if (w < oldW)
        {
            // new weight less than old weight, need to heapify up
            heapifyUp(cIndex);
        }
        else if (w > oldW)
        {
            // new weight greater than old weight, need to heapify down
            heapifyDown(cIndex);
        }
        
    }
    else
    {
        IndexedHeapNode<NUMERICWEIGHT, DATATYPE> hn;   
        
        // add item to the heap
        hn.index = clientIndex;
        hn.w = w;
        hn.data = d;
        
        // add to the back of the heap
        vH.push_back(hn);
        
        // record new item's location in the heap (location is the 1-based index (heap style index))
        vI[clientIndex] = vH.size();
        
        // now restore heap property upward from the insertion point
        heapifyUp(vH.size());
    }
}

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::heapifyUp(int i)
{
    
    while (i > 1)
    {
        int iParent = heapParent(i);
        
        if (vH[heapIndex(i)].w >= vH[heapIndex(iParent)].w)
        {
            // if element >= parent, then heap property holds
            break;
        }
        
        // if element < parent, swap
        swap(i, iParent);
        
        // and continue looking upward
        i = iParent;
    }
}

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::heapifyDown(int i)
{
    int size = vH.size();
    
    while (i < size)
    {
        int iLeft = heapLeft(i);
        int iRight = heapRight(i);
        int iMin = i;
        
        if ((iLeft <= size) && (vH[heapIndex(iLeft)].w < vH[heapIndex(i)].w))
            iMin = iLeft;
            
        if ((iRight <= size) && (vH[heapIndex(iRight)].w < vH[heapIndex(iMin)].w))
            iMin = iRight;
            
        if (iMin == i)
        {
            // if element <= than both its children, heap property holds
            break;
        }
        
        // make iMin the parent
        swap(i, iMin);
        
        // and continue looking downward
        i = iMin;
    }
}

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::swap(int i, int j)
{
    ASSERT((i > 0) && (i <= vH.size()) && (j > 0) && (j <= vH.size()), "IndexedHeap::swap(): invalid index");
           
    IndexedHeapNode<NUMERICWEIGHT, DATATYPE> hn;
    hn = vH[heapIndex(i)];
    vH[heapIndex(i)] = vH[heapIndex(j)];
    vH[heapIndex(j)] = hn;
    
    // record new location of swapped elements
    vI[ vH[heapIndex(i)].index ] = i;
    vI[ vH[heapIndex(j)].index ] = j;
}

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::getMin(int &indexRef)
{
    NUMERICWEIGHT w;
    DATATYPE d;
    getMin(indexRef, w, d);
}

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::getMin(int &indexRef, NUMERICWEIGHT &wRef)
{
    DATATYPE d;
    getMin(indexRef, wRef, d);
}

template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::getMin(int &indexRef, NUMERICWEIGHT &wRef, DATATYPE &dRef)
{
    if (vH.size() == 0) throw INDEXEDHEAP_ERR_EMPTYHEAP;
    
    // extract the data from the least element
    IndexedHeapNode<NUMERICWEIGHT, DATATYPE> hn = vH[heapIndex(1)];
    
    // swap it to the end and reduce size of heap by popping the last (previously first) element
    swap(1, vH.size());    
    vH.pop_back();
    
    // record that the popped item no longer in the heap
    vI[hn.index] = 0;
    
    // restore heap property downward from the top
    heapifyDown(1);
    
    // and return relevant data
    indexRef = hn.index;
    wRef = hn.w;
    dRef = hn.data;
}

template <class NUMERICWEIGHT, class DATATYPE>
bool IndexedHeap<NUMERICWEIGHT, DATATYPE>::find(int clientIndex, NUMERICWEIGHT &wRef)
{
	DATATYPE d;
	return find(clientIndex, wRef, d);
}

template <class NUMERICWEIGHT, class DATATYPE>
bool IndexedHeap<NUMERICWEIGHT, DATATYPE>::find(int clientIndex, NUMERICWEIGHT &wRef, DATATYPE &dRef)
{
	if ((clientIndex < 0) || (clientIndex > maxIndex)) throw INDEXEDHEAP_ERR_INVALIDINDEX;
	
	int index = vI[clientIndex];
	
	if (index == 0) return false;
	
	wRef = vH[heapIndex(index)].w;
	dRef = vH[heapIndex(index)].data;
	return true;
}


template <class NUMERICWEIGHT, class DATATYPE>
void IndexedHeap<NUMERICWEIGHT, DATATYPE>::print(void)
{
    std::cout << "Heap size: " << vH.size() << ", contents:\n";
    std::cout << "------------------------------\n";
    for (int i = 0; i < vH.size(); i++)
    {
        std::cout << "[" << i << "]: index=" << vH[i].index << ", w=" << vH[i].w << "\n";
    }
    std::cout << "\n";
}


#endif