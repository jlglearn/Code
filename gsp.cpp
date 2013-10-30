#include <vector>
#include <math.h>
#include "heap.h"
#include "graph.h"
#include "gsp.h"

class GSPDistanceDescriptor {
    public:
    VertexID idSrc;
    VertexID idDst;
    double   w;
    
    bool operator <  (const GSPDistanceDescriptor &);
    bool operator <= (const GSPDistanceDescriptor &);
    bool operator >  (const GSPDistanceDescriptor &);
    bool operator >= (const GSPDistanceDescriptor &);
};

inline bool GSPDistanceDescriptor::operator < (const GSPDistanceDescriptor & other)
{   return (w < other.w);   }
inline bool GSPDistanceDescriptor::operator <= (const GSPDistanceDescriptor & other)
{   return (w <= other.w);   }
inline bool GSPDistanceDescriptor::operator > (const GSPDistanceDescriptor & other)
{   return (w > other.w);   }
inline bool GSPDistanceDescriptor::operator >= (const GSPDistanceDescriptor & other)
{   return (w >= other.w);   }

GraphShortestPath::GraphShortestPath(void)
{
}

GraphShortestPath::~GraphShortestPath(void)
{
}

GSPResults *GraphShortestPath::Compute(VertexID idSrc, Graph &g)
{
    int nVertices = g.V();
    
    if ((idSrc < 0) || (idSrc > nVertices))
        throw GSP_ERR_INVALIDVERTEX;
        
    HEAP<GSPDistanceDescriptor> H;     
    GSPResults *pR = new GSPResults;
    
    GSPDistance gspd;
    gspd.idDst = -1;
    gspd.idPrevious = -1;
    gspd.distance = INFINITY;
    
    (*pR).idSrc = idSrc;
    (*pR).ds.resize(nVertices, gspd);           // initialize vector of distances to default -1 values
    
    GSPDistanceDescriptor dd;
    
    // initialize heap with the single distance from source to itself (distance = 0.0)
    dd.idSrc = idSrc;
    dd.idDst = idSrc;
    dd.w = 0.0;
    H.AddItem(dd);
    
    while (!H.empty())
    {
        dd = H.ExtractMin();
        
        VertexID idVertex = dd.idDst;
        VertexID idPrevious = dd.idSrc;
        double w = dd.w;
        
        if ((*pR).ds[idVertex].distance != INFINITY)
        {
            // have already processed this vertex, skip
            continue;
        }
        
        // record known shortest distance and predecessor for current vertex
        (*pR).ds[idVertex].idPrevious = idPrevious;
        (*pR).ds[idVertex].idDst = idVertex;   
        (*pR).ds[idVertex].distance = w;        
        
        // and explore its neighbors for further consideration
        NeighborSet *pN = g.Neighbors(idVertex);
        for (int iNeighbor = 0; iNeighbor < pN->size(); iNeighbor++)
        {
            dd.idSrc = idVertex;                    // idVertex is neighbor's predecessor
            dd.idDst = (*pN)[iNeighbor].idNeighbor;
            dd.w     = w + (*pN)[iNeighbor].w;      // distance from source is equal to distance to idSrc + d(idSrc, idNeighbor)
            
            H.AddItem(dd);                          // add to heap
        }
        delete pN;
    }
    
    return pR;
}
