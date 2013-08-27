class Vector:

    def __init__(self, labels, f={}):
        self.labels = [];
        self.values = {};
        self.labels = labels;
        for l in f.keys():
            if f[l] != 0:
                self.values[l] = f[l];
        
    def __add__(self, other):
        assert(isinstance(other, Vector));
        assert(self.labels == other.labels);
        v = Vector(self.labels,{});
        for l in self.labels:
            x = 0;
            x += self.values[l] if l in self.values else 0;
            x += other.values[l] if l in other.values else 0;
            if x != 0:
                v.values[l] = x;
        return v;
        
    def __sub__(self, other):
        return self + (other * -1);
        
    def __mul__(self, other):
        if isinstance(other,Vector):
            x = 0;
            for l in self.labels:
                x += ((self.values[l] * other.values[l]) if ((l in self.values) and (l in other.values)) else 0);
            return x;
        elif isinstance(other, (int,long,float)):
            v = Vector(self.labels,{});
            for l in self.labels:
                x = other * self.values[l] if l in self.values else 0;
                if x != 0:
                    v.values[l] = x;
            return v;
        return None;
        
    def __div__(self, other):
        assert(isinstance(other, (int, long, float)));
        assert(other != 0);
        return self * float(1.0/other);
        
        
class Matrix:

    def __init__(self, n, m, f={}):
        assert((n != 0) and (m != 0));
        self.n = n;
        self.m = m;
        self.values = {};
        for k in f.keys():
            if (f[k] != 0):
                r,c = k;
                self.values[(r,c)] = f[k];
                
    def __add__(self, other):
        assert( isinstance(other, Matrix));
        if not isinstance(other, Matrix):
            return None;
            
        f = {};
        assert((self.n == other.n) and (self.m == other.m));
        for r in range(self.n):
            for c in range(self.m):
                x = 0;
                x += self.values[(r,c)] if (r,c) in self.values else 0;
                x += other.values[(r,c)] if (r,c) in other.values else 0;
                if x != 0:
                    f[(r,c)] = x;

        return Matrix(self.n, self.m, f);
        
    def __mul__(self, other):
        assert(isinstance(other,(int,long,float)));
        if not (isinstance(other,(int,long,float))):
            return None;
        f = {};
        if other != 0:
            for r in range(self.n):
                for c in range(self.m):
                    if ((r,c) in self.values) and (self.values[(r,c)] != 0):
                        f[(r,c)] = self.values[(r,c)] * other;
                        
        return Matrix(self.n, self.m, f);
                    
    def __div__(self, other):
        assert(isinstance(other, (int,long,float)));
        assert(other != 0);
        if not (isinstance(other, (int,long,float))):
            return None;
        if other == 0:
            return None;
        return self * float(1.0/other);
        
    def __sub__(self, other):
        assert( isinstance(other, Matrix));
        if not isinstance(other, Matrix):
            return None;
            
        f = {};
        assert((self.n == other.n) and (self.m == other.m));
        for r in range(self.n):
            for c in range(self.m):
                x = 0;
                x += self.values[(r,c)] if (r,c) in self.values else 0;
                x -= other.values[(r,c)] if (r,c) in other.values else 0;
                if x != 0:
                    f[(r,c)] = x;

        return Matrix(self.n, self.m, f);