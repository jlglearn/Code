#ifndef _GMST_H_
#define _GMST_H_

#include "graph.h"

typedef struct structGMSTResults {
    VertexID   idSrc;           // source vertex (0 for MST's computed by Kruskal's algorithm
    double     wTotal;          // total weight (cost) of MST
    EdgeIDSet  E;               // std::vector<EdgeID>: set of ids of edges that form MST    
} GMSTResults;

class GraphMinimalSpanningTree {

    public:
    GraphMinimalSpanningTree(void);
    ~GraphMinimalSpanningTree(void);
    GMSTResults *Prim(Graph &g);
    GMSTResults *Prim(Graph &g, VertexID idSource);
    GMSTResults *Kruskal(Graph &g);
};


typedef enum enumGMSTError {
    GMST_ERR_OUTOFMEMORY
    } GMSTError;


#endif

