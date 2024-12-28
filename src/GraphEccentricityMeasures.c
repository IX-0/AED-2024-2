//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

<<<<<<< HEAD
// Student Name : Igor Xavier
// Student Number : 
=======
// Student Name : Igor Baltarejo
// Student Number : 118832
>>>>>>> c27436278a22f7f963d3c6f9696a24b5ea631186
// Student Name : João Barreira
// Student Number : 120054

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // Para usar INT_MAX


#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* graph) {
  assert(graph != NULL);

  // Initialize measures structure
  GraphEccentricityMeasures* measures =
      (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  assert(measures != NULL);

  unsigned int numVertices = GraphGetNumVertices(graph);

  // Execute all pairs shortest distances algorithm
  GraphAllPairsShortestDistances* apsd = GraphAllPairsShortestDistancesExecute(graph);

  // Allocate space for the eccentricity of each vertex
  measures->eccentricity = (int*)malloc(numVertices * sizeof(int));
  assert(measures->eccentricity != NULL);

  measures->graph = graph;
  measures->graphRadius = INT_MAX;  // Initialize with a very high value
  measures->graphDiameter = -1;     // Initialize with a very low value

  // Calculate eccentricity of each vertex
  for (unsigned int v = 0; v < numVertices; v++) {
    int maxDistance = -1;
    for (unsigned int w = 0; w < numVertices; w++) {
      int distance = GraphGetDistanceVW(apsd, v, w);
      if (distance != -1 && distance > maxDistance) {
        maxDistance = distance;
      }
    }
    measures->eccentricity[v] = maxDistance;

    // Update radius and diameter
    if (maxDistance != -1) {
      if (maxDistance < measures->graphRadius) {
        measures->graphRadius = maxDistance;
      }
      if (maxDistance > measures->graphDiameter) {
        measures->graphDiameter = maxDistance;
      }
    }
  }

  // Determine central vertices
  unsigned int numCentralVertices = 0;
  for (unsigned int v = 0; v < numVertices; v++) {
    if (measures->eccentricity[v] == measures->graphRadius) {
      numCentralVertices++;
    }
  }

  measures->centralVertices =
      (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
  assert(measures->centralVertices != NULL);

  measures->centralVertices[0] = numCentralVertices;
  unsigned int idx = 1;
  for (unsigned int v = 0; v < numVertices; v++) {
    if (measures->eccentricity[v] == measures->graphRadius) {
      measures->centralVertices[idx++] = v;
    }
  }

  // Free memory of the distances module
  GraphAllPairsShortestDistancesDestroy(&apsd);

  return measures;
}


void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);
  assert(p->centralVertices != NULL);

  unsigned int numCentralVertices = p->centralVertices[0];
  unsigned int* result =
      (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
  assert(result != NULL);

  for (unsigned int i = 0; i <= numCentralVertices; i++) {
    result[i] = p->centralVertices[i];
  }

  return result;
}


// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  printf("Graph Radius: %d\n", p->graphRadius);
  printf("Graph Diameter: %d\n", p->graphDiameter);

  unsigned int V = GraphGetNumVertices(p->graph);
  printf("Vertex Eccentricities:\n");
  for (unsigned int v = 0; v < V; v++) {
    printf("Vertex %u: %d\n", v, p->eccentricity[v]);
  }

  printf("Central Vertices (Radius = %d):\n", p->graphRadius);
  unsigned int numCentralVertices = p->centralVertices[0];
  for (unsigned int i = 1; i <= numCentralVertices; i++) {
    printf("%u ", p->centralVertices[i]);
  }
  printf("\n");
}
