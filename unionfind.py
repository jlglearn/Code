import sys;
from random import randint;

class UnionFind:

    def __init__(self, n):
        self.grid = [-1 for i in range(n*n)];
        self.size = [1 for i in range(n*n)];
        self.dimension = n;
        
    def getIndex(i,j):
        assert((i>=1) and (i<=self.dimension));
        assert((j>=1) and (j<=self.dimension));    
        return ((i-1)*self.dimension) + (j-1);
        
    def is_open(self,i,j):
        n = self.getIndex(i,j);
        return True if (self.grid[n] != -1) else False;
        
    def open(self,i,j):
        n = self.getIndex(i,j);
        if self.grid[n] == -1:
            #first assume site is disconnected
            self.grid[n] = n;
            
            #now check connectivity with 4 neighbors
            
            #top neighbor
            if (i > 1) and self.is_open(i-1,j):
                self.connect((i,j),(i-1,j));
                
            #left neighbor
            if (j > 1) and self.is_open(i,j-1):
                self.connect((i,j),(i,j-1));
                
            #right neighbor
            if (j < self.dimension) and self.is_open(i,j+1):
                self.connect((i,j),(i,j+1));
                
            #bottom neighbor
            if (i < self.dimension) and self.is_open(i+1,j):
                self.connect((i,j),(i+1,j));
        
    def findRoot(self,i,j):
        p = self.getIndex(i,j);
        while p != self.grid[p]:
            self.grid[p] = self.grid[self.grid[p]];
            p = self.grid[p];
        return p;
        
    def connect(self,p,q):
        ip,jp = p;
        iq,jq = q;
        np = self.getIndex(ip,jp);
        nq = self.getIndex(iq,jq);
        
        rootP = self.findRoot(ip,jp);
        rootQ = self.findRoot(iq,jq);
        
        if self.size[rootP] <= self.size[rootQ]:
            self.grid[rootP] = rootQ;
            self.size[rootQ] += self.size[rootP];
        else:
            self.grid[rootQ] = rootP;
            self.size[rootP] += self.size[rootQ];

def createDataset(fname, n, m):
    print "Creating dataset into file {} (n={}, m={}).\n".format(fname,n,m);
    
    if m > n: m = n;
    if m < 0: m = 0;
    
    l = [False for i in range(n)];
    v = [];
    c = 0;
    
    while c < m:
    
        x = randint(0,n-1);
        if l[x]: 
            #already set this value, skip
            continue;
            
        l[x] = True;
        v.append(x);
        c += 1;
        
    with open(fname, "w") as f:
        f.write("{}\n".format(n));
        
        for w in v:
            f.write("{}\n".format(w));
            
def main():


    if (len(sys.argv) >= 5) and (sys.argv[1] == "create"):
        assert(len(sys.argv)>=4);
        n = int(sys.argv[2]);
        m = int(sys.argv[3]);
        f = sys.argv[4] if (len(sys.argv) > 4) else "dataset.txt";
        createDataset(f, n, m);
        return 0;        
    else:
        print "Usage {} [create n m filename] where".format(sys.argv[0]);
        print "   n is array dimension";
        print "   m is number of unique randomly opened sites";
        print "   filename is the name of dataset file to be written.";
        return 1;
            
if __name__ == "__main__":
    main();