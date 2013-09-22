#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

#include "debug.h"
#include "bitfield.h"
#include "unionfind.h"
#include "qsort.h"
#include "graph.h"

typedef struct structEdgeWeightIndex {
    float w;
    int id;
} EdgeWeightIndex;

static float getWeight(void *pA, int i)
{
    EdgeWeightIndex *pI = (EdgeWeightIndex *)pA;
    return (float) pI[i].w;
}

static void * resizeBlock(void *pOld, int oldsize, int newsize, int elementSize)
{
    void *pNew = (void *) malloc( elementSize * newsize );
    memset(pNew, 0, elementSize * newsize);
    if ((pOld != (void *)0) && (oldsize != 0))
    {
        memcpy(pNew, pOld, elementSize * oldsize);
        free(pOld);
    }
    return pNew;
}

static float getNodeIdFloat(void *pA, int i)
{
    GraphNode *pN = (GraphNode *)pA;
    return (float) pN[i].id;
}

GRAPH::GRAPH(void)
{
    pB = new BITFIELD(DEFNODESSIZE);
    
    sizeNodes = 0;
    sizeEdges = 0;
    nNodes = 0;
    nEdges = 0;
    
    resizeNodes();
    resizeEdges();
    
    fLoading = 0;
}

GRAPH::~GRAPH(void)
{
    delete(pB);
    
    if (pN)
    {
        for (int i = 0; i < nNodes; i++)
        {
            if (pN[i].pvE) delete(pN[i].pvE);
        }
        free(pN);
    }
    
    if (pE) free(pE);
}

int GRAPH::Load(char *filename)
{
    std::ifstream fs(filename);
    std::string s;
    int i;
    
    fLoading = 1;
    for (i = 0; getline(fs, s); i++)
    {
        int n, src, dst;
        float w;    
        std::istringstream ss(s);
        
        if (i == 0)
        {
            ss >> n;
        }
        else
        {
            ss >> src;
            ss >> dst;
            ss >> w;    
            // note, src, dst are 1-based, convert to 0-based before adding
            
            addEdge(--src, --dst, w);
        }
    }
    
    QSortX((void *) pN, sizeof(GraphNode), 0, nNodes, getNodeIdFloat );
    
    // record, for each node, its incident edges
    for (i = 0; i < nEdges; i++)
    {
        recordEdge( pE[i].idN1, pE[i].id );
        recordEdge( pE[i].idN2, pE[i].id );
    }
    
    fLoading = 0;
    return nEdges;
}

int GRAPH::EdgeCount(void)
{ return nEdges; }

int GRAPH::NodeCount(void)
{ return nNodes; }

int GRAPH::MDClustering(int nClusters)
{
    int i;
    UNIONFIND U;
    
    // build an index of edge weights (distances), then sort in increasing order
    EdgeWeightIndex *pIndex = (EdgeWeightIndex *) malloc(sizeof(EdgeWeightIndex) * nEdges);
    for (i = 0; i < nEdges; i++)
    {
        pIndex[i].id = pE[i].id;
        pIndex[i].w  = pE[i].w;
    }
    QSortX(pIndex, sizeof(EdgeWeightIndex), 0, nEdges, getWeight);

    // initialize a UnionFind structure on the nodes
    U.Init(nNodes);
    
    for (i = 0; i < nEdges; i++)
    {
        int iEdge = pIndex[i].id;
        int idN1 = pE[iEdge].idN1;
        int idN2 = pE[iEdge].idN2;
        
        if (U.Find(idN1) == U.Find(idN2))
        {
            // the endpoints of this edge already belong to the same cluster, ignore
            continue;
        }
        else
        {
            // merge the clusters to which each endpoint belongs
            
            U.Union(idN1, idN2);
        }
        
        if (U.ClusterCount() == nClusters)
        {
            // down to target number of clusters, done
            break;
        }
    }
    
    // NOTE: At end of loop, i is the index of the first unseen edge, or nEdges if
    // loop ran to completion
    
    // having found the components, record the data
    CD.nClusters = U.ClusterCount();
    
    for ( ; i < nEdges; i++ )
    {
        int iEdge = pIndex[i].id;
        int idN1 = pE[iEdge].idN1;
        int idN2 = pE[iEdge].idN2;
        
        if (U.Find(idN1) == U.Find(idN2))
        {
            // this edge is contained within a recognized cluster, discard
            continue;
        }
        
        // the weight (distance) of this edge marks the minimal distance
        // among disjoint clusters.
        CD.minW = pE[iEdge].w;
        break;
    }
    
    free(pIndex);
    return CD.nClusters;
            
}

float GRAPH::MDClusteringSpacing()
{   return CD.minW;  }


int GRAPH::addNode(int idNode)
{
    ASSERT(!pB->getBit(idNode), "GRAPH::addNode: node already added");
    
    if (nNodes >= sizeNodes)
        resizeNodes();
        
    pN[nNodes].id = idNode;
    pN[nNodes].pvE = new std::vector<int>(DEFNEIGHBORS);
    pB->setBit(idNode);
    nNodes++;
}

int GRAPH::addEdge(int idSrc, int idDst, float w)
{   
    if (!pB->getBit(idSrc))
        addNode(idSrc);
        
    if (!pB->getBit(idDst))
        addNode(idDst);
        
    if (nEdges >= sizeEdges)
        resizeEdges();
        
    pE[nEdges].id = nEdges;
    pE[nEdges].idN1 = ((idSrc < idDst) ? idSrc : idDst);
    pE[nEdges].idN2 = ((idSrc < idDst) ? idDst : idSrc);
    pE[nEdges].w = w;
    
    if (!fLoading)
    {
        // if reading from file (loading), this will be done in bulk at the end of load
        recordEdge(idSrc, nEdges);
        recordEdge(idDst, nEdges);
    }
    
    nEdges++;
}

void GRAPH::resizeNodes(void)
{
    int newsize = ((sizeNodes == 0) ? DEFNODESSIZE : sizeNodes * 2);
    pN = (GraphNode *) resizeBlock((void *)pN, sizeNodes, newsize, sizeof(GraphNode));
    sizeNodes = newsize;
}

void GRAPH::resizeEdges(void)
{
    int newsize = ((sizeEdges == 0) ? DEFEDGESSIZE : sizeEdges * 2);
    pE = (GraphEdge *) resizeBlock((void *)pE, sizeEdges, newsize, sizeof(GraphEdge));
    sizeEdges = newsize;
}

void GRAPH::recordEdge(int idNode, int idEdge)
{
    int k = findNode(idNode);
    ASSERT(k != -1, "GRAPH::recordEdge: node not found.");
    pN[k].pvE->push_back(idEdge);
}

int GRAPH::findNode(int idNode)
{
    if (!pB->getBit(idNode))
    {
        // idNode not in node list
        return -1;
    }
    
    // assume node array is dense (that is nNodes ~ maxNodeId)
    int k = ((idNode >= nNodes) ? (nNodes - 1) : idNode);
    
    while ( pN[k].id > idNode ) 
    {
        k--;
    }
    
    ASSERT( pN[k].id == idNode, "GRAPH::findNode: pN[k].id != idNode");
    return k;    
}
