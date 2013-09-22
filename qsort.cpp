#include <cstring>
#include "qsort.h"

static int SelectPivot(int a[], int start, int end)
{
    return start;
}

static int SelectPivotX(void *P, int size, int start, int end, float fn(void *P, int i))
{
    return start;
}

static void Swap(int a[], int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

static void SwapX(void *P, int size, int i, int j)
{
    unsigned char t[size];
    unsigned char *pt = (unsigned char *)P;
    
    memcpy((void *) &t[0],         (void *) &pt[size * i], size);
    memcpy((void *) &pt[size * i], (void *) &pt[size * j], size);
    memcpy((void *) &pt[size * j], (void *) &t[0],         size);
}    

void QSort(int a[], int start, int end)
{
    int n = end - start;
    
    if (n > 2)
    {
        int p = SelectPivot(a, start, end);
        if (p != start) Swap(a, start, p);
        
        int v = a[start];
        int eq = start;
        int gt = end;
        int i = start + 1;
        
        while (i < gt)
        {
            if (a[i] < v)
                Swap(a, i++, eq++);
            else if (a[i] > v)
                Swap(a, i, --gt);
            else
                i++;
        }
        
        QSort(a, start, eq);
        QSort(a, gt, end);
    }
    else if (n == 2)
    {
        if (a[start] > a[start+1])
            Swap(a, start, start+1);
    }
}

void QSortX(void *P, int size, int start, int end, float fn(void *P, int i))
{
    int n = end - start;
    
    if (n > 2)
    {
        int p = SelectPivotX(P, size, start, end, fn);
        if (p != start) SwapX(P, size, start, p);
        
        float v = fn(P, start);
        int eq = start;
        int gt = end;
        int i = start + 1;
        
        while (i < gt)
        {
            float w = fn(P, i);
            
            if (w < v)
                SwapX(P, size, i++, eq++);
            else if (w > v)
                SwapX(P, size, i, --gt);
            else
                i++;
        }
        
        QSortX(P, size, start, eq, fn);
        QSortX(P, size, gt, end, fn);
    }
    else if (n == 2)
    {
        if (fn(P, start) > fn(P, start+1))
            SwapX(P, size, start, start+1);
    }
}