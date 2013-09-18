#include <cstring>
#include <cstdlib>

static void Swap(void *pA, int elementSize, int i, int j)
{   
    unsigned char *p = (unsigned char *)pA;    
    unsigned char t[elementSize];
    void *pI = (void *) &p[elementSize * i];
    void *pJ = (void *) &p[elementSize * j];
    void *pT = (void *) &t[0];

    memcpy(pT, pI, elementSize);
    memcpy(pI, pJ, elementSize);
    memcpy(pJ, pT, elementSize);
}

static int SelectPivot(void *pA, int elementSize, int start, int end, float (*fnVal)(void *pA, int i))
{
    return start;
}

void QSortX(void *pA, int elementSize, int start, int end, float (*fnVal)(void *pA, int i))
{
    int n = end - start;
    
    if (n > 2)
    {
        int p = SelectPivot(pA, elementSize, start, end, fnVal);
        
        if (p != start)
            Swap(pA, elementSize, p, start);
            
        int l = start + 1;
        int r = start + 1;
        float v = fnVal(pA, start);
        
        while (r < end)
        {
            if ( fnVal(pA, r) <= v )
            {
                Swap(pA, elementSize, l, r);
                l++;
            }
            
            r++;
        }
        
        Swap(pA, elementSize, start, l-1);
        QSortX(pA, elementSize, start, l-1, fnVal);
        QSortX(pA, elementSize, l, end, fnVal);
    }
    else if (n == 2)
    {
        if ( fnVal(pA, start) > fnVal(pA, start+1) )
            Swap(pA, elementSize, start, start+1);
    }
}