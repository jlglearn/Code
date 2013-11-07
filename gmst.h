#ifndef _GMST_H_
#define _GMST_H_

#include "debug.h"
#include "graph.h"

typedef struct structGMSTResults {
    EdgeIDSet  E;
} GMSTResults;

class GraphMinimalSpanningTree {

    public:
    GraphMinimalSpanningTree(void);
    ~GraphMinimalSpanningTree(void);
    GMSTResults *Prim(Graph &g);
};


typedef enum enumGMSTError {
    GMST_ERR_OUTOFMEMORY
    } GMSTError;


#endif

