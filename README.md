# LibGastonForSofia
А wrap-up c++ library for gSpan algorithm implemented by Taku Kudo

The detailes about the algorithm can be found in 

> X. Yan and J. Han, “gSpan: Graph-Based Substructure Pattern Mining,” in Data Mining, 2002. ICDM 2003. Proceedings. …, 2002, pp. 721–724.[1] X. Yan and J. Han, “gSpan: Graph-Based Substructure Pattern Mining,” in Data Mining, 2002. ICDM 2003. Proceedings. …, 2002, pp. 721–724.

This implementation is referenced by Xifeng Yan, the author of gSpan, [in his web page](https://www.cs.ucsb.edu/~xyan/software/gSpan.htm). 
And is taken from [library gBoost](http://www.nowozin.net/sebastian/gboost/#download), Graph Boosting Toolbox for Matlab.
The implementation of the algorithm is taken from [the web cite of Siegfried Nijssen](http://liacs.leidenuniv.nl/~nijssensgr/gaston/index.html).  gBoost is dual-licensed under both the GNU General Public License, version 2 and the Mozilla Public License, version 1.1. Accordingly we remain among these settings.

## Some details about the algorithm
Gaston finds frequent graph patterns starting from empty graphs and moving to larger and large ones. It uses canoncial code DFS to avoid redandancy in generated subgraphs. 

## Compilation of the library
You will need
* [premake4](https://premake.github.io/download.html)

The next step is to convert the project to your most loved envirement. For that run

> $ premake4 {configuration}

The most widely used configurations are 

> $ premake4 vs2005|vs2008|vs2010|vs2012 \# for diferent version of visual studio   *sln** file

> $ premake4 gmake \# for GNU make file

> $ premake4 codeblocks \# for cbp file of code::blocks IDE

> $ premake4 clean \# for removing the created files

The script converts the description from the file __premake4.lua__ to the format of your IDE in love. The file is placed in the __build__ subdirectory.
Then, I guess, you know what to do with the resulting file.

## Interface of the LibGastonForSofia library
The interface of the library can be found in the [include file](https://github.com/AlekseyBuzmakov/LibgSpanForSofia/blob/master/inc/LibgSpanForSofia.h).

It consist of one function **RungSpan** that accepts the same parameters as the original implementation plus a reference to the callback function and an arbitrary data pointer passed to the callback function.
The introduction of the callback function allows for a flexibal interaction with the **gSpan** algorithm.
```c++
/*
	TODO
*/
```

### Callback function

Below you can see the prototype of the callback function
```c++
/**
 * A type of function that is used for reporting newly found graphs
 * @param graph is the reported graph
 * @return returns if the graph should be further expanded
 */
typedef bool (LibgSpanAPI *ReportGraphCallback)( LibgSpanDataRef data, const LibgSpanGraph* graph );
```

This callback function is used to report found graphs, encoded by structure **LibgSpanGraph**. The reference to data coincides with the one passed to **RungSpan** function.
It is never used inside the algorithm and required only for the user if he/she is needed to pass some additional data to the callback function (e.g., an instance of the callback-processing class).

### Graph encoding in callback

The graph is encoded by the following structure and basically is just an enumeration of graph vertices, edges, and zero-based objects (transactions) IDs that refer to graphs from the dataset (tidsets).

```c++
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
```

### The **return** value of the callback function.
The callback function returns a boolean value that adds an aditional control over **gSpan**. It is very usefull for branch cutting. Basically it returns the necessity of the expanision of the current graph pattern. If the user knows somehow that the current graph and all its supergraph are of no use, it can return **false** from the callback function saving some computational time. If the callback function always returns **true**, then the result is exactly the same as the one returned by original **Gaston** with the corresponding parameters.
