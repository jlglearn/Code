import array;
from random import randint;

def SCCCallback1(op,i,v,l,s,state):
    if op==1:
        if not v in state['v']:
            state['v'][v] = s;
    elif op==3:
        state['b'].append(v);
    return (1,None);
    
def SCCCallback2(op,i,v,l,s,state):
    if op==1:
        if not v in state['v']:
            state['v'][v]=s;
            iComponent = len(state['C'])-1;
            state['C'][iComponent]['n'] += 1;
    return (1,None);
    
class DGraph:

    def __init__(self):
        self.V = {};
        self.m = 0;
        
    def AddVertex(self, v):
        if not v in self.V:
            self.V[v] = {'I': set(), 'O': set()};
            
    def NeighborsOf(self,v):
        if not v in self.V:
            return None;
        return list(self.V[v]['O']);            
        
    def INeighborsOf(self,v):
        if not v in self.V:
            return None;
        return list(self.V[v]['I']);
        
    def OneVertex(self):
        return None if (len(self.V) == 0) else self.V.keys()[randint(0,len(self.V.keys())-1)];
        
    def DFTX(self,s,callback,arg,dir):
        if not s in self.V:
            return 0;
            
        visited = {};
        stack = array.array('i');
        i = 0;
        
        callback(0,None,None,None,None,arg);
        
        stack.append(s);
        visited[s] = {'L': 0, 'i': 0, 'R': False };
        
        while len(stack) > 0:
        
            v = stack.pop();
            l = visited[v]['L'];
            
            if not visited[v]['R']:
                visited[v]['R'] = True;
                callback(1,i,v,l,s,arg);
                i += 1;
                
            N = self.NeighborsOf(v) if (dir == 0) else self.INeighborsOf(v);
            j = visited[v]['i'];
            backtrack = True;
            
            while j < len(N):
            
                u = N[j];
                j += 1;
                
                if not u in visited:
                
                    stack.append(v); #there may be more neighbors of v
                    visited[v]['i'] = j;
                    
                    stack.append(u); #but we first want to look at u's neighbors
                    visited[u] = {'L':l+1, 'i':0, 'R':False};
                    
                    backtrack = False;
                    break; #break out of inner while loop
                    
            visited[v]['i'] = j;
            
            if backtrack:
                callback(3,None,v,l,s,arg);
                        
                
        callback(2,i,None,None,None,arg);
        return 1;
        
        
    def DFT(self, s, callback, arg):
        return self.DFTX(s,callback,arg,0);
        
    def RDFT(self, s, callback, arg):
        return self.DFTX(s,callback,arg,1);
        
    def SCC(self):
    
        state = {'v':{}, 'b':[]};
        i = 0;
        for s in self.V.keys():
            if not s in state['v']:
                self.RDFT(s,SCCCallback1,state);
                i += 1;
                
        del state['v'];
        
        state['b'].reverse();
        state['v'] = {};
        state['C'] = [];
        
        for s in state['b']:
            if not s in state['v']:
                state['C'].append({'s':s, 'n':0});
                self.DFT(s,SCCCallback2,state);
        
        del state['v'];
        
        f = {};
        for i in range(len(state['C'])):
            n = state['C'][i]['n'];
            if not n in f:
                f[n] = 0;
            f[n] += 1;

        L = f.keys();
        L.sort();
        L.reverse();
        
        print 'Frequency histogram of component sizes:';
        print 'Size   \tCount';
        for i in range(len(L)):
            print '{}\t{}'.format(L[i], f[L[i]]);
        
        
            
            
        
    def Load(self, fn):
        with open(fn) as f:
        
            for oneLine in f:
                values = [int(x) for x in oneLine.split()];
                
                o = values[0];
                
                self.AddVertex(o);
                
                for d in values[1:]:
                
                    if not d in self.V:
                        self.AddVertex(d);
                        
                    if not d in self.V[o]['O']:
                        self.V[o]['O'] |= set([d]);
                        self.V[d]['I'] |= set([o]);
                        self.m += 1;
                        
        for v in self.V.keys():
            self.V[v]['O'] = list(self.V[v]['O']);
            self.V[v]['O'].sort();
            self.V[v]['I'] = list(self.V[v]['I']);
            self.V[v]['I'].sort();
                        
        print 'Loaded [' + fn + '], |E|=[{}], |V|=[{}]'.format(self.m,len(self.V));
        
                        
                    