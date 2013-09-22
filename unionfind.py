import sys;
from random import randint;

class UnionFind:

    def __init__(self, n):
        assert(isinstance(n,int) and (n>0));
        self.n = n;
        self.A = [i for i in range(n)];
        self.size = [1 for i in range(n)];
        self.components = {i:0 for i in range(n)};
        
    def find(self, p):
        assert((p >= 0) and (p < self.n));
        while (p != self.A[p]):
            self.A[p] = self.A[self.A[p]];
            p = self.A[p];
        return p;
        
    def union(self, p, q):
        assert((p >= 0) and (p < self.n) and (q >= 0) and (q < self.n));
        if p != q:
            proot = self.find(p);
            qroot = self.find(q);
            
            if (proot != qroot):
                if self.size[proot] <= self.size[qroot]:
                    self.A[proot] = qroot;
                    self.size[qroot] += self.size[proot];
                    del self.components[proot];
                else:
                    self.A[qroot] = proot;
                    self.size[proot] += self.size[qroot];
                    del self.components[qroot];
                    
    def connected(self, p, q):
        assert((p >= 0) and (p < self.n) and (q >= 0) and (q < self.n));
        if p == q: return True;
        proot = self.find(p);
        qroot = self.find(q);
        return (proot == qroot);
        
    def count(self):
        return len(self.components);
        