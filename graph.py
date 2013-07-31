import array;
from random import randint;

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
        
    def BFT(self, s, callback):
        if not s in self.V:
            return 0;
        
        q = array.array('i');        
        visited = {};
        
        callback(0,None,None,None);  #callback(traverse_start, x,x,x)        
        q.append(s);
        visited[s] = {'L':0, 'P':[s]};

        i = 0;
        while len(q) > 0:
            v = q.pop(0);
            N = self.NeighborsOf(v);
            for u in N:
                if not u in visited:
                    visited[u]['L'] = visited[v]['L'] + 1;
                    visited[u]['P'] = [x for x in visited[v]['P']].append(u);
                    q.append(u);
            callback(1,i,v,visited[v]['L']);    #callback(traverse_item, i, item, level)
            i += 1;
            
        callback(2,i,None,None);     #callback(traverse_end, count, x, x)
        return 1;
        
     
