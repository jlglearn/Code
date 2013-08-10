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
        
def HeapInsert(x):
    global A;
    
    A.append(x);
    HeapSwap(1,len(A));
    MinHeapify(1);

def HeapPop():
    global A;
    if len(A) > 0:
        x = A[0];
        HeapSwap(1,len(A));
        A.pop();
        MinHeapify(1);    
        return x;

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

            