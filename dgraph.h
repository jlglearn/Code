#ifndef _DGRAPH_H_
#define _DGRAPH_H_

#define INFINITY 9999999

typedef struct structVDNode {
    int iDst;
    int w;
} VDNode;

typedef struct structBFAResult {
    int fValid;
    int nIterations;
    int iSrc;
    int nV;
    VDNode *pd;
} BFAResult;
    
typedef struct structDGVertex {
    int id;
    int *pIE;
    int *pOE;
    int nIE;
    int nOE;
    int nIESize;
    int nOESize;
} DGVertex;

typedef struct structDGEdge {
    int id;
    int iSrc;
    int iDst;
    int w;
} DGEdge;

class DGRAPH
{
    public:
    DGRAPH(void);
    ~DGRAPH(void);
    
    void Load(char *filename);
    void Write(char *filename);
    int VertexCount(void);
    int EdgeCount(void);
    BFAResult *BFA(int iSrc);
    
    private:
    int nV;
    int nE;
    int nVSize;
    int nESize;
    DGVertex *pV;
    DGEdge *pE;
    
    int fLoading;
    
    void AddVertex(int iVertex);
    void AddEdge(int iSrc, int iDst, int w);
    void RecordEdge(int iSrc, int iDst, int iEdge);

    
};

#endif