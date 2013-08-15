def SelectPivot(l,r):
    global A;
    return l;

def Swap(i,j):
    global A;
    t = A[i];
    A[i] = A[j];
    A[j] = t;
    
def QS(l,r):
    global A;
    
    n = r-l;
    
    if n < 2:
        #nothing to do
        return n;
    elif n == 2:
        if A[l+1] < A[l]: Swap(l,l+1);
        return n;
    else:
    
        ip = SelectPivot(l,r);
        if ip != l: Swap(ip,l);
        
        pv = A[l];
        j = l+1;
        i = l+1;
        
        while j < r:
            if A[j] <= pv:
                Swap(i,j);
                i += 1;
            j += 1;
            
        Swap(l,i-1);
        
        QS(l,i-1);
        QS(i,r);
        return n;
