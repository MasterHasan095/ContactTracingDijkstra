/* Created by Muhammad Mohiuddin on 3/23/2021 and revised on 11/11/21. Several functions related to Dijkstra algorithm
 are taken from Program P7.3 from Kalicharan and their full implementation is given below and these MUST NOT BE CHANGED!
THE ASSIGNMENT REQUIRES YOU TO FOLLOW THE TO DO list and implement four functions as per the TO DO list!
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "contactTracing.h"
GEdgePtr newGEdge(int, int);
void initSingleSource(Graph G, int s) {
    for (int i = 1; i <= G->numV; i++) {
        G -> vertex[i].cost = INT_MAX;
        G -> vertex[i].parent = 0;
    }
    G -> vertex[s].cost = 0;
} //end initSingleSource
void printCostPath(Graph G) {
    for (int i = 1; i <= G -> numV; i++) {
        printf("Cost to %d: %2d, Path: ", G -> vertex[i].id, G -> vertex[i].cost);
        followPath(G, i);
        printf("\n");
    }
} //end printCostPath
void followPath(Graph G, int c) {
    if (c != 0) {
        followPath(G, G -> vertex[c].parent);
        if (G -> vertex[c].parent != 0) printf(" -> "); //do not print -> for source
        printf("rinting 12 : %d ", G -> vertex[c].id);
    }
} //end followPath
void siftUp(Graph G, int heap[], int n, int heapLoc[]) {
    //sifts up heap[n] so that heap[1..n] contains a heap based on cost
    int siftItem = heap[n];
    int child = n;
    int parent = child / 2;
    while (parent > 0) {
        if (G->vertex[siftItem].cost >= G->vertex[heap[parent]].cost) break;
        heap[child] = heap[parent]; //move down parent
        heapLoc[heap[parent]] = child;
        child = parent;
        parent = child / 2;
    }
    heap[child] = siftItem;
    heapLoc[siftItem] = child;
} //end siftUp
void siftDown(Graph G, int key, int heap[], int root, int last, int heapLoc[]) {
    int smaller = 2 * root;
    while (smaller <= last) { //while there is at least one child
        if (smaller < last) //there is a right child as well; find the smaller
            if (G->vertex[heap[smaller+1]].cost < G->vertex[heap[smaller]].cost)
                smaller++;
        //'smaller' holds the index of the smaller child
        if (G -> vertex[key].cost <= G -> vertex[heap[smaller]].cost) break;
        //cost[key] is bigger; promote heap[smaller]
        heap[root] = heap[smaller];
        heapLoc[heap[smaller]] = root;
        root = smaller;
        smaller = 2 * root;
    } //end while
    heap[root] = key;
    heapLoc[key] = root;
} //end siftDown
Graph newGraph(int n) {
    if (n > NUM_VERTICES) {
        printf("\nToo big. Only %d vertices allowed.\n", NUM_VERTICES);
        exit(1);
    }
    Graph p = (Graph) malloc(sizeof(struct graph));
    p -> numV = n;
    return p;
} //end newGraph

GVertex newGVertex(int id) {
    GVertex temp;
    temp.id = id;
    temp.firstEdge = NULL;
    return temp;
}
void addEdge(int srcID, int destID, int weight, Graph G) {
//    printf("Receiving Data : Src : %d  ,  dest : %d\n", srcID, destID);

    //add an edge srcID -> destID with a given weight
    int h, k;
    //find srcID in the list of nodes; its location is h
    for (h = 1; h <= G->numV; h++) {
        if (srcID == G->vertex[h-1].id) {
            break;
        }
    }

    //find destID in the list of nodes; its location is k
    for (k = 1; k <= G->numV; k++) if (destID == G->vertex[k-1].id) break;


    if (h > G->numV || k > G->numV) {
        printf("No such edge: %d -> %d\n", h, k);
        exit(1);
    }
//    printf("H and K are  : H : %d  ,  K : %d\n\n", h, k);


    GEdgePtr ep = newGEdge(k, weight); //create edge vertex
    // add it to the list of edges, possible empty, from X;
    // it is added so that the list is in order by vertex id
    GEdgePtr prev, curr;
    prev = curr = G->vertex[h-1].firstEdge;
    while (curr != NULL && destID > G->vertex[curr->child].id)  {
        prev = curr;
        curr = curr->nextEdge;
    }
    if (prev == curr) {
        ep->nextEdge = G->vertex[h-1].firstEdge;
        G->vertex[h-1].firstEdge = ep;
    } else {
        ep->nextEdge = curr;
        prev->nextEdge = ep;
    }
} //end addEdge
GEdgePtr newGEdge(int c, int w) {
    //return a pointer to a new GEdge node
    GEdgePtr p = (GEdgePtr) malloc(sizeof(GEdge));
    p->child = c;
    p->weight = w;
    p->nextEdge = NULL;
    return p;
}
/**
 *
 * @param region it takes an integer representing region number from the menu
 * @return the corresponding name of the region
 */
char* regionLookup(int region) {
    return regions[region];
}
/**
 * This function builds a graph comprising of the number of vertices given by the user. Random number of edges varying from
 * 0 to MAX_NUM_EDGES for every vertex to build a random graph representing people and their meetings with others.
 * Hint! this function may call generateRandomDistinctIntegersWithAnExclusion function to generate random edges
 * and then invoke addEdge function iteratively to build the graph
 * @param G is the graph
 */
void buildGraphRandom(Graph G) {
    for (int i = 0; i < G->numV; ++i) {
        G->vertex[i] = newGVertex(i + 1);
    }

    for (int i = 0; i < G->numV; ++i) {
        int numEdges = rand() % (MAX_NUM_EDGES + 1); // Random number of edges from 0 to MAX_NUM_EDGES
        int edges[numEdges];
        generateRandomDistinctIntegersWithAnExclusion(edges, numEdges, G->numV-1, 1, 1, i + 1);
        for (int j = 0; j < numEdges; ++j) {
            addEdge(i+1, edges[j], 1, G);
        }
    }
}

/**
 *
 * @param arr is the array that will be populated with distinct random list of integers
 * @param cnt is the size of the array
 * @param range is the range or spread of the set of random integers
 * @param offset is the offset of the set of random integers
 * @param distinct all numbers should be distinct then set it to 1 else 0
 * @param exclude any single integer to be excluded else pass INT_MIN or any integer outside the range provided
 * Note! For a distinct set of integers, make sure range > cnt, else the function will set range equal to cnt
 */
void generateRandomDistinctIntegersWithAnExclusion(int arr[], int cnt, int range, int offset, int distinct, int exclude) {
    range = cnt > range ? cnt : range;
    for (int dex = 0; dex < cnt; dex++) {
        int randInt = exclude;
        while (randInt == exclude) {
            randInt = rand() % range + offset;
        }
        arr[dex] = randInt;
        if (distinct) {
            for (int ctr = 0; ctr < dex; ctr++) {
                if (arr[dex] == arr[ctr]) {
                    dex--;
                    break;
                }
            }
        }
    }
}

/**
 * This function displays data as per the screenshots shown in the assignment specs. Region is provided as an integer only
 * to get its text using mapping function. The name of the region is used only in the header statement on the console
 * @param G
 * @param region
 */
void printGraph(Graph G, int region) {

    int contactCounter = 0;
    printf("%-15s%-15s%-25s\n", "Person's", "# of direct", "IDs of people");
    printf("%-15s%-15s%-25s\n", "ID", "contacts", "contacted directly");
    for (int i = 0; i < G->numV; ++i) {
        if (i%25 == 0 && i != 0 ){
            printf("%-15s%-15s%-25s\n", "ID", "# of contacts", "IDs of contacts");
        }
        printf("%-15d", G->vertex[i].id);
        GEdgePtr edge = G->vertex[i].firstEdge;
        while (edge != NULL) {
            contactCounter++;
            edge = edge->nextEdge;
        }
        printf("%-15d", contactCounter, "");
        edge = G->vertex[i].firstEdge;
        while (edge != NULL) {
            printf("%-4d ", edge->child);
            edge = edge->nextEdge;
        }
        printf("\n");
        contactCounter = 0;
    }
}
/**
 * Write the graph to a text file
 * @param in is the file pointer
 * @param G is the graph
 */
void writeGraphToFile(FILE* in, Graph G) {
    // TODO 3 write function: void writeGraphToFile(FILE* in, Graph G)
    puts("Writing data to file");

    if (in == NULL) {
        printf("File could not be opened\n");
    } else {
        fprintf( in, "%d\n", G->numV); // Total num of vertices
        for (int i = 0; i < G->numV; ++i) {
            fprintf( in, "%-3d", G->vertex[i].id); // All vertices
        }
        fprintf(in,"\n");

        int contactCounter = 0;
        for (int i = 0; i < G->numV; ++i) {
            fprintf( in, "%d :", G->vertex[i].id);

            GEdgePtr edge = G->vertex[i].firstEdge;
            while (edge != NULL) {
                contactCounter++;
                edge = edge->nextEdge;
            }
            fprintf(in," %d :", contactCounter);
            edge = G->vertex[i].firstEdge;
            while (edge != NULL) {
                fprintf(in,"%3d", edge->child);
                edge = edge->nextEdge;
            }
            fprintf(in,"\n");
            contactCounter = 0;
        }
    }
}
//        int ctr = 0;
//        while (top){
//            fprintf( cfPtr, "%s %s %s %d %d %d\n", top->data.race, top->data.region, top->data.town, top->data.familySize, top->data.testedPositive, top->data.fullyVaccinated);
//            ctr++;
//            top=top->next;
//        }
////        Last Record None 0 0 0
//        fprintf(cfPtr, "Last Record None 0 0 0");
//        fclose(cfPtr);
//        printf("%d records and the sentinerl record were written to %s\n", ctr, fileName);
//        displayRecordsFromFile();
//    }

/**
 *
 * @param G is the graph of people and their contact history. This is primarily based on Dijkstra function given in the
 * Dijkstra application, however, you need to only calculate the path between source and one destination, so it must
 * add a check for destination at appropriate place in the Dijkstra code and return if true.
 * @param source source person ID
 * @param destination destination person ID
 * @return it returns number of intermediate contacts i.e., for direct contact it returns 0 and so on
 * Hint! Copy Dijkstra function from Dijkstra application here and make appropriate changes
 *
 */
int DijkstraContactTracing(Graph G, int s, int d) {
    int heap[NUM_VERTICES + 1], heapLoc[NUM_VERTICES + 1];
    initSingleSource(G, s);
    for (int i = 1; i <= G->numV; i++) {
        heap[i] = i;
        heapLoc[i] = i;
    }
    heap[1] = s;
    heap[s] = 1;
    heapLoc[s] = 1;
    heapLoc[1] = s;
    int heapSize = G->numV;

    while (heapSize > 0) {
        int u = heap[1];
        if (G->vertex[u].cost == INT_MAX) break; // no paths to other vertices
        heap[1] = heap[heapSize];
        heapLoc[heap[heapSize]] = 1;
        heapSize--;
        siftDown(G, heap[1], heap, 1, heapSize, heapLoc);

        GEdgePtr edge = G->vertex[u].firstEdge;
        while (edge != NULL) {
            int v = edge->child;
            if (G->vertex[u].cost + edge->weight < G->vertex[v].cost) {
                G->vertex[v].cost = G->vertex[u].cost + edge->weight;
                G->vertex[v].parent = u;
                siftUp(G, heap, heapLoc[v], heapLoc);
            }
            if (v == d) { // destination reached
                int count = 0;
                int temp = v;
                while (G->vertex[temp].parent != s) {
                    count++;
                    temp = G->vertex[temp].parent;
                }
                return count;
            }
            edge = edge->nextEdge;
        }
    }
    return -1; // destination not reachable
}
//ends DijkstraContactTracing



