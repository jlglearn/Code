#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "graph.h"

// utility function to return a random number
static inline double getRandom(void) { return (((double) rand()) / RAND_MAX); }

// default Vertex constructor, assign an invalid vertex ID
Vertex::Vertex(void)
{	idVertex = -1;  }

// default Vertex destructor, no need to do anything special
Vertex::~Vertex(void)
{
}

// default Graph constructor, allocates memory for set of vertices and set of edges
// assumes graph is undirected.
Graph::Graph(void)
{
	pV = new VertexSet;
	if (!pV) throw GRAPH_ERR_OUTOFMEMORY;
	
	pE = new EdgeSet;
	if (!pE) throw GRAPH_ERR_OUTOFMEMORY;
	
	fDirected = false;
}

// default Graph destructor, deallocate memory used for set of vertices and set of edges
Graph::~Graph(void)
{
	if (pE) delete pE;
	if (pV) delete pV;
}

// clear all graph information and internal states.  Create an empty graph of nVertices vertices
void Graph::Reset(int nVertices)
{
	if (pV->size() > 0) pV->clear();
	if (pE->size() > 0) pE->clear();
	
	if (nVertices > 0) 
	{
		pV->resize(nVertices);
		
		if (pV->size() != nVertices)
			throw GRAPH_ERR_OUTOFMEMORY;
			
        // vertices are created with initial invalid ID == -1, assign proper and valid IDs
        // IDs are the 0-based indices into the vector of vertices.
		for (int i = 0; i < nVertices; i++)
			(*pV)[i].idVertex = (VertexID) i;
	}
	
	fDirected = false;          // assume graph is undirected
}

// return the number of vertices in the graph
int Graph::V(void)
{	return pV->size();	}

// return the number of edges in the graph
int Graph::E(void)
{	return pE->size();	}

// set the graph to be directed.  A graph can be marked as directed only if no edges have been added.
void Graph::SetDirected(void)
{	
    // if graph has any edges, then it's too late to mark it as directed
	if (pE->size() > 0) throw GRAPH_ERR_ILLEGALOPERATION;
    
    // mark as directed
	fDirected = true;	
}


// add an unconnected vertex to the graph
// returns the ID assigned to the newly created vertex
VertexID Graph::AddVertex(void)
{
	VertexID idVertex = (VertexID) pV->size();      // obtain the next ID in the sequence
	Vertex v;                                       // create an empty vertex structure
	v.idVertex = idVertex;                          // set id
	pV->push_back(v);                               // add to set
	return idVertex;                                // return ID of newly created vertex
}

// returns a randomly selected vertex (id)
VertexID Graph::AnyVertex(void)
{   return (VertexID) (pV->size() * getRandom());   }

// returns a randomly selected edge (id)
EdgeID Graph::AnyEdge(void)
{   return (EdgeID) (pE->size() * getRandom()); }

// add an edge from vertex idSrc to vertex idDst.  Assumes a path length of 1.0
void Graph::AddEdge(VertexID idSrc, VertexID idDst)
{	AddEdge(idSrc, idDst, 1.0);	}

// add an edge from vertex idSrc to vertex idDst of given length, cost, distance, etc)
void Graph::AddEdge(VertexID idSrc, VertexID idDst, double w)
{
    // make sure idSrc and idDst are valid vertex ids
	CheckVertex(idSrc);
	CheckVertex(idDst);
	
    // create edge
	Edge e;
    e.idEdge = pE->size();                          // obtain next edge id
	e.idSrc  = idSrc;                               // record source vertex id
	e.idDst  = idDst;                               // record destination vertex id
	e.w      = w;                                   // record length (cost, weigth, etc)
	pE->push_back(e);                               // add to set
	
	(*pV)[idSrc].OE.push_back(e.idEdge);            // record as an outgoing edge at source vertex
	(*pV)[idDst].IE.push_back(e.idEdge);            // record as an incoming edge at destination vertex
	
	if (!fDirected)
	{
        // if graph is undirected, also record in opposite direction
		(*pV)[idSrc].IE.push_back(e.idEdge);        // record as an ougoing edge at destination vertex
		(*pV)[idDst].OE.push_back(e.idEdge);        // record as an incoming edge at source vertex
	}

}

// return the requested edge
void Graph::GetEdge(EdgeID idEdge, Edge &e)
{
    if ((idEdge < 0) || (idEdge >= pE->size()))
        throw GRAPH_ERR_INDEXOUTOFRANGE;
        
    e.idEdge = (*pE)[idEdge].idEdge;
    e.idSrc  = (*pE)[idEdge].idSrc;
    e.idDst  = (*pE)[idEdge].idDst;
    e.w      = (*pE)[idEdge].w;
}

// read a graph from the given filename.  The file is assumed to have the following structure:
// FIRST LINE: contains the number of vertices in the graph (integer)
// ALL OTHER LINES:  idSrc idDst w  -- where idSrc and idDst are integer vertex ids and w the edge length
void Graph::Load(char *filename)
{
	std::ifstream f(filename);
	std::string s;
	int nVertices;
	
	for (int i = 0; getline(f, s); i++)
	{
		std::istringstream ss(s);
		
		if (i == 0)
		{
            // if this is the first line, read in number of vertices
			ss >> nVertices;
            
            // reset graph to an empty graph of nVertices vertices
			if (nVertices >= 0) Reset(nVertices);
		}
		else
		{
			VertexID idSrc;
			VertexID idDst;
			double w;
			
			ss >> idSrc;                // read in source vertex ID
			ss >> idDst;                // read in destination vertex ID
            ss >> w;                    // read in edge length
			
			AddEdge(idSrc, idDst, w);   // add edge to graph
		}
	}
}

// writes graph to a file with the given filename.  It writes the file in the same format
// as expected in Load() above
void Graph::Write(char *filename)
{
    std::ofstream f(filename);
    
    f << pV->size() << "\n";
    for (int iEdge = 0; iEdge < pE->size(); iEdge++)
    {
        f << (*pE)[iEdge].idSrc << " "
          << (*pE)[iEdge].idDst << " "
          << (*pE)[iEdge].w << "\n";
    }
    f.close();
}

// return set of vertices adjacent (directly connected to) idVertex
NeighborSet *Graph::Neighbors(VertexID idVertex)
{
	CheckVertex(idVertex);              // make sure idVertex is a valid vertex id
    
    // get the number of adjacent vertices
	int nNeighbors = (*pV)[idVertex].OE.size();
	
    // allocate memory for the set of adjacent vertices
	NeighborSet *pN = new NeighborSet(nNeighbors);
	
	for (int iNeighbor = 0; iNeighbor < nNeighbors; iNeighbor++)
	{
		EdgeID idEdge = (*pV)[idVertex].OE[iNeighbor];  // get the id of adjacent outgoing edge
		VertexID idNeighbor = (*pE)[idEdge].idDst;      // get the id of destination vertex
		
        // if graph is undirected, only one edge is stored to represent both directions,
        // it is possible that we need to consider the reverse direction to get the 
        // correct destination vertex id		
        if ((!fDirected) && (idNeighbor == idVertex))
			idNeighbor = (*pE)[idEdge].idSrc;
			
		double w = (*pE)[idEdge].w;                     // get the edge length
		
        (*pN)[iNeighbor].idEdge = idEdge;               // record edge id
		(*pN)[iNeighbor].idNeighbor = idNeighbor;       // record id of adjacent vertex
		(*pN)[iNeighbor].w = w;                         // and the distance to it
	}
	
	return pN;
}

// determine whether the graph is directed (true if directed, false otherwise)
bool Graph::isDirected(void)
{   return fDirected;   }


// do a Breadth-First-Traversal of the graph, starting at given source vertex (idSrc)
//
// parameters:
//
//      VertexID idSrc      -- id of source vertex (where traveral will start)
//      callback function   -- optional callback function, see details below
//      void *pArgs         -- optional callback parameter, typically a pointer to client-defined object
//
// client code can provide a callback function that will be called at different stages of the traversal indicated
// by the value of the GraphTraversalOp parameter:
//      GRAPH_TRAVERSAL_START:    traversal about to start (before visiting the first vertex)
//      GRAPH_TRAVERSAL_VISIT:    currently visiting the given vertex
//      GRAPH_TRAVERSAL_END:      traversal complete (after visiting last vertex)
//
// the parameters of the callback function are:
//      GraphCallbackOp:    described above
//      int i:              current iteration, the semantics depends on the value of the GraphCallbackOp parameter:
//              op == GRAPH_TRAVERSAL_START: i is 0, has no special meaning
//              op == GRAPH_TRAVERSAL_VISIT: i is the ith visited vertex
//              op == GRAPH_TRAVERSAL_END:   i is the total number of vertices visited during traversal
//      int iLevel:         current traversal level (hops removed from source vertex), only used if
//                          op == GRAPH_TRAVERSAL_VISIT, and is 0 otherwise
//      VertexID idSrc:     ID of source vertex (where traversal started)
//      VertexID idVertex:  ID of current vertex, only used if op == GRAPH_TRAVERSAL_VISIT
//      void *pArgs:        A pointer to an arbitrary object, provided by the client.
int Graph::BFT(VertexID idSrc, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs)
{	return Traversal(GRAPH_TRAVERSAL_BFT, idSrc, fn, pArgs);	}



// do a Depth-First-Traversal of the graph, starting at given source vertex.
// for parameter semantics, see description of BFT(), above
int Graph::DFT(VertexID idSrc, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs)
{	return Traversal(GRAPH_TRAVERSAL_DFT, idSrc, fn, pArgs);	}



// generate a random graph of N vertices, directed/undirected, with density p, and random edge length within specified range
void Graph::GenerateRandom(int N, bool directed, double p, double minLength, double maxLength)
{
	if (N < 0) throw GRAPH_ERR_BADGRAPHSIZE;
	
	if (minLength < 0) minLength = 0;
	if (maxLength < minLength) maxLength = minLength;
	
	Reset(N);                               // clear all graph information and reset to empty graph of N vertices
	
	if (directed) SetDirected();            // record whether graph is directed
	
    // for all possible edges, add (or not) to graph with probability = p (density)
	for (int i = 0; i < N - 1; i++)         // loop over all possible source vertices
	{
		for (int j = i + 1; j < N; j++)     // loop over all corresponding possible destination vertices
		{
			if (getRandom() < p)            // with probability p
			{
                // add edge from source vertex i, to destination vertex j, with random length within spacified range
				AddEdge(i, j, minLength + ((maxLength - minLength) * getRandom()));
			}
		}
	}
}


// internal functions:

// make sure a given id is a valid vertex id
inline void Graph::CheckVertex(VertexID idVertex)
{
	if ((idVertex < 0) || (idVertex >= V()))
		throw GRAPH_ERR_INDEXOUTOFRANGE;
}

// do a specified graph traversal
//
// parameters:
//      GraphTraversalType: can be one of:
//              GRAPH_TRAVERSAL_BFT:    for breadth-first traversal
//              GRAPH_TRAVERSAL_DFT:    for depth-first traversal
//
//      VertexID idSrc: id of source vertex (where traversal will start)
//
//      Callback funtion (optional): for details see BFT() above
//
//      a pointer to an arbitrary client-defined object (optional), will be supplied to each invocation of callback function.
int Graph::Traversal(GraphTraversalType gtt, VertexID idSrc, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs)
{

    // make sure traversal type is a known type
	if ((gtt != GRAPH_TRAVERSAL_BFT) && (gtt != GRAPH_TRAVERSAL_DFT))
		throw GRAPH_ERR_INVALIDTRAVERSALTYPE;
		
    // make sure source vertex is valid
	CheckVertex(idSrc);
	
    // initilize utility variables
	GraphCallbackAction action = GRAPH_CALLBACK_CONTINUE;
	std::vector<int> vLevel(V(), -1);
	std::deque<VertexID> Q;
	
    // signal traversal about to start
	if (fn) action = fn(GRAPH_TRAVERSAL_START, 0, 0, idSrc, 0, pArgs);
	
    // check whether client wants to abort traversal
	if (action == GRAPH_CALLBACK_STOP)
		return 0;
		
    // initialize level (hops removed from source vertex) of source vertex to 0
	vLevel[idSrc] = 0;
    
    // initialize queue/stack to contain only the source vertex
	Q.push_back(idSrc);
	
	int iLevel = 0;         // current depth (hops removed from source vertex)
	int iVertex = 0;        // ith visited vertex, initially 0 for source vertex
	int maxLevel = 0;       // max depth level reached
		
    // while there are more vertices to visit
	while (!Q.empty())
	{
		VertexID idVertex;
		
        // if DFT pop from back (LIFO), if BFT pop from front (FIFO)
		if (gtt == GRAPH_TRAVERSAL_BFT)
		{
			idVertex = Q.front();       // id of next vertex to visit
			Q.pop_front();              // remove from Q
		}
		else
		{
			idVertex = Q.back();        // id of next vertex to visit
			Q.pop_back();               // remove from Q
		}
		
		iLevel = vLevel[idVertex];      // depth level of given vertex
        
		if (iLevel > maxLevel) maxLevel = iLevel;
		
        // signal that we are visiting current vertex
		if (fn) action = fn(GRAPH_TRAVERSAL_VISIT, iVertex, iLevel, idSrc, idVertex, pArgs);
		iVertex++;
		
        // check whether client wants to interrupt traversal
		if (action == GRAPH_CALLBACK_STOP)
			return iVertex;
			
        // explore vertices adjacent to currrent vertex, and add to Q for later processing
		NeighborSet *pN = Neighbors(idVertex);
		for (int i = 0; i < pN->size(); i++)
		{
			VertexID idNeighbor = (*pN)[i].idNeighbor;  // id of an adjacent vertex
			
            // check to see whether we have visited this vertex before
			if (vLevel[idNeighbor] != -1)
			{
				// already processed this vertex, skip
				continue;
			}
			
			vLevel[idNeighbor] = iLevel + 1;            // record level of adjacent vertex
			Q.push_back(idNeighbor);                    // and add to Q for later processing
		}
		delete pN;
	}
	
    // signal that traversal is complete
	if (fn) fn(GRAPH_TRAVERSAL_END, iVertex, maxLevel, idSrc, 0, pArgs);
	return iVertex;
}