import array;
from random import randint;
    
def BFSCallback(op, i, v, l, s, state):
    if op == 1:
        if v == state['v']:
            state['d'] = l;
            return (0, l);
    elif op == 0:
        state['d'] = -1;
    elif op == 2:
        return (0, state['d']);
        
    return (1,None);
            

class DGraph:
    #V = dictionary of vertices, indexed by vertex id, each of which is a dictionary of the form:
    #    {'OE': set of outgoing edges, 'IE': set of incoming edges}
    
    #E = dictionary of edges, indexed by ID of origin vertex, of the form: set( destination vertices );
    
    def __init__(self):
        self.V = {};
        self.m = 0;
        self.E = {};

    def NeighborsOf(self, v):
        l = list(self.E[v]);
        l.sort();
        return l;
               
    def DFT(self, s, fn, arg):
        
        if not s in self.V:
            return 0;
            
        i = 0;
        stack = array.array('i');
        visited = {};
        
        fn(0,None,None,None,None,arg);
        
        stack.append(s);
        visited[s] = {'L':0, 'R':False, 'i':0};
        
        while len(stack) > 0:
        
            v = stack.pop();
            l = visited[v]['L'];
            
            if not visited[v]['R']:
                visited[v]['R'] = True;
                (action, result) = fn(1,i,v,l,s,arg);
                i += 1;
                
                if action == 0:
                    fn(2,i,None,None,None,arg);
                    return result;
                    
            #look at neighbors of v
            N = self.NeighborsOf(v);
            j = visited[v]['i'];
            while (j < len(N)):
                u = N[j];
                j += 1;
                
                if not u in visited:
                    #haven't looked at u yet
                    stack.append(v); #there may be more unexplored neighbors of v
                    visited[v]['i'] = j;
                    
                    stack.append(u); #but first we want to look at neighbors of u
                    visited[u] = {'L': l+1, 'R': False, 'i':0};
                    break;  #break out of while loop
                    
        (action, result) = fn(2,i,None,None,None,arg);
        return result if action == 0 else 1;
        
    #search for x starting from vertex s.  Can be used to compute distance from s to x;
    #returns -2 if either s or x are not vertices
    #returns -1 if vertex x can't be reached from s
    #returns non-negative distance otherwise
    def BFS(self, s, x):
    
        if not ((s in self.V) and (x in self.V)):
            return -2;
            
        if s == x:
            return 0;
            
        state = {'d':-1, 'v':x};
        return self.BFT(s, BFSCallback, state);
        
        
    # Breadth First Traversal, starting with node s.
    # arguments:
    #   s : root of Breadth First Traversal
    #   fn : callback function (see below)
    #   arg : arguments that will be passed to callback function
    #
    # To provide functionality to the traversal function, a callback is provided of the form
    # (action, result) = callback( op, i, v, l, s, arg )
    # arguments of callback():
    #   op: 0 for traversal_start: callback(0, None, None, None, None, arg);
    #       1 for traversal_item:  callback(1, i, v, l, s, arg);
    #           i: the ith item visited, starting with 0 (the root)
    #           v: the current item being visited
    #           l: the current traversal level (distance from the root)
    #           s: the starting node s
    #           arg: application defined arguments
    #       2 for traversal_end: callback(2, n, None, None, None, arg)
    #           n: total number of nodes visited
    #           arg: application defined arguments
    #
    #  callback must return:
    #  when op is 1 (traversal_item) or 2 (traversal_end), callback must return a tuple of the form
    #  (action, resultValue):   if action = 0, traversal will be stopped and resultValue will be
    #                           used as return value of BFT()
    #                           if action = 1, traversal will continue (resultValue is ignored, but must
    #                           be provided.
    #  when op is 0 (traversal_start) any value returned by callback will be ignored
    def BFT(self,s,fn,arg):
    
        if not s in self.V:
            return 0;
    
        visited = {};
        i = 0;
        q = array.array('i');
        
        fn(0, None, None, None, None, arg);   #callback(traversal_start, None, None, None, None, arg);

        q.append(s);
        visited[s] = {'P': [s], 'L': 0};
        
        while len(q) > 0:
        
            v = q.pop(0);
            p = visited[v]['P'];
            l = visited[v]['L'];
            (action, result) = fn(1, i, v, l, s, arg);
            i += 1;
            
            if action == 0:
                #abort traversal and return 'result'
                fn(2,i,None,None,None,arg);
                return result;
                
            # now look at v's neighbors
            N = self.NeighborsOf(v);
            
            for n in N:
                if not n in visited:
                    visited[n] = {'P': p, 'L':l+1};
                    visited[n]['P'].append(n);
                    q.append(n);
        
        (action, result) = fn(2, i, None, None, None, arg);
        return result if action == 0 else 1;
            
    def EdgeCount(self):
        return self.m;
        
    def VertexCount(self):
        return len(self.V);
        
    def OneVertex(self):
        return self.V.keys()[randint(0,len(self.V)-1)];
        
    def Load(self,fn):
        f = open(fn);
        i = 0;
        
        for oneLine in f:
            values = [int(x) for x in oneLine.split()];
            
            o = values[0];
            
            if not o in self.V: 
                self.V[o]={'OE':set(), 'IE':set()};
                self.E[o]=set();
                
            for d in values[1:]:
            
                if not d in self.V: 
                    self.V[d]={'OE':set(), 'IE':set()};
                    self.E[d]=set();
                
                if not (d in self.V[o]['OE']):
                    self.V[o]['OE'] |= set([d]);
                    self.V[d]['IE'] |= set([o]);
                    self.E[o] |= set([d]);
                    self.m += 1;
                
                i += 1;
            
        print "Read ", i, "lines: |V|=", self.VertexCount(), " |E|=", self.EdgeCount();
		