/*
 * Student's name: Master Hasan
 * Created by Muhammad Mohiuddin on 3/23/2021. This file contains definitions related to Dijkstra algorithm and some
 * additional functions related to random graph generator for contact tracing application.
 * Read inline comments especially for structures and function definitions written specifically for contact tracing
 * application.
 * NO CHANGE MUST BE MADE IN THIS FILE! except writing the name of the student above!
 */
#ifndef RANDOMGRAPHGENERATOR3_CONTACTTRACING_H
#define RANDOMGRAPHGENERATOR3_CONTACTTRACING_H

#define NUM_VERTICES 20
#define MAX_NUM_EDGES 5 // to represent maximum direct contacts with one person
// Structure definitions and function prototypes  copied from Program P7.3 from Kalicharan
#define NUM_OF_REGIONS 4
#define Infinity 99999

static char* regions[NUM_OF_REGIONS] ={"Durham", "Peel", "York", "GTA"};
typedef struct gEdge {
    int child, weight; //'child' is the location of the child vertex
    struct gEdge *nextEdge;
} GEdge, *GEdgePtr;

typedef struct {
    int id;
    int parent, cost, discover, finish, inDegree;
    GEdgePtr firstEdge;
} GVertex;

typedef struct graph {
    int numV;
    GVertex vertex[NUM_VERTICES+1];
} *Graph;
Graph newGraph(int);
void initSingleSource(Graph, int);
void siftDown(Graph, int, int[], int, int, int[]);
void siftUp(Graph, int[], int, int[]);
void followPath(Graph, int);
GVertex newGVertex(int id);
void addEdge(int srcID, int desID, int, Graph);
/**
 *
 * @param G
 */
void buildGraphRandom(Graph G);
/**
 *
 * @param G is the graph
 * @param source vertex id
 * @param destination vertex id
 * @return it returns number of intermediate contacts i.e., for direct contact it returns 0 and so on
 */
int DijkstraContactTracing(Graph G, int source, int destination);
/**
 *
 * @param G
 * @param region
 */
void printGraph(Graph G, int region);
/**
 *
 * @param in file pointer to write the graph to
 * @param G graph
 */
void writeGraphToFile(FILE* in, Graph G);
/**
 *
 * @param arr is the array that will be populated with random yet sorted list of integers
 * @param cnt is the size of the array
 * @param range is the range or spread of the set of random integers
 * @param offset is the offset of the set of random integers
 * @param distinct all numbers should be distinct then set it to 1 else 0
 * @param exclude any single integer to be excluded else pass INT_MIN or any integer outside the range provided
 * Note! For a distinct set of integers, make sure range > cnt, else the function will set range equal to cnt
 */
void generateRandomDistinctIntegersWithAnExclusion(int arr[], int cnt, int range, int offset, int distinct, int exclude);
/**
 *
 * @param region integer representing a region for the menu
 * @return its corresponding name
 */
char* regionLookup(int region);

#endif //RANDOMGRAPHGENERATOR3_CONTACTTRACING_H