#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include "bitfield.h"

#define DEFNODESSIZE 512
#define DEFEDGESSIZE 512
#define DEFNEIGHBORS 8

struct structGraphEdge {
    int id;
    int idN1;
    int idN2;
    float w;
};

struct structGraphNode {
    int id;
    std::vector<int> *pvE;
};  

typedef struct structGraphEdge GraphEdge;
typedef struct structGraphNode GraphNode;

class GRAPH
{
    public:
    GRAPH();
    ~GRAPH();
    
    int Load(char *filename);
    int addNode(int idNode);
    int addEdge(int idSrc, int idDst, float w);
       
    private:
    BITFIELD *pB;
    GraphEdge *pE;
    GraphNode *pN;
    int sizeNodes;
    int sizeEdges;
    int nNodes;
    int nEdges;
    
    int fLoading;
    
    void resizeNodes(void);
    void resizeEdges(void);
    int findNode(int idNode);
    void recordEdge(int idNode, int idEdge);
};

#endif