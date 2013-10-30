typedef struct structGSPDistance {
    VertexID idDst;
    VertexID idPrevious;
    double   distance;
} GSPDistance;

typedef std::vector<GSPDistance> GSPDistanceSet;

class GSPResults {
    public:
    VertexID idSrc;
    GSPDistanceSet ds;
};

class GraphShortestPath {

    public:
    GraphShortestPath(void);
    ~GraphShortestPath(void);
    GSPResults *Compute(VertexID idVertex, Graph & g);

};

static const int _iGSPErrBase = 0x433;
typedef enum enumGSPError {
    GSP_ERR_INVALIDVERTEX
} GSPError;