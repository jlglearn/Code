#ifndef _GSCC_H_
#define _GSCC_H_

class GSCCResults {
    public:
    std::vector<VertexIDSet> vSCC;
};

typedef struct structTSCCStateNode {
    int low;
    int index;
} TSCCStateNode;

typedef std::vector<TSCCStateNode> TSCCStateNodeSet;

class TSCCState {
    friend class GraphStronglyConnectedComponents;
    
    public:
    TSCCState(int nVertices);
    ~TSCCState(void);
    
    int index;
    
    TSCCStateNodeSet tss;
    
    private:
    int nVertices;
};

class GraphStronglyConnectedComponents {
    public:
    GraphStronglyConnectedComponents(void);
    ~GraphStronglyConnectedComponents(void);
    GSCCResults *Compute(Graph &g);
    
    private:    
    GSCCResults *TarjanSCC(Graph &g);    
};

#endif 