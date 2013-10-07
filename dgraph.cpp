#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "debug.h"
#include "bitfield.h"
#include "dgraph.h"

#define MINVSIZE 256
#define MINESIZE 256
#define MINVESIZE 8

void *resizeBlock(int blocksize, void *pOld, int oldsize, int newsize)
{
    void *pnew = (void *) malloc(blocksize * newsize);
    if (!pnew) throw new std::bad_alloc();
    memset(pnew, 0, blocksize * newsize);
    if (oldsize) memcpy(pnew, pOld, blocksize * oldsize);
    if (pOld) free(pOld);
    return pnew;
}

void *allocMatrix(int blocksize, int rows, int cols)
{
    int bytesize = blocksize * rows * cols;
    void *p = (void *) malloc(bytesize);
    if (!p) throw new std::bad_alloc();
    memset((void *)p, 0, bytesize);
    return p;
}

BFAResult *allocBFAResult(int nV)
{
    int bytesize = sizeof(BFAResult) + nV * sizeof(VDNode);
    unsigned char *p = (unsigned char *) malloc(bytesize);
    if (!p) throw new std::bad_alloc();
    memset((void *)p, 0, bytesize);
    
    unsigned char *q = &p[sizeof(BFAResult)];
    BFAResult *pr = (BFAResult *)p;
    if (nV) pr->pd = (VDNode *)q;
    return pr;
}


DGRAPH::DGRAPH(void)
{
    pV = (DGVertex *)0;
    pE = (DGEdge *)0;
    nV = 0;
    nE = 0;
    nVSize = 0;
    nESize = 0;
    fLoading = 0;
}

DGRAPH::~DGRAPH(void)
{
    if (pV)
    {
        for (int i = 0; i < nV; i++)
        {
            if (pV[i].pIE) free(pV[i].pIE);
            if (pV[i].pOE) free(pV[i].pOE);
        }
        
        free(pV);
    }
    
    if (pE) free(pE);
}

int DGRAPH::VertexCount(void)
{   return nV;  }

int DGRAPH::EdgeCount(void)
{   return nE;  }

void DGRAPH::AddVertex(int iVertex)
{
    if (iVertex >= nVSize)
    {
        int minsize;
        for (minsize = MINVSIZE; minsize <= iVertex; minsize *= 2);
        pV = (DGVertex *) resizeBlock(sizeof(DGVertex), pV, nVSize, minsize);
        nVSize = minsize;
    }
    
    pV[iVertex].id = iVertex;
    if (iVertex >= nV) nV = iVertex+1;
}

void DGRAPH::AddEdge(int iSrc, int iDst, int w)
{
    if (nE >= nESize)
    {
        int minsize;
        for (minsize = MINESIZE; minsize < nE+1; minsize *= 2);
        pE = (DGEdge *) resizeBlock(sizeof(DGEdge), pE, nESize, minsize);
        nESize = minsize;
    }
    
    pE[nE].id = nE;
    pE[nE].iSrc = iSrc;
    pE[nE].iDst = iDst;
    pE[nE].w = w;
    
    if (!fLoading) RecordEdge(iSrc, iDst, nE);
    
    nE++;
}

void DGRAPH::RecordEdge(int iSrc, int iDst, int iEdge)
{
    // record this outgoing edge at source vertex
    if (pV[iSrc].nOE >= pV[iSrc].nOESize)
    {
        int newsize = ((pV[iSrc].nOESize == 0) ? MINVESIZE : MINVESIZE + pV[iSrc].nOESize);
        pV[iSrc].pOE = (int *) resizeBlock(sizeof(int), pV[iSrc].pOE, pV[iSrc].nOESize, newsize);
        pV[iSrc].nOESize = newsize;
    }
    pV[iSrc].pOE[ pV[iSrc].nOE ] = iEdge;
    pV[iSrc].nOE++;
    
    // record this incoming edge at destination vertex
    if (pV[iDst].nIE >= pV[iDst].nIESize)
    {
        int newsize = ((pV[iDst].nIESize == 0) ? MINVESIZE : MINVESIZE + pV[iDst].nIESize);
        pV[iDst].pIE = (int *) resizeBlock(sizeof(int), pV[iDst].pIE, pV[iDst].nIESize, newsize);
        pV[iDst].nIESize = newsize;
    }
    pV[iDst].pIE[ pV[iDst].nIE ] = iEdge;
    pV[iDst].nIE++;
}

void DGRAPH::Write(char *filename)
{
    std::ofstream f(filename);
    f << "" << (nV-1) << " " << nE << "\n";
    for (int i = 0; i < nE; i++)
    {
        f << pE[i].iSrc << " " << pE[i].iDst << " " << pE[i].w << "\n";
    }
    f.close();
}

void DGRAPH::Load(char *filename)
{
    std::ifstream f(filename);
    std::string s;
    
    int nVertices = 0;
    int nEdges = 0;
    
    BITFIELD *pb;
    
    fLoading = 1;
    for (int i = 0; getline(f, s); i++)
    {
        std::istringstream ss(s);
        if (i > 0)
        {
            int iSrc;
            int iDst;
            int w;
            ss >> iSrc;
            ss >> iDst;
            ss >> w;
            
            if (!pb->getBit(iSrc))
            {
                // haven't seen this vertex, add it
                AddVertex(iSrc);
                pb->setBit(iSrc);
            }
            
            if (!pb->getBit(iDst))
            {
                // haven't seen this vertex, add it
                AddVertex(iDst);
                pb->setBit(iDst);
            }
            
            AddEdge(iSrc, iDst, w);
        }
        else
        {
            ss >> nVertices;
            ss >> nEdges;
            pb = new BITFIELD(nVertices+1);
        }
    }
    fLoading = 0;
    
#ifdef DEBUG_ON
    // verify, all entries in pV list must have index == id
    int cErrors = 0;
    for (int i = 0; i < nV; i++)
        if (i != pV[i].id)
        {
            std::cerr << "Error in pV (iVertex != pV[iVertex].id) at index [" << i << "], id=[" << pV[i].id << "]\n";
            cErrors++;
        }
    std::cout << "Loaded graph, verified vertices (" << cErrors << ") errors\n";
#endif    

    for (int i = 0; i < nE; i++)
        RecordEdge(pE[i].iSrc, pE[i].iDst, pE[i].id);
    
    if (pb) delete pb;
}

#define BFAELEMENT(i,v) A[((i)*nV)+(v)]
BFAResult *DGRAPH::BFA(int iSrc)
{
    int *A = (int *) allocMatrix(sizeof(int), nV+1, nV);
    
    for (int v = 0; v < nV; v++)
        BFAELEMENT(0,v) = ((v == iSrc) ? 0 : INFINITY);
        
    int fStop = 0, maxIteration = 0;
    
    for (int i = 1; (!fStop) && (i <= nV); i++)
    {
        // assume no further updates
        fStop = 1;
        
        for (int iVertex = 0; iVertex < nV; iVertex++)
        {
            int minW = BFAELEMENT(i-1, iVertex);
            
            for (int j = 0, m = pV[iVertex].nIE; j < m; j++)
            {
                int iEdge = pV[iVertex].pIE[j];
                int w = pE[iEdge].w + BFAELEMENT(i-1, pE[iEdge].iSrc);                
                if (w < minW)
                    minW = w;
            }
            
            BFAELEMENT(i,iVertex) = minW;
            
            if (BFAELEMENT(i,iVertex) != BFAELEMENT(i-1,iVertex))
            {
                fStop = 0;
                maxIteration = i;
            }
        }
    }
    
    BFAResult *bfa = allocBFAResult(fStop ? nV : 0);
    bfa->iSrc = iSrc;
    bfa->nIterations = maxIteration+1;
    
    if (!fStop)
    {
        // if we did not finish early then there must be a negative-cost cycle
        bfa->fValid = 0;
    }
    else
    {
        bfa->fValid = 1;
        bfa->nV = nV;
        for (int v = 0; v < nV; v++)
        {
            bfa->pd[v].iDst = v;
            bfa->pd[v].w = BFAELEMENT(maxIteration, v);
        }
    }
    
    free(A);
    return bfa;
}

