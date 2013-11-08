#ifndef _GMST_H_
#define _GMST_H_

#include "debug.h"
#include "graph.h"

typedef struct structGMSTResults {
    EdgeIDSet  E;
    double     wTotal;          // total weight (cost) of MST
} GMSTResults;

class GraphMinimalSpanningTree {

    public:
    GraphMinimalSpanningTree(void);
    ~GraphMinimalSpanningTree(void);
    GMSTResults *Prim(Graph &g);
    GMSTResults *Kruskal(Graph &g);
};


typedef enum enumGMSTError {
    GMST_ERR_OUTOFMEMORY
    } GMSTError;


#endif

