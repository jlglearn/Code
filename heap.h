#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>

// ----------------------------------------------------------------------------
// ADT HEAP
// 
// Implements a MIN HEAP on arbitrary types or classes COMPARABLE.
//
// HEAP requires that the inequality operators (<, <=, >, >=) be properly defined
// for the COMPARABLE types.  All native numeric types (char, int, double, etc)
// naturally support this requirement.  For user defined classes, the operators must
// be properly defined.
// ----------------------------------------------------------------------------
template <class COMPARABLE> class HEAP {

	public:
	HEAP(void);
	HEAP(int (*fn)(COMPARABLE c1, COMPARABLE c2));
	void AddItem(COMPARABLE c);
	COMPARABLE ExtractMin(void);
	int size(void);
	bool empty(void);
	
	private:
	std::vector<COMPARABLE> items;
	int (*fnCompare)(COMPARABLE c1, COMPARABLE c2);
	
	int Compare(int i, int j);
	void HeapifyUp(int i);
	void HeapifyDown(int i);
	void Swap(int i, int j);
};

// ----------------------------------------------------------------------------
// utility functions to compute indices in heap:
// parent, left child, right child, effective index
// ----------------------------------------------------------------------------
static inline int HeapParent(int i) { return i / 2; }
static inline int HeapRight(int i) { return i * 2 + 1; }
static inline int HeapLeft(int i) { return i * 2; }
static inline int HeapIndex(int i) { return i - 1; }

// ----------------------------------------------------------------------------
// template <class COMPARABLE>
// HEAP<COMPARABLE>::HEAP( int fn(COMPARABLE c1, COMPARABLE c2));
//
// constructor
//
// optional parameter: a comparison function that takes as parameters
// to instances of the heap element type (c1 y c2, respectively), and
// returns:
//    -1 if c1 < c2
//     1 if c1 > c2
//     0 if c1 == c2
//
// if compare function is not specified, then the heap element type must be
// directly comparable (e.g., int, float, doble, etc)
// ----------------------------------------------------------------------------
template <class COMPARABLE>
HEAP<COMPARABLE>::HEAP(void) {fnCompare = 0;}

template <class COMPARABLE>
HEAP<COMPARABLE>::HEAP(int fn(COMPARABLE c1, COMPARABLE c2))
{	fnCompare = fn ? fn : 0;	}

// ----------------------------------------------------------------------------
// template <class COMPARABLE>
// void HEAP<COMPARABLE>::Swap(int i, int j)
//
// utility function to swap 2 elements of the heap
// ----------------------------------------------------------------------------
template <class COMPARABLE>
void HEAP<COMPARABLE>::Swap(int i, int j)
{
	if ((i >= 1) && (i <= size()) &&
	    (j >= 1) && (j <= size()))
	{
		COMPARABLE t = items[HeapIndex(i)];
		items[HeapIndex(i)] = items[HeapIndex(j)];
		items[HeapIndex(j)] = t;
	}
}

// ----------------------------------------------------------------------------
// template <class COMPARABLE>
// COMPARABLE HEAP<COMPARABLE>::ExtractMin(void)
//
// return the least element in the heap
//
// ----------------------------------------------------------------------------
template <class COMPARABLE>
COMPARABLE HEAP<COMPARABLE>::ExtractMin(void)
{
	COMPARABLE t = items[HeapIndex(1)];
	Swap(1, size());
	items.pop_back();
	HeapifyDown(1);
	return t;
}

// ----------------------------------------------------------------------------
// template <class COMPARABLE>
// int HEAP<COMPARABLE>::size(void)
//
// return number of items in the heap
// ----------------------------------------------------------------------------
template <class COMPARABLE>
int HEAP<COMPARABLE>::size(void)
{	return items.size();	}

// ----------------------------------------------------------------------------
// template <class COMPARABLE>
// bool HEAP<COMPARABLE>::empty(void)
//
// return true if heap is empty
// ----------------------------------------------------------------------------
template <class COMPARABLE>
bool HEAP<COMPARABLE>::empty(void)
{	return (size() == 0); 	}

// ----------------------------------------------------------------------------
// template <class COMPARABLE> int HEAP<COMPARABLE>::Compare(int i, int j)
//
// compare two heap elements, identified by heap indices i and j, respectively
//
// if a compare function has been provided, then comparison is delegated to
// the compare function, which must conform to the following return values:
//
// returns:
//	-1 if element(i) < element(j)
//   1 if element(i) > element(j)
//   0 if element(i) == element(j)
// ---------------------------------------------------------------------------- 
template <class COMPARABLE>
int HEAP<COMPARABLE>::Compare(int i, int j)
{
	if (fnCompare) 
    {
		return fnCompare(items[HeapIndex(i)], items[HeapIndex(j)]);
    }
    
    if (items[HeapIndex(i)] < items[HeapIndex(j)]) return -1;
    if (items[HeapIndex(i)] > items[HeapIndex(j)]) return 1;
    return 0;
}

// ----------------------------------------------------------------------------
// template <class COMPARABLE> void HEAP<COMPARABLE>::HeapifyDoan(int i)
//
// restore heap property downward, beginning with element i.
//
// this function assumes that, except for element indicated by i, the heap
// property holds for all other elements
// ----------------------------------------------------------------------------
template <class COMPARABLE>
void HEAP<COMPARABLE>::HeapifyDown(int i)
{
	int n = size();
	while (i < n)
	{
		int left = HeapLeft(i);
		int right = HeapRight(i);
		int min = i;
		
		if ((left <= n) && (Compare(left, i) < 0))
			min = left;
			
		if ((right <= n) && (Compare(right, min) < 0))
			min = right;
			
		if (min == i)
			break;
			
		Swap(i, min);
		
		i = min;
	}
}

// ----------------------------------------------------------------------------
// template <class COMPARABLE> void HEAP<COMPARABLE>::HeapifyUp(int i)
//
// restore heap property upward, beginning with element i.
//
// this function assumes that, except for element indicated by i, the heap
// property holds for all other elements
// ----------------------------------------------------------------------------
template <class COMPARABLE>
void HEAP<COMPARABLE>::HeapifyUp(int i)
{
	while (i > 1)
	{
		int parent = HeapParent(i);
		if (Compare(i, parent) >= 0)
			break;			
		Swap(i, parent);
		i = parent;
	}
}


// ----------------------------------------------------------------------------
// template <class COMPARABLE> void HEAP<COMPARABLE>::AddItem(COMPARABLE c)
//
// add an element to the end of the heap and restore the heap property
// ----------------------------------------------------------------------------
template <class COMPARABLE>
void HEAP<COMPARABLE>::AddItem(COMPARABLE c)
{
	items.push_back(c);
	HeapifyUp(items.size());
}

#endif