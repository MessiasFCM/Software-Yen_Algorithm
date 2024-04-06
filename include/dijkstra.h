#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "paths.h"
#include "graph.h"

typedef struct {
    int no;
    int custo;
} NoHeap;

typedef struct {
    int custo;
    int noAnterior;
} InfoCaminho;

void relaxar(NoHeap* heap, int* tamHeap, InfoCaminho* caminhos, int src, int dest, int peso);
int estaNoHeap(NoHeap* heap, int tamHeap, int no);
void inserirNoHeap(NoHeap* heap, int* tamHeap, int no, int custo);
NoHeap extrairMin(NoHeap* heap, int* tamHeap);
void heapMinimo(NoHeap* heap, int i, int tamHeap);
ShortestPath* dijkstra(Graph* grafo, int origem);

#endif
