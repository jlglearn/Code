import array;
from random import randint;

def CalcComponentsCallback( op, i, v, l, r, state ):
    if op == 1:
        iComponent = len(state['C'])-1;
        s = set([v]);
        state['C'][iComponent] |= s;
        state['v'] |= s;
    else:
        pass;
    return (1,None);
	
class Graph:
    def __init__(self):
        self.V = {};
        self.E = {};
        self.ES = set();
        self.edgeId = 0;
        pass;
        
    def AddVertex(self, v):
        if v in self.V: return 0;
        self.V[v] = {'V':set(), 'E':set()};
        return 1;
        
    def Load(self, filename ):
        f = open(filename);
        fl = list(f);
        
        for oneLine in fl:
            values = [int(x) for x in oneLine.split()];
            v = values[0];
            
            self.AddVertex(v);
            
            if len(values) > 1:
                #convert to set to eliminate potential duplicates
                es = set(values[1:]);
                
                #and back to list for easy processing
                el = list(es);
                
                for e in el:
                    if e != v:
                        self.AddVertex(e);
                        
                        edge = set([v,e]);
                        
                        if not (edge in self.ES):
                            self.ES |= set(edge);
                            eId = self.edgeId;
                            self.edgeId += 1;
                            self.E[eId] = edge;
                            self.V[v]['E'] |= set([eId]);
                            self.V[e]['E'] |= set([eId]);
                            self.V[v]['V'] |= set([e]);
                            self.V[e]['V'] |= set([v]);
        
        f.close();
        
    def Vertices(self):
        l = [v for v in self.V.keys()];
        l.sort();
        return l;
        
    def Edges(self):
        return {e:self.E[e] for e in self.E.keys()};
        
    def DegreeOf(self,v):
        if v in self.V:
            return len(self.V[v]['E']);
        return 0;
        
    def AvgDegree(self):
        return (2*len(self.E))/len(self.V);
        
    def VertexCount(self):
        return len(self.V);
        
    def EdgeCount(self):
        return len(self.E);
        
    def OneVertex(self):
        i = randint(0,len(self.V)-1);
        return self.V.keys()[i];
        
    def NeighborsOf(self,v):
        if v in self.V:
            l = list(self.V[v]['V']);
            l.sort();
            return l;
        return None;
        
        
    def BFT(self, s, callback, arg):
    
        if not s in self.V:
            return 0;
        
        q = array.array('i');        
        visited = {};
        
        callback(0,None,None,None,None,arg);  #callback(traverse_start, x,x,x,x,arg)
            
        q.append(s);
        visited[s] = {'L':0, 'P':[s]};

        i = 0;
        returnValue = 1;
        
        while len(q) > 0:
            v = q.pop(0);
            p = [x for x in visited[v]['P']];
            l = visited[v]['L'];
            
            (op, returnValue) = callback(1, i, v, l, s, arg);
            i += 1;
            
            if (op == 0):
                callback(2, i, None, None, None, arg);
                return returnValue;
                
            N = self.NeighborsOf(v);
            
            for u in N:
                if not u in visited:
                    visited[u]={'L': l+1, 'P':p};
                    visited[u]['P'].append(u);
                    q.append(u);
        
        callback(2,i,None,None,None,arg);     #callback(traverse_end, count, x, x, x)
        return 1;
		
    def Components(self):
        state = { 'C':{}, 'v':set() };
        for node in self.V:
            #if we haven't explored node, then start a BFT rooted on it.
            if not (node in state['v']):
                iComponent = len(state['C']);
                s = set([node]);
                state['C'][iComponent] = set();
                state['C'][iComponent] |= s;
                state['v'] |= s;
                self.BFT(node, CalcComponentsCallback, state);
        return state['C'];
        
    def DFT(self, s, callback, arg):
    
        if not s in self.V:
            return 0;
        
        stack = array.array('i');
        visited = {};
        i = 0;
        
        callback(0,None,None,None,None,arg);
        
        visited[s]={'L':0, 'P':[s], 'R':False};
        stack.append(s);
        
        while len(stack) > 0:
        
            v = stack.pop();
            p = visited[v]['P'];
            l = visited[v]['L'];
            
            if not visited[v]['R']:
                visited[v]['R'] = True;
                (o,r) = callback(1,i,v,l,s,arg);
                i += 1;
                if o == 0:
                    callback(2,i,None,None,None,arg);
                    return r;
                    
            for n in self.NeighborsOf(v):
            
                if not (n in visited):
                
                    visited[n]={'L':l+1, 'P':[x for x in p], 'R':False};
                    visited[n]['P'].append(n);
                    
                    stack.append(v); #there may be some unvisited neighbors of v
                    stack.append(n); #but we now want to first explore n and its neighbors
                    break; #break out of for loop to explore n and its neighbors;
                    
        callback(2,i,None,None,None,arg);
        return 1;
 
