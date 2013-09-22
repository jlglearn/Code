HeapLeft = lambda i : i*2;
HeapRight = lambda i : i*2+1;
HeapParent = lambda i : i//2;

class Heap:

    def __init__(self):
        self.A = [];
        self.nElements = 0;
        
    def Value(self, i):
        assert((i > 0) and (i <= self.nElements));
        return self.A[i-1]['key'];
        
    def Swap(self, i, j):
        assert((i > 0) and (i <= self.nElements) and (j > 0) and (j <= self.nElements));
        t = self.A[i-1];
        self.A[i-1] = self.A[j-1];
        self.A[j-1] = t;
        
    def HeapifyDown(self, i):
        
        l = HeapLeft(i);
        r = HeapRight(i);
        
        m = i;
        
        if ((l <= self.nElements) and (self.Value(l) < self.Value(i))):
            m = l;
            
        if ((r <= self.nElements) and (self.Value(r) < self.Value(m))):
            m = r;
            
        if m == i: return;
        
        self.Swap(m, i);
        self.HeapifyDown(m);
        
    def HeapifyUp(self, i):
        if i == 1:
            return;
            
        p = HeapParent(i);
        l = HeapLeft(p);
        r = HeapRight(p);
        
        m = p;
        
        if ((l <= self.nElements) and (self.Value(l) < self.Value(p))):
            m = l;
            
        if ((r <= self.nElements) and (self.Value(r) < self.Value(m))):
            m = r;
            
        if m == p:
            #parent strictly less than children, done
            return;
            
        self.Swap(m, p);
        self.HeapifyUp(p);
        
    def Element(self, i):
        return self.A[i-1];
        
    def Insert(self, key, data):
        self.A.append({'key':key, 'data':data});
        self.nElements += 1;
        self.HeapifyUp(self.nElements);
        
    def Empty(self):
        return self.nElements == 0;
        
    def Pop(self):
        assert(self.nElements > 0);
        x = self.Element(1);
        self.Swap(1, self.nElements);
        del self.A[self.nElements-1];
        self.nElements -= 1;
        self.HeapifyDown(1);
        return x;
        
        
        