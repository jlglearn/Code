#include "graph.h"
#include "gscc.h"

// Implemens Strongly Connected Components algorithms
// References:
//      For Tarjan algorithm: Wikipedia

template <class T>
static inline bool minVal(T v1, T v2)
{   return ((v1 < v2) ? v1 : v2);   }

bool InStack(VertexIDSet &S, VertexID idVertex)
{
    for (int i = 0; i < S.size(); i++)
    {
        if (S[i] == idVertex)
            return true;
    }
    return false;
}

static const int UNDEFINED = -1;

static void TSCCExplore(Graph &g, TSCCState *pS, std::vector<VertexID> &S, VertexID idVertex, GSCCResults *pR)
{
    // Set the depth index for v to the smallest unused index
    pS->tss[idVertex].index = pS->index;
    pS->tss[idVertex].low   = pS->index;
    pS->index++;
    S.push_back(idVertex);
    
    // for each neighbor of idVertex:
    NeighborSet *pN = g.Neighbors(idVertex);
    for (int iNeighbor = 0; iNeighbor < pN->size(); iNeighbor++)
    {
        VertexID idNeighbor = (*pN)[iNeighbor].idNeighbor;
        
        if (pS->tss[idNeighbor].index == UNDEFINED)
        {
            // if this neighbor hasn't been visited, recurse on it
            TSCCExplore(g, pS, S, idNeighbor, pR);
            pS->tss[idVertex].low = minVal(pS->tss[idVertex].low, pS->tss[idNeighbor].low);
        }
        else if (InStack(S, idNeighbor))
        {
            // idNeighbor is in stack, therefore in current SCC
            pS->tss[idVertex].low = minVal(pS->tss[idVertex].low, pS->tss[idNeighbor].index);
        }
    }
    delete pN;
    
    // If idVertex is a root node, pop the stack and generate an SCC
    if (pS->tss[idVertex].low == pS->tss[idVertex].index)
    {
        // start a new SCC
        VertexIDSet scc;
        VertexID idMember;
        
        do {
            // peel back stack until we reach the root (idVertex)
            idMember = S.back();
            S.pop_back();            
            // add idMember to this new component
            scc.push_back(idMember);
        } while (idMember != idVertex);
        
        pR->vSCC.push_back(scc);
    }    
}

TSCCState::TSCCState(int nVertices):nVertices(nVertices)
{
    TSCCStateNode ts_sample;
    ts_sample.index = UNDEFINED;
    ts_sample.low   = UNDEFINED;
    
    index = 0;                          // index := 0
    tss.resize(nVertices, ts_sample);   // for each vertex, initialize v.index := UNDEFINED
}

TSCCState::~TSCCState(void)
{}

GraphStronglyConnectedComponents::GraphStronglyConnectedComponents(void)
{}

GraphStronglyConnectedComponents::~GraphStronglyConnectedComponents(void)
{}

GSCCResults *GraphStronglyConnectedComponents::Compute(Graph &g)
{   
    if (!g.isDirected())
        return UndirectedSCC(g);
    else
        return TarjanSCC(g); 
}

static GraphCallbackAction fnStoreSCC(GraphCallbackOp op, int i, int iLevel, VertexID idSrc, VertexID idDst, void *pArgs)
{
    if (op == GRAPH_TRAVERSAL_VISIT)
    {
        std::vector<VertexID> *p = (std::vector<VertexID> *)pArgs;
        (*p)[idDst] = idSrc;
    }
    return GRAPH_CALLBACK_CONTINUE;
}

GSCCResults *GraphStronglyConnectedComponents::UndirectedSCC(Graph &g)
{
    // for undirected graphs computing SCC is easy through a simple BFT
    
    // a simple vector to record which component a given vertex belongs to
    std::vector<VertexID> *pvC = new std::vector<VertexID>(g.V(), UNDEFINED);
    
    // a mapping from root of scc to index of scc
    std::vector<int> *pidC = new std::vector<int>(g.V(), UNDEFINED);
    
    int iSCC = 0;
    for (VertexID idSrc = 0; idSrc < g.V(); idSrc++)
    {
        if ((*pvC)[idSrc] != UNDEFINED) continue; 
        
        // start new component
        (*pidC)[idSrc] = iSCC++;                // remember index of this component
        g.BFT(idSrc, fnStoreSCC, (void *)pvC);  // compute it
    }
    
    GSCCResults *pR = new GSCCResults;
    pR->vSCC.resize(iSCC);                      // create vectors for each component
    
    for (VertexID idVertex = 0; idVertex < g.V(); idVertex++)
    {
        VertexID idComponent = (*pvC)[idVertex];
        int iComponent = (*pidC)[idComponent];
        pR->vSCC[iComponent].push_back(idVertex);
    }
    
    delete pvC;
    delete pidC;
    return pR;    
}

GSCCResults *GraphStronglyConnectedComponents::TarjanSCC(Graph &g)
{
    int nVertices = g.V();
    GSCCResults *pR = new GSCCResults;
    TSCCState   *pS = new TSCCState(nVertices);
    
    std::vector<VertexID> S;                    // stack, initially empty
    
    for (int idVertex = 0; idVertex < nVertices; idVertex++)
    {
        if ((*pS).tss[idVertex].index == UNDEFINED)
        {
            TSCCExplore(g, pS, S, idVertex, pR);
        }
    }
    
    
    delete pS; 
    return pR;
}


