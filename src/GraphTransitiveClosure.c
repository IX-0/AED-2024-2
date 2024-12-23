//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g));
  assert(GraphIsWeighted(g) == 0);
  
  unsigned int V = GraphGetNumVertices(g);

  /* New graph */
  Graph* ng = GraphCreate(V, 1, 0);
  assert(ng != NULL);

  for (unsigned int v = 0; v < V; v++) {
    GraphBellmanFordAlg* bf = GraphBellmanFordAlgExecute(g, v);
    
    /* For each bellman ford vertice (bfv) */
    for (unsigned int bfv = 0; bfv < V; bfv++) {
      if ((v != bfv) & GraphBellmanFordAlgReached(bf, bfv)) {
        GraphAddEdge(ng, v, bfv);
      }
    }

    GraphBellmanFordAlgDestroy(&bf);
  }

  return ng;
}
