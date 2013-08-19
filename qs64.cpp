#include <cstdlib>
#include "qs64.h"

static int SelectPivot(int64_t A[], int s, int e)
{
    return s;
}
static int Swap(int64_t A[], int i, int j)
{
    int64_t t = A[i];
    A[i] = A[j];
    A[j] = t;
}

void QSort64(int64_t A[], int s, int e)
{
    int n = e - s;
    
    if (n > 2)
    {
        int p, l, r;
        int64_t v;
        
        p = SelectPivot(A,s,e);
        if ( p != s )
        {
            Swap(A,p,s);
        }
        v = A[s];
        
        l = s+1;
        r = s+1;
        
        while (r < e)
        {
            if ( A[r] <= v )
            {
                Swap(A,r,l);
                l++;
            }
            r++;
        }
        
        Swap(A,s,l-1);
        QSort64(A,s,l-1);
        QSort64(A,l,e);
    
    }
    else if (n == 2)
    {
        if (A[s+1] < A[s])
        {
            Swap(A,s,s+1);
        }
    }
}
