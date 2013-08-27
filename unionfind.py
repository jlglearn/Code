class UnionFind:

    def __init__(self, n):
        self.grid = [-1 for i in range((n*n)+1)];
        self.size = [1 for i in range((n*n))];
        
        self.dimension = n;
        self.topSite = len(self.grid)-1;
        
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
        
        pass;
    
        