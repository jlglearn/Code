#include <iostream>
#include <cstdlib>

static int SelectPivot( int A[], int s, int e )
{
    return s;
}

static void Swap( int A[], int i, int j )
{
    int t = A[i];
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

int main(int argc, char *argv[])
{
    int A[32], i, n = sizeof(A)/sizeof(A[0]);
    
    for(i = 0; i < n; i++ )
    {
        A[i] = (int)((double)10000*rand()/RAND_MAX);
    }
    
    QuickSort(A,0,n);
    
    for(i = 0; i < n; i++ )
    {
        std::cout << A[i] << " ";
    }
    std::cout << "\n";
    
    for(i = 0; i < n-1; i++ )
    {
        if (A[i+1] < A[i])
        {
            std::cout << "Error in index [" << i << "] A[i]=" << A[i] << " A[i+1]=" << A[i+1] << "\n";
        }
    }
}