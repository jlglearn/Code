import sys;

A = [];

HeapLeft = lambda i: 2*i;
HeapRight = lambda i: 2*i + 1;
HeapParent = lambda i: i//2;

def HeapElement(i):
    global A;
    return A[i-1];

def HeapSwap(i,j):
    global A;
    x = A[i-1];
    A[i-1] = A[j-1];
    A[j-1] = x;
    

def MinHeapify(i):
    global A;
    
    l = HeapLeft(i);
    r = HeapRight(i);
    
    m = i; 
    if (l <= len(A)) and (HeapElement(l) < HeapElement(i)):
        m = l; 
    
    if (r <= len(A)) and (HeapElement(r) < HeapElement(m)):
        m = r;         
        
    if (i != m):
        HeapSwap(i,m);
        MinHeapify(m);

def HeapPop():
    global A;
    if len(A) > 0:
        x = A[0];
        HeapSwap(1,len(A));
        A.pop();
        MinHeapify(1);    
        return x;
        
def HeapInsert(k):
    global A;
    A.append(k);
    i = len(A);
    while i != 1:
        j = HeapParent(i);
        ki = HeapElement(i);
        kj = HeapElement(j);
        
        if ki > kj:
            # if child (ki) is greater than its parent (kj), heap holds, done.
            break;
        elif ki < kj:
            # if child (ki) is less than its parent (kj), swap them and continue checking upward
            HeapSwap(i, j);
        else:
            # if child (ki) is equal to its parent (kj), make sure it's on parent's left subtree (by convention)
            l = HeapLeft(j);
            
            if i == HeapLeft(j):
                #indeed left child, done
                break;
            else:
                #swap left and right children of parent, we are now done
                HeapSwap(i,l);
                break;
        
        i = j;

def MakeMinHeap():
    global A;
        
    i = len(A)//2;
    
    while i > 0:
        MinHeapify(i);
        i -= 1;

def LoadFile(filename):
    global A;
    with open(filename,'r') as f:
        for l in f:
            for x in l.split():
                A.append(int(x));
    f.close();
    
def WriteFile(A,filename):
    with open(filename, 'w') as f:
        for x in A:
            f.write("{}\n".format(x));
            
def PrintSequence():
    global A;
    s1 = "";
    s2 = "";
    
    for i in range(len(A)):
        s1 += "|{0:3d}".format(i+1);
        s2 += "|{0:3d}".format(A[i]);
        
        if (i%16) == 15:
            print s1 + "\n" + s2 + "\n";
            s1 = "";
            s2 = "";

            