#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <deque>

// define VertexID and EdgeID types, as well as vectors of them
typedef int VertexID;                           // VertexID
typedef int EdgeID;                             // EdgeID
typedef std::vector<VertexID> VertexIDSet;      // VertexIDSet
typedef std::vector<EdgeID>   EdgeIDSet;        // EdgeIDSet

// basic structure to hold Vertex information
class Vertex {	
	friend class Graph;                         // allow Graph class to access Vertex class fields
	
	public:
	Vertex(void);
	~Vertex(void);
	
	private:
	VertexID idVertex;                          // vertex id
	EdgeIDSet OE;                               // set of outgoing edges
	EdgeIDSet IE;                               // set of incoming edges
};

typedef std::vector<Vertex> VertexSet;          // vector of Vertex structures

// basic structure to hold Edge information
class Edge {
	friend class Graph;                         // allow Graph class to access Edge class fields
	
	public:
	EdgeID   idEdge;                            // edge id
	VertexID idSrc;                             // source vertex id
	VertexID idDst;                             // destination vertex id
	double   w;                                 // weight, distance, path length, etc
};

typedef std::vector<Edge> EdgeSet;              // vector of Edge structures

// structure to return information on adjacent vertices
class Neighbor {
	friend class Graph;                         // allow Graph class to access Neighbor class fields
	
	public:
    EdgeID   idEdge;                            // id of edge that connects to neighbor
	VertexID idNeighbor;                        // id of adjacent vertex
	double   w;                                 // distance to adjacent vertex
};

typedef std::vector<Neighbor> NeighborSet;      // vector of Neighbor structures


// useful internal constants used to specify connected state of the graph
typedef enum enumGraphConnected {
    GRAPH_CONNECTED_UNKNOWN,
    GRAPH_CONNECTED,
    GRAPH_UNCONNECTED
} GraphConnectedState;

// useful constants to define traversal types (only used internally, not by client code)
typedef enum enumGraphTraversalType {
	GRAPH_TRAVERSAL_BFT,
	GRAPH_TRAVERSAL_DFT
} GraphTraversalType;


// define callback actions, used during traversal of a graph
typedef enum enumGraphCallbackAction {
	GRAPH_CALLBACK_CONTINUE,                    // continue traversal
	GRAPH_CALLBACK_STOP                         // stop traversal
} GraphCallbackAction;


// define callback operations, used during traversal of a graph
typedef enum enumGraphCallbackOp {
	GRAPH_TRAVERSAL_START,                      // starting traversal
	GRAPH_TRAVERSAL_VISIT,                      // visiting a node
	GRAPH_TRAVERSAL_END                         // traversal complete
} GraphCallbackOp;

// ----------------------------------------------------------------------------
// Graph class
// ----------------------------------------------------------------------------
class Graph {
	public:
	Graph(void);
	~Graph(void);
	
	void Reset(int nVertices);                  // erase all graph information and state, and create an empty
                                                // graph with nVertices vertices
                                                
	void Load(char *filename);                  // read graph from file
    void Write(char *filename);                 // write graph to file
	void SetDirected(void);                     // mark graph as directed
    bool isDirected(void);                      // return whether graph is directed
    bool isConnected(void);                     // return whether graph is connected
	
	void AddEdge(VertexID idSrc, VertexID idDst);   // add an edge from idSrc to idDst, assumes w = 1.0
	void AddEdge(VertexID idSrc, VertexID idDst, double w);	    // add an edge from idSrc to idDst with given weight
    void GetEdge(EdgeID idEdge, Edge &e);           // retrieve requested edge
    
	VertexID AddVertex(void);	                    // add a vertex to the graph
    VertexID AnyVertex(void);                       // return id of a randomly selected vertex
    EdgeID   AnyEdge(void);                         // return id of a randomly selected edge
	NeighborSet *Neighbors(VertexID idVertex);      // return the set of vertices adjacent to vertex idVertex
		
	int V(void);                                // return number of vertices
	int E(void);                                // return number of edges
    double d(VertexID idSrc, VertexID idDst);   // return the distance of edge from idSrc to idDst
	
    // Breadth-First-Traversal of the graph
	int BFT(VertexID idVertex, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *) = 0, void *pArgs = 0);
    
    // Depth-First-Traversal of the graph
	int DFT(VertexID idVertex, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *) = 0, void *pArgs = 0);
	
    // Generate a random graph of N vertices, directed or undirected, with density p and random length
	void GenerateRandom(int N, bool directed, double p, double minLength, double maxLength);
	
	
	private:
	EdgeSet   ES;                               // pointer to set of edges
	VertexSet VS;                               // pointer to set of vertices
	bool fDirected;                             // set to true if graph is directed
    GraphConnectedState eConnected;             // connected state of the graph
	
	inline void CheckVertex(VertexID idVertex); // utility function to check whether idVertex is a valid vertex id.
    EdgeID findEdge(VertexID idSrc, VertexID idDst);  // returns the ID of the edge from idSrc to idDst
    
    // internal function to generate a graph traversal of the given type (BFT, DFT)
	int Traversal(GraphTraversalType gtt, VertexID idVertex, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs);
};


// define useful error codes
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