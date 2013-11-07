#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <deque>

typedef enum enumGraphTraversalType {
	GRAPH_TRAVERSAL_BFT,
	GRAPH_TRAVERSAL_DFT
} GraphTraversalType;

typedef enum enumGraphCallbackAction {
	GRAPH_CALLBACK_CONTINUE,
	GRAPH_CALLBACK_STOP
} GraphCallbackAction;

typedef enum enumGraphCallbackOp {
	GRAPH_TRAVERSAL_START,
	GRAPH_TRAVERSAL_VISIT,
	GRAPH_TRAVERSAL_END
} GraphCallbackOp;

typedef int VertexID;
typedef int EdgeID;
typedef std::vector<VertexID> VertexIDSet;
typedef std::vector<EdgeID>   EdgeIDSet;

class Vertex {	
	friend class Graph;
	
	public:
	Vertex(void);
	~Vertex(void);
	
	private:
	VertexID idVertex;
	EdgeIDSet OE;
	EdgeIDSet IE;
};

typedef std::vector<Vertex> VertexSet;

class Edge {
	friend class Graph;
	
	public:
	EdgeID   idEdge;
	VertexID idSrc;
	VertexID idDst;
	double   w;
};

typedef std::vector<Edge> EdgeSet;

class Neighbor {
	friend class Graph;
	
	public:
	VertexID idNeighbor;
	double   w;
};

typedef std::vector<Neighbor> NeighborSet;


class Graph {
	public:
	Graph(void);
	~Graph(void);
	
	void Reset(int nVertices);
	void Load(char *filename);
	void SetDirected(void);
    bool isDirected(void);
	
	void AddEdge(VertexID idSrc, VertexID idDst);
	void AddEdge(VertexID idSrc, VertexID idDst, double w);	
	VertexID AddVertex(void);	
	NeighborSet *Neighbors(VertexID idVertex);
    bool GetEdge(EdgeID idEdge, Edge &e);
		
	int V(void);
	int E(void);
	
	int BFT(VertexID idVertex, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *) = 0, void *pArgs = 0);
	int DFT(VertexID idVertex, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *) = 0, void *pArgs = 0);
	
	void GenerateRandom(int N, bool directed, double p, double minLength, double maxLength);
	
	
	private:
	EdgeSet   *pE;
	VertexSet *pV;
	bool fDirected;
	
	inline void CheckVertex(VertexID idVertex);
	int Traversal(GraphTraversalType gtt, VertexID idVertex, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs);
};


static const int _iGraphErrorBase = 0x333;

typedef enum enumGraphError {
	GRAPH_ERR_INVALIDSIZE = _iGraphErrorBase,
	GRAPH_ERR_OUTOFMEMORY,
	GRAPH_ERR_INDEXOUTOFRANGE,
	GRAPH_ERR_ILLEGALOPERATION,
	GRAPH_ERR_INVALIDTRAVERSALTYPE,
	GRAPH_ERR_BADGRAPHSIZE
} GraphError;




#endif