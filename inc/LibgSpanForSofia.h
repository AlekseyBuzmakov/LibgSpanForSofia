#ifndef _LibgSpanForSofia_H_
#define _LibgSpanForSofia_H_

#ifndef _WIN32
#define __stdcall
#endif

#define LibgSpanAPI __stdcall

#ifdef __cplusplus
extern "C" {
#endif

// Enumeration of possible 'gSpan' algorithm modes.
enum TgSpanRunningMode {
  // No limitations on found graphs
  GRM_All = 0,
  // Only tree-like graphs (including pathes)
  GRM_Trees,
  // Only pathes
  GRM_Pathes,

  GRM_EnumCount
};

// Type for storing label of vertices
typedef int VertexLabelType;
// Type for storing label of edges
typedef int EdgeLabelType;
/**
 * A sructure encoding a graph edge
 */
struct LibgSpanGraphEdge {
  // The 0-based indices of the vertices for this edge.
  int From;
  int To;
  // Label of the edge.
  EdgeLabelType Label;
};

/**
 * A Sructure encoding a graph
 */
struct LibgSpanGraph {
  // Number of vertices and edges in the graph
  int VertexCount;
  int EdgeCount;
  // List of vertices, the size is equal VertexCount
  VertexLabelType* Vertices;
  // List of edges, the size is equal EdgesCount
  LibgSpanGraphEdge* Edges;
  // The support of this graph
  int Support;
  // The set of objects covering this pattern. Length of Objects is @var Support.
  int* Objects;
};

// Any data that should be passed to callback.
typedef void* LibgSpanDataRef;

/**
 * A type of function that is used for reporting newly found graphs
 * @param graph is the reported graph
 * @return returns if the graph should be further expanded
 */
typedef bool (LibgSpanAPI *ReportGraphCallback)( LibgSpanDataRef data, const LibgSpanGraph* graph );

/**
 * This function that runs gaston on the input dataset.
 * @param inputFileName is the name of the file containing the descriptions of graphs
 * @param support is minimal support (the number of graphs from the dataset) of a discovered graph pattern (a subgraph)
 * @param callback is the callback function, which is used for reporting discovered graphs
 * @param minsize is the minimal number of nodes in discovered graphs 
 * @param maxsize is the maximal number of nodes in discovered graphs (negative number means 'no limitations')
 * @param directed is the flag for indicating if the graphs in the database are directed
 * @return is the success of the operation.
 */
typedef bool (LibgSpanAPI *RungSpanFunc)( LibgSpanDataRef data,
                                          const char* inputFileName, int support, ReportGraphCallback callback, int minsize /*= 0*/, int maxsize /*= -1*/, bool directed /*= false*/ );
bool LibgSpanAPI RungSpan( LibgSpanDataRef data,
    const char* inputFileName, int support, ReportGraphCallback callback, int minsize = 0, int maxsize = -1, bool directed = false );

#ifdef __cplusplus
}
#endif

#endif
