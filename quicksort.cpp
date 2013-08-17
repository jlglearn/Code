static int SelectPivot( int A[], int s, int e )
{
    return s;
}

static int SelectPivot64( int64_t A[], int s, int e )
{
    return s;
}

static void Swap( int A[], int i, int j )
{
    int t = A[i];
    A[i] = A[j];
    A[j] = t;
}

static void Swap64( int64_t A[], int i, int j )
{
    int64_t t = A[i];
    A[i] = A[j];
    A[j] = t;
}


void QuickSort( int A[], int s, int e )
{
    int n = e - s;
    
    if ( n > 2 )
    {
        int iPivot = SelectPivot(A,s,e);
        
        if (iPivot != s)
        {
            Swap(A,iPivot,s);
        }
        
        int p = s+1, j = s+1, v = A[s];
        
        while (j < e)
        {
            if (A[j] <= v)
            {
                Swap(A,p,j);
                p++;
            }
            
            j++;
        }
        
        Swap(A,s,p-1);
        QuickSort(A,s,p-1);
        QuickSort(A,p,e);
    }
    else if ( n == 2 )
    {
        if (A[s+1] < A[s])
        {
            Swap(A,s,s+1);
        }
    }
}

void QuickSort64( int64_t A[], int s, int e )
{
    int n = e - s;
    
    if ( n > 2 )
    {
        int iPivot = SelectPivot64(A,s,e);
        
        if (iPivot != s)
        {
            Swap64(A,iPivot,s);
        }
        
        int p = s+1, j = s+1;
        int64_t v = A[s];
        
        while (j < e)
        {
            if (A[j] <= v)
            {
                Swap64(A,p,j);
                p++;
            }
            
            j++;
        }
        
        Swap64(A,s,p-1);
        QuickSort64(A,s,p-1);
        QuickSort64(A,p,e);
    }
    else if ( n == 2 )
    {
        if (A[s+1] < A[s])
        {
            Swap64(A,s,s+1);
        }
    }
}