//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

// Macros for counters
#define CMP InstrCount[0] //Comparitions
#define ITER InstrCount[1] //Loop iterations
#define ADD InstrCount[2] //Adding
#define MEMOPS InstrCount[3] //Array access

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);

  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Given graph and start vertex for the shortest-paths
  result->graph = g;
  result->startVertex = startVertex;

  unsigned int V = GraphGetNumVertices(g);
  
  /* Initialize problem variables */
  unsigned int* mark = (unsigned int*) malloc(V * sizeof(unsigned int)); 
  int* pred = (int*) malloc(V * sizeof(int)); 
  int* dist = (int*) malloc(V * sizeof(int)); 

  /* Set init values */
  for (unsigned int i = 0; i < V ; i++) {
    mark[i] = 0;
    pred[i] = -1;
    dist[i] = -1;
  }
  dist[startVertex] = 0;
  mark[startVertex] = 1;

  unsigned int updated = 0; /* Flag to know when to quit early */
  unsigned int degree;
  int cost; /* Distance to current vertice */
  
  /* Run algorithm V - 1 times */
  for (unsigned int j = 0; j < V - 1; j++) {
    
    updated = 0;

    /* Run through all vertices */
    for (unsigned int v = 0; v < V; v++) {
      
      /* Skip vertices with infinite cost */
      if ((cost = dist[v]) == -1) continue; CMP++;
      
      degree = GraphIsDigraph(g) ? GraphGetVertexOutDegree(g, v) 
            : GraphGetVertexDegree(g, v);

      /* If vertice has no edges skip it */
      if (degree == 0) continue; CMP++;
        
      unsigned int* nbrs = GraphGetAdjacentsTo(g, v);
      
      /* Run through all conected edges */
      for (unsigned int n = 1; n < degree + 1; n++) {
          
        if ((dist[nbrs[n]] > cost + 1) | (dist[nbrs[n]] == -1)) {
          dist[nbrs[n]] = cost + 1; ADD++;
          pred[nbrs[n]] = v;
          mark[nbrs[n]] = 1; MEMOPS += 3;
          updated = 1;
        } CMP++;

        ITER++;
      }
      
      free(nbrs);
    }

    /* Quit early if nothing changed */
    if (updated == 0) break; CMP++;
  }

  result->marked = mark;
  result->predecessor = pred;
  result->distance = dist;

  return result;
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}
