#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "graph.h"

static inline double getRandom(void) { return (((double) rand()) / RAND_MAX); }

Vertex::Vertex(void)
{
	idVertex = -1;
}

Vertex::~Vertex(void)
{
}

Graph::Graph(void)
{
	pV = new VertexSet;
	if (!pV) throw GRAPH_ERR_OUTOFMEMORY;
	
	pE = new EdgeSet;
	if (!pE) throw GRAPH_ERR_OUTOFMEMORY;
	
	fDirected = false;
}


Graph::~Graph(void)
{
	if (pE) delete pE;
	if (pV) delete pV;
}


void Graph::Reset(int nVertices)
{
	if (pV->size() > 0) pV->clear();
	if (pE->size() > 0) pE->clear();
	
	if (nVertices > 0) 
	{
		pV->resize(nVertices);
		
		if (pV->size() != nVertices)
			throw GRAPH_ERR_OUTOFMEMORY;
			
		for (int i = 0; i < nVertices; i++)
			(*pV)[i].idVertex = (VertexID) i;
	}
	
	fDirected = false;
}

int Graph::V(void)
{	return pV->size();	}

int Graph::E(void)
{	return pE->size();	}

void Graph::SetDirected(void)
{	
	if (pE->size() > 0) throw GRAPH_ERR_ILLEGALOPERATION;
	fDirected = true;	
}

VertexID Graph::AddVertex(void)
{
	VertexID idVertex = (VertexID) pV->size();
	Vertex v;
	v.idVertex = idVertex;
	pV->push_back(v);
	return idVertex;
}


void Graph::AddEdge(VertexID idSrc, VertexID idDst)
{	AddEdge(idSrc, idDst, 1.0);	}

void Graph::AddEdge(VertexID idSrc, VertexID idDst, double w)
{
	CheckVertex(idSrc);
	CheckVertex(idDst);
	
	Edge e;
	e.idEdge = pE->size();
	e.idSrc  = idSrc;
	e.idDst  = idDst;
	e.w      = w;
	pE->push_back(e);
	
	(*pV)[idSrc].OE.push_back(e.idEdge);
	(*pV)[idDst].IE.push_back(e.idEdge);
	
	if (!fDirected)
	{
		(*pV)[idSrc].IE.push_back(e.idEdge);
		(*pV)[idDst].OE.push_back(e.idEdge);
	}

}


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
			ss >> nVertices;
			if (nVertices >= 0) Reset(nVertices);
		}
		else
		{
			VertexID idSrc;
			VertexID idDst;
			double w;
			
			ss >> idSrc;
			ss >> idDst;
			ss >> w;
			
			AddEdge(idSrc, idDst, w);
		}
	}
}

NeighborSet *Graph::Neighbors(VertexID idVertex)
{
	CheckVertex(idVertex);
	int nNeighbors = (*pV)[idVertex].OE.size();
	
	NeighborSet *pN = new NeighborSet(nNeighbors);
	
	for (int iNeighbor = 0; iNeighbor < nNeighbors; iNeighbor++)
	{
		EdgeID idEdge = (*pV)[idVertex].OE[iNeighbor];
		VertexID idNeighbor = (*pE)[idEdge].idDst;
		
		if ((!fDirected) && (idNeighbor == idVertex))
			idNeighbor = (*pE)[idEdge].idSrc;
			
		double w = (*pE)[idEdge].w;
		
		(*pN)[iNeighbor].idNeighbor = idNeighbor;
		(*pN)[iNeighbor].w = w;
	}
	
	return pN;
}

bool Graph::isDirected(void)
{   return fDirected;   }

inline void Graph::CheckVertex(VertexID idVertex)
{
	if ((idVertex < 0) || (idVertex >= V()))
		throw GRAPH_ERR_INDEXOUTOFRANGE;
}

int Graph::BFT(VertexID idSrc, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs)
{	return Traversal(GRAPH_TRAVERSAL_BFT, idSrc, fn, pArgs);	}

int Graph::DFT(VertexID idSrc, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs)
{	return Traversal(GRAPH_TRAVERSAL_DFT, idSrc, fn, pArgs);	}

void Graph::GenerateRandom(int N, bool directed, double p, double minLength, double maxLength)
{
	if (N < 0) throw GRAPH_ERR_BADGRAPHSIZE;
	
	if (minLength < 0) minLength = 0;
	if (maxLength < minLength) maxLength = minLength;
	
	Reset(N);
	
	if (directed) SetDirected();
	
	double lRange = maxLength - minLength;
	
	for (int i = 0; i < N - 1; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			if (getRandom() < p)
			{
				AddEdge(i, j, lRange * getRandom());
			}
		}
	}
}

int Graph::Traversal(GraphTraversalType gtt, VertexID idSrc, GraphCallbackAction fn(GraphCallbackOp, int, int, VertexID, VertexID, void *), void *pArgs)
{
	if ((gtt != GRAPH_TRAVERSAL_BFT) && (gtt != GRAPH_TRAVERSAL_DFT))
		throw GRAPH_ERR_INVALIDTRAVERSALTYPE;
		
	CheckVertex(idSrc);
	
	GraphCallbackAction action = GRAPH_CALLBACK_CONTINUE;
	std::vector<int> vLevel(V(), -1);
	std::deque<VertexID> Q;
	
	if (fn) action = fn(GRAPH_TRAVERSAL_START, 0, 0, idSrc, 0, pArgs);
	
	if (action == GRAPH_CALLBACK_STOP)
		return 0;
		
	vLevel[idSrc] = 0;
	Q.push_back(idSrc);
	
	int iLevel = 0;
	int iVertex = 0;
	int maxLevel = 0;
		
	while (!Q.empty())
	{
		VertexID idVertex;
		
		if (gtt == GRAPH_TRAVERSAL_BFT)
		{
			idVertex = Q.front();
			Q.pop_front();
		}
		else
		{
			idVertex = Q.back();
			Q.pop_back();
		}
		
		iLevel = vLevel[idVertex];
		if (iLevel > maxLevel) maxLevel = iLevel;
		
		if (fn) action = fn(GRAPH_TRAVERSAL_VISIT, iVertex, iLevel, idSrc, idVertex, pArgs);
		iVertex++;
		
		if (action == GRAPH_CALLBACK_STOP)
			return iVertex;
			
		NeighborSet *pN = Neighbors(idVertex);
		for (int i = 0; i < pN->size(); i++)
		{
			VertexID idNeighbor = (*pN)[i].idNeighbor;
			
			if (vLevel[idNeighbor] != -1)
			{
				// already processed this vertex, skip
				continue;
			}
			
			vLevel[idNeighbor] = iLevel + 1;
			Q.push_back(idNeighbor);
		}
		delete pN;
	}
	
	if (fn) fn(GRAPH_TRAVERSAL_END, iVertex, maxLevel, idSrc, 0, pArgs);
	return iVertex;
}