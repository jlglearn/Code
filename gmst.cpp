#include "debug.h"
#include "graph.h"
#include "gmst.h"
#include "iheap.hpp"

GraphMinimalSpanningTree::GraphMinimalSpanningTree(void)
{
}

GraphMinimalSpanningTree::~GraphMinimalSpanningTree(void)
{
}

GMSTResults * GraphMinimalSpanningTree::Prim(Graph &g)
{
    const int nVertices = g.V();
    const int NULLEDGE = -1;
    const int NOTVISITED = -2;
    
    std::vector<EdgeID> R(nVertices, (EdgeID) NOTVISITED);      // to record which edge leads us to each vertex
    IndexedHeap<double, EdgeID> H(nVertices);                   // for easy retrieval of least-cost edges
    
    VertexID idSource = g.AnyVertex();                          // get an arbitrary starting vertex
    
    GMSTResults *pR = new GMSTResults;                          // allocate object to return MST
    if (!pR) throw GMST_ERR_OUTOFMEMORY;
    
    // initialize heap to contain the single null edge from the source vertex to itself, with cost 0
    H.addItem(idSource, 0, NULLEDGE);
    
    while (!H.empty())
    {        
        // retrieve least-cost edge from the open set
        VertexID idVertex;
        EdgeID   idEdge;
        double   w;

        H.getMin(idVertex, w, idEdge);
        
        // skip if already seen
        if (R[idVertex] != NOTVISITED)
        {
            continue;
        }
        
        // mark as seen
        R[idVertex] = idEdge;
        
        // unless the special case for starting vertex, add edge to MST
        if (idEdge != NULLEDGE) 
        {
            pR->E.push_back(idEdge);
        }
        
        // terminate early if we have reached all possible vertices
        if (pR->E.size() == nVertices) break;
        
        // otherwise explore vertices we can reach from current vertex
        NeighborSet *pN = g.Neighbors(idVertex);
        
        for (int iNeighbor = 0; iNeighbor < pN->size(); iNeighbor++)
        {
            VertexID idNeighbor = (*pN)[iNeighbor].idNeighbor;
            EdgeID   idEdge     = (*pN)[iNeighbor].idEdge;
            double   w          = (*pN)[iNeighbor].w;
            
            // skip if already seen
            if (R[idNeighbor] != NOTVISITED)
            {
                continue;
            }
            
            double wPrev;
            
            // if not already in the heap, or if this edge has less cost than previous, add
            if ((!H.find(idNeighbor, wPrev)) || (w < wPrev))
                H.addItem(idNeighbor, w, idEdge);
                
        }
        delete pN;
        
    }
    
    return pR;
}