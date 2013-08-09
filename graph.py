from random import randint;

class Graph:

    def __init__(self):
        self.E = {};
        self.m = 0;
        self.directed = False;
        self.INFINITY = 1000000;
        
    def AddVertex(self, v):
        if not v in self.E:
            self.E[v] = {};
            
    def VertexCount(self):
        return len(self.E);
        
    def EdgeCount(self):
        return self.m;
        
    def OneVertex(self):
        return self.E.keys()[randint(0,len(self.E.keys())-1)];
        
    def NeighborsOf(self, v):
        if not v in self.E:
            return None;
        return [x for x in self.E[v]];

    def CostOf(self,s,d):
        if not ((s in self.E) and (d in self.E)):
            return None;
        return self.E[s][d] if (d in self.E[s]) else self.INFINITY;
        
    def Load(self, filename, directed=False ):
        
        with open(filename, 'r') as f:
        
            for oneLine in f:
            
                elements = oneLine.split();                
                s = int(elements[0]);
                dl = elements[1:];
                
                if not s in self.E:
                    self.AddVertex(s);
                
                for tstr in dl:
                
                    if len(tstr.split(',')) == 2:
                        #tstr is a tuple of the form 'd,c', where:
                        #d is a destination vertex id, and 
                        #c the cost associated with the edge
                        
                        t = tstr.split(',')
                        d = int(t[0]);
                        c = int(t[1]);
                    else:
                        d = int(tstr);
                        c = 1;
                        
                    if not d in self.E:
                        self.AddVertex(d);
                        
                    if not d in self.E[s]:
                        self.m += 1;
                        
                    self.E[s][d] = c;
                    
                    if not self.directed:
                        self.E[d][s] = c;
                        
                        if not s in self.E[d]:
                            self.m += 1;
 
        return len(self.E);

    def Export(self, filename, format):
    
        if format != "net":
            return None;
            
        with open(filename, 'w') as f:
        
            f.write('*vertices {}\n'.format(len(self.E)));
            
            for v in self.E:
                f.write('{}\n'.format(v));
                
            f.write('*arcs\n');
            
            for s in self.E:
                for d in self.E[s]:
                    f.write('{}\t{}\t{}\n'.format(s,d,self.CostOf(s,d)));
                    
        return 1;
        
    def SP(self, v):
    
        if not v in self.E:
            return None;
        
        tick = 0;
        A = {u:{'c':self.INFINITY, 't':None, 'p':None, 'h':None} for u in self.E};
        A[v] = {'c':0, 't':tick, 'p': None, 'h':[]};
        
        X = {};
        X[v] = tick;
        
        F = {};
        F[v] = tick;
        tick += 1;
        
        while len(F) > 0:
        
            nextArc = {'s':None, 'd':None, 'c':self.INFINITY, 'h':None};
            removeSet = set();
            
            # for each node on the frontier (vertices in X with at least one edge to vertices not in X)
            for s in F:
                
                As = A[s]['c'];
                # assume done with vertex s
                sDone = True;     
                
                # for each of its neighbors
                N = self.NeighborsOf(s);
                for d in N:
                
                    # only consider nodes for which a shortes path is still unknown
                    if not d in X:
                    
                        sDone = False; # this node unvisited, not yet done with s
                        
                        Ad = As + self.CostOf(s,d);
                        
                        if Ad < nextArc['c']:
                        
                            #found a candidate with distance shorter than the 
                            #previous best candidate
                            
                            nextArc['s'] = s;
                            nextArc['d'] = d;
                            nextArc['c'] = Ad;
                            nextArc['h'] = A[s]['h']+[d];
                    
                if sDone:
                    removeSet |= set([s]);

                
            if nextArc['d'] == None:
                #no candidates found
                break;
                
            for w in removeSet:
                #remove from the frontier all vertices that don't have at least one
                #unprocessed vertex
                del F[w];
                
            #pull best candidate into the set of nodes for which a shortest path is known, and record its data
            X[nextArc['d']] = tick;
            F[nextArc['d']] = tick;
            A[nextArc['d']] = {'c': nextArc['c'], 't':tick, 'p':nextArc['s'], 'h':nextArc['h']};
            tick += 1;
            
        return A;
