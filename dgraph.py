from array import array;
from random import randint;

class DGraph:
    #V = dictionary of vertices, indexed by vertex id, each of which is a dictionary of the form:
    #    {'OE': set of outgoing edges, 'IE': set of incoming edges}
    
    #E = dictionary of edges, indexed by ID of origin vertex, of the form: set( destination vertices );
    
    def __init__(self):
        self.V = {};
        self.m = 0;
        self.E = {};

    def BFS(v,fn,arg):
        pass;
    
    def EdgeCount(self):
        return len(self.E);
        
    def Load(self,fn):
        f = open(fn);
        i = 0;
        
        for oneLine in f:
            values = oneLine.split();
            [o,d] = [int(x) for x in values];
            
            if not o in self.V: 
                self.V[o]={'OE':set(), 'IE':set()};
                self.E[o]=set();
                
            if not d in self.V: 
                self.V[d]={'OE':set(), 'IE':set()};
                self.E[d]=set();
            
            if not (d in self.V[o]['OE']):
                self.V[o]['OE'] |= set([d]);
                self.V[d]['IE'] |= set([o]);
                self.E[o] |= set([d]);
            
            i += 1;
            
        print "Read ", i, "lines: |V|=", self.VertexCount(), " |E|=", self.EdgeCount();
		