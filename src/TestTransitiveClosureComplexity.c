#include <assert.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "GraphTransitiveClosure.h"
#include "instrumentation.h"

#define VERTICES InstrCount[3]
#define EDGES InstrCount[4]
#define ID InstrCount[5]

/// Generate worst case cenario, use with root at V - 1
static Graph* generateGraphWorstCase(unsigned int V, unsigned int digraph){
    Graph* gp = GraphCreate(V, digraph, 0);
    
    //Add edges in a way to force Bellman-Ford to evaluate V - 1 times
    for (unsigned int i = V - 1; i > 0; i--) {
        GraphAddEdge(gp, i, i - 1);
    }

    return gp;
};

/// Generate best case cenario, use with root at 0
static Graph* generateGraphBestCase(unsigned int V, unsigned int digraph){
    Graph* gp = GraphCreate(V, digraph, 0);
    
    //Add edges in a way to force Bellman-Ford to evaluate 1 time
    for (unsigned int i = 0; i < V - 1; i++) {
        GraphAddEdge(gp, i, i + 1);
    }

    return gp;
};

static Graph* generateGraphAverageCase(unsigned int V, unsigned int digraph) {
    
    Graph* gp = GraphCreate(V, digraph, 0);

    unsigned int v;
    unsigned int w;
    for (unsigned int i = 0; i < V * 2 ;) {
        v = rand() % V;
        w = rand() % V;
        if (v != w) {
            if (GraphAddEdge(gp, v, w)) i++; 
        }
    } 

    return gp;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Wrong usage!\n");
        exit(EXIT_FAILURE);
    } 

    InstrName[0] = "cmp";
    InstrName[1] = "iter";
    InstrName[2] = "memops";
    InstrName[3] = "vertices";
    InstrName[4] = "edges";
    InstrName[5] = "id";
    InstrCalibrate();
    InstrReset();
    
    int maxV = atoi(argv[1]); 
    Graph* gp;

    for (int i = 5; i < maxV ;i++) {
        //Best case
        gp = generateGraphBestCase(i, 1);

        InstrReset();
        ID = 3;
        VERTICES = (unsigned long) GraphGetNumVertices(gp);
        EDGES = (unsigned long) GraphGetNumEdges(gp);
        GraphComputeTransitiveClosure(gp);
        InstrPrint();

        GraphDestroy(&gp);

        //Worst case
        gp = generateGraphWorstCase(i, 1);

        InstrReset();
        ID = 1;
        VERTICES = (unsigned long) GraphGetNumVertices(gp);
        EDGES = (unsigned long) GraphGetNumEdges(gp);
        GraphComputeTransitiveClosure(gp);
        InstrPrint();

        GraphDestroy(&gp);

        //Average case
        gp = generateGraphAverageCase(i, 1);

        InstrReset();
        ID = 2;
        VERTICES = (unsigned long) GraphGetNumVertices(gp);
        EDGES = (unsigned long) GraphGetNumEdges(gp);
        GraphComputeTransitiveClosure(gp);
        InstrPrint();

        GraphDestroy(&gp);
    }

    return 0;
}
