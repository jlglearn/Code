#include "qsx.h"

static INT selectPivot(INT a[], INT start, INT end, double (*fnVal)(INT))
{
    return start;
}

static void swap(INT a[], INT i, INT j)
{
    INT t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void QSx(INT a[], INT start, INT end, double (*fnVal)(INT))
{
    INT n = end - start;
    
    if (n > 2)
    {        
    
        INT p = selectPivot(a, start, end, fnVal);
        
        if ( p != start )
        {
            swap(a, p, start);
        }
        
        INT i, j;
        double w;
        
        i = start + 1;
        j = start + 1;
        w = fnVal(a[start]);
        
        while ( j < end )
        {
            if ( fnVal(a[j] <= w ))
            {
                swap(a, i, j);
                i++;
            }
            j++;
        }
        
        swap(a, start, i-1);
        QSx(a, start, i-1, fnVal);
        QSx(a, i, end, fnVal);
    }
    else if (n == 2)
    {
        if (fnVal(a[start]) > fnVal(a[start+1]))
        {
            swap(a, start, start+1);
        }
    }
}