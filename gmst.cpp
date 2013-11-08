#include "graph.h"
#include "gmst.h"
#include "iheap.hpp"
#include "unionfind.hpp"

// ----------------------------------------------------------------------------
// GraphMinimalSpanningTree::GraphMinimalSpanningTree(void)
//
// default constructor
// ----------------------------------------------------------------------------
GraphMinimalSpanningTree::GraphMinimalSpanningTree(void)
{
}

// ----------------------------------------------------------------------------
// GraphMinimalSpanningTree::~GraphMinimalSpanningTree(void)
//
// default destructor
// ----------------------------------------------------------------------------
GraphMinimalSpanningTree::~GraphMinimalSpanningTree(void)
{
}


// ----------------------------------------------------------------------------
// GraphMinimalSpanningTree::Prim(Graph &g)
//
// GraphMinimalSpanningTree::Prim(Graph &g, VertexID idSource);
//
// Computes a Minimal Spanning Tree for the given graph, using Prim's algorithm.
//
// An optional second parameter is the source vertex.  If none is given, a source
// vertex will be chosen at random.
//
// returns a pointer to a GMSTResults structure:
//      VertexID  idSrc:     ID of source vertex
//      EdgeIDSet E;         EdgeIDSet (std::vector<EdgeID>) of IDs of edges in MST
//      double    wTotal;    Total cost of MST
//
// after client code is done using the GMSTResults structure, it must be deallocated
// by calling delete
//
// If graph is not connected, then no attempt is made to compute a MST
// A more complete solution would be to return the set of connected components
//   but this implementation does not do that.
// ----------------------------------------------------------------------------
GMSTResults * GraphMinimalSpanningTree::Prim(Graph &g)
{   return Prim(g, g.AnyVertex());    }

GMSTResults * GraphMinimalSpanningTree::Prim(Graph &g, VertexID idSource)
{
    const int nVertices = g.V();
    const int NULLEDGE = -1;
    const int NOTVISITED = -2;
    
    std::vector<EdgeID> R(nVertices, (EdgeID) NOTVISITED);      // to record which edge leads us to each vertex
    IndexedHeap<double, EdgeID> H(nVertices);                   // for easy retrieval of least-cost edges
    
    GMSTResults *pR = new GMSTResults;                          // allocate object to return MST
    if (!pR) throw GMST_ERR_OUTOFMEMORY;
    
    pR->idSrc  = idSource;                                      // record starting vertex
    pR->wTotal = 0.0;                                           // start with 0 total cost
    
    if (!g.isConnected()) return pR;                            // if graph is not connected, don't attempt a solution
    
    // initialize heap to contain the single null edge from the source vertex to itself, with cost 0
    H.addItem(idSource, 0, NULLEDGE);
    
    while (!H.empty())
    {        
        // retrieve least-cost edge from the open set
        VertexID idVertex;
        EdgeID   idEdge;
        double   w;

        H.getMin(idVertex, w, idEdge);      // find least-cost edge (idEdge) to (idVertex) with cost (w)
        
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
            pR->E.push_back(idEdge);    // add id of this edge to MST
            pR->wTotal += w;            // maintain running total cost
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

// ----------------------------------------------------------------------------
// GraphMinimalSpanningTree::Kruskal(Graph &g)
//
// Computes a Minimal Spanning Tree for the given graph, using Kruskal's algorithm.
//
// returns a pointer to a GMSTResults structure:
//      VertexID  idSrc:     ID of source vertex // NOTE: set to zero, not used
//      EdgeIDSet E;         EdgeIDSet (std::vector<EdgeID>) of IDs of edges in MST
//      double    wTotal;    Total cost of MST
//
// after client code is done using the GMSTResults structure, it must be deallocated
// by calling delete
//
// If graph is not connected, then no attempt is made to compute a MST
// A more complete solution would be to return the set of connected components
//   but this implementation does not do that.
// ----------------------------------------------------------------------------
GMSTResults * GraphMinimalSpanningTree::Kruskal(Graph &g)
{
    const int nVertices = g.V();                    // number of vertices
    const int nEdges = g.E();                       // number of edges

    GMSTResults *pR = new GMSTResults;              // allocate memory for resulting MST
    if (!pR) throw GMST_ERR_OUTOFMEMORY;

    pR->wTotal = 0.0;                               // initialize to 0
    pR->idSrc  = 0;                                 // unused, there is no starting vertex for Kruskal's algorithm
    if (!g.isConnected()) return pR;                // if graph is not connected, don't attempt to compute a solution
    
    IndexedHeap<double> H(nVertices);               // heap for easy retrieval of least cost edges
    UnionFind U(nVertices);                         // union-find for easy cycle detection
    
    // initialize the heap to contain all edges in the graph
    for (int i = 0; i < nEdges; i++)
    {
        Edge e;        
        g.GetEdge(i, e);                            // retrieve this edge from graph
        H.addItem(i, e.w);                          // add its id and length to heap
    }
    
    // iterate until we find a complete MST or no more edges to explore
    while (!H.empty())
    {
        EdgeID idEdge;
        double w;
        Edge   e;
        
        H.getMin(idEdge, w);                        // retrieve least-cost edge
        g.GetEdge(idEdge, e);                       // retrieve edge from graph
        
        if (U.Find(e.idSrc) == U.Find(e.idDst))
        {
            // there is already a path connecting this edge's endpoints
            // the edge would create a cycle.  toss away.
            continue;
        }
        
        // add this edge to resulting MST
        pR->E.push_back(idEdge);
        pR->wTotal += w;
        
        // remember that endpoints now connected in MST
        U.Join(e.idSrc, e.idDst);
        
        if (U.Size(e.idSrc) == nVertices)
        {
            // terminate early if we have reached all possible vertices
            break;
        }
    }
    
    return pR;
}